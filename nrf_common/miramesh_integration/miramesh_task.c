#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "task.h"

#include "semphr.h"

#include "nrf_sdh.h"
#include "nrf_sdh_freertos.h"

#include "miramesh.h"
#include "miramesh_sys.h"

#include "MiraMeshConfig.h"

#include "freertos_miramesh_integration.h"

static xTaskHandle miramesh_task_handle;
static SemaphoreHandle_t miramesh_api_lock = NULL;
static SemaphoreHandle_t miramesh_integration_initialized = NULL;

static void wake_miramesh_task_from_irq(
    void)
{
    if (miramesh_task_handle != NULL) {
        portBASE_TYPE switch_task = pdFALSE;
        xTaskNotifyFromISR(miramesh_task_handle, 1, eSetBits, &switch_task);
        portYIELD_FROM_ISR(switch_task);
    }
}

static void wake_miramesh_task_from_app(
    void)
{
    if (miramesh_task_handle != NULL) {
        xTaskNotify(miramesh_task_handle, 1, eSetBits);
    }
}

static void init_miramesh_lock(
    void)
{
    miramesh_api_lock = xSemaphoreCreateRecursiveMutex();
}

static void miramesh_lock(
    void)
{
    if (xSemaphoreTakeRecursive(miramesh_api_lock, portMAX_DELAY) != pdTRUE) {
        ASSERT(!pdTRUE);
        while (1);
    }
}

static void miramesh_unlock(
    void)
{
    if (xSemaphoreGiveRecursive(miramesh_api_lock) != pdTRUE) {
        ASSERT(!pdTRUE);
        while (1);
    }
}

static void *miramesh_malloc(
    mira_size_t size,
    void *storage)
{
    (void) storage;

    void *retval = malloc(size);

    if (retval == NULL) {
        ASSERT(!pdTRUE);
        while (1);
    }

    return retval;
}

static void miramesh_task(
    void *pvParameters)
{
    mira_mem_set_alloc_callback(miramesh_malloc, NULL);

    printf("MiraMesh task started\r\n");

    /* Signal that initialisation is over */
    xSemaphoreGive(miramesh_integration_initialized);

    do {
        uint32_t val;
        /* Wait for wakeup from the wake_miramesh_* functions */
        if (xTaskNotifyWait(0, 1, &val, portMAX_DELAY) == pdPASS) {
            if (val & 1) {
                /* Run the MiraMesh tasks that need to run: */
                miramesh_run_once();
            }
        }
    } while (true);
}

void task_hook(
    void *arg)
{
    (void) arg;
}

void freertos_miramesh_integration_init(
    void)
{
    /* Create a semaphore to signal when the system is ready.
     * It is created in the Taken state. */
    miramesh_integration_initialized = xSemaphoreCreateBinary();

    init_miramesh_lock();

#if MIRAMESH_STARTS_SOFTDEVICE
    nrf_sdh_freertos_init(task_hook, NULL);
    ret_code_t err = nrf_sdh_enable_request();
    if (err != NRF_SUCCESS) {
        printf("Failed to start SD: %ld\n", err);
        while (1);
    }
#endif

    const miramesh_hardware_cfg_t *hardware = MIRAMESH_HARDWARE_CFG;

    miramesh_config_t miramesh_config = {
        .callback = {
            .api_lock = miramesh_lock,
            .api_unlock = miramesh_unlock,
            .wakeup_from_app_callback = wake_miramesh_task_from_app,
            .wakeup_from_irq_callback = wake_miramesh_task_from_irq,
        },
        .hardware = {
        },
        .certificate = {
            .start = MIRAMESH_CERTIFICATE_START,
            .end = MIRAMESH_CERTIFICATE_END,
        }
    };
    miramesh_config.hardware = *hardware;

    if (!nrf_sdh_is_enabled()) {
        printf("Softdevice must be enabled\n");
        while (1)
            ;
    }

    printf("MiraMesh init called\n");
    miramesh_init(
        &miramesh_config,
#ifdef MIRAMESH_FRONTEND_CFG
        MIRAMESH_FRONTEND_CFG
#else
        NULL
#endif
    );

    portBASE_TYPE rc = xTaskCreate(miramesh_task,
        (const char *const) "MiraMesh",
        2048, // Stacksize
        NULL,
        MIRAMESH_TASK_PRIO,
        &miramesh_task_handle);
    if (rc != pdPASS) {
        printf("xTaskCreate failed");
        while (1);
    }
}

void freertos_miramesh_integration_wait_for_ready(
    void)
{
    ASSERT(miramesh_integration_initialized != NULL);

    /* The semaphore is free when init is done, so wait until it is taken. */
    while (xSemaphoreTake(miramesh_integration_initialized,
        portMAX_DELAY) != pdTRUE)
        /* pass */;

    /* Give the semaphore for the next waiting task */
    while (xSemaphoreGive(miramesh_integration_initialized) != pdTRUE)
        /* pass */;
}
