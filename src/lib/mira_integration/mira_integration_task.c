#include "mira_integration_task.h"
#include "mira_integration_config.h"
#include "miramesh.h"

#include "swi_callback_handler.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

/**
 * Recursive mutex for MiraMesh access
 *
 * It is required that miramesh api internally is only called from one task at a
 * time. Therefore, miramesh implements an interface with api_lock and
 * api_unlock callbacks when entering and exiting the API.
 *
 * Note that API will be locked during callbacks, from the API, but it is then
 * valid to call the API back. Therefore, the mutex used needs to be a recursive
 * mutex.
 */
static SemaphoreHandle_t mira_integration_api_lock = NULL;

/**
 * MiraMesh main task handle
 *
 * MiraMesh is running its internal scheduler in a single FreeRTOS task.
 */
static TaskHandle_t mira_integration_task_handle;

/**
 * Lock the miramesh API
 */
static void mira_integration_lock(void)
{
    /* timeout with portMAX_DELAY should never return unless success */
    xSemaphoreTakeRecursive(mira_integration_api_lock, portMAX_DELAY);
}

/**
 * Unlock the miramesh API
 */
static void mira_integration_unlock(void)
{
    BaseType_t status = xSemaphoreGiveRecursive(mira_integration_api_lock);
    configASSERT(status == pdTRUE);
}

/**
 * Wakeup mira mesh task swi callback
 *
 * Callback to allow waking up miramesh task from interrupts of an ISR with
 * priority low enough for FreeRTOS API to be called.
 *
 * Used via the invoke_swi_callback when needed to be woken up from higher
 * priority interrupts
 */
static void miramesh_notification_callback(void)
{
    BaseType_t switch_task = pdFALSE;
    xTaskNotifyFromISR(mira_integration_task_handle, 1, eSetBits, &switch_task);
    portYIELD_FROM_ISR(switch_task);
}

/**
 * Wakeup mira mesh task from IRQ
 *
 * Note that this may be called from interrupts of higher priority than the
 * maximum allowed for FreeRTOS. Therefore a mechanism is needed to drop
 * priority before waking up the miramesh task.
 */
static void mira_integration_wakeup_from_irq(void)
{
    /* Invoke notification from lower priority interrupt */
    invoke_swi_callback(miramesh_notification_callback);
}

static void mira_integration_wakeup_from_app(void)
{
    if (mira_integration_task_handle != NULL) {
        xTaskNotify(mira_integration_task_handle, 1, eSetBits);
    }
}

/**
 * Main MiraMesh task
 *
 * This is running the miramesh main loop
 */
static void mira_integration_task(void* pv_param)
{
    do {
        /* Wait for wakeup from the wake_miramesh_* functions */
        if (xTaskNotifyWait(0, 1, NULL, portMAX_DELAY) == pdPASS) {
            /* Run the MiraMesh tasks that need to run: */
            miramesh_run_once();
        }
    } while (true);
}

void mira_integration_task_init(miramesh_callback_cfg_t* cbconfig)
{
    mira_integration_api_lock = xSemaphoreCreateRecursiveMutex();
#if configQUEUE_REGISTRY_SIZE > 0
    vQueueAddToRegistry(mira_integration_api_lock, "MiraMesh API");
#endif

    cbconfig->api_lock = mira_integration_lock;
    cbconfig->api_unlock = mira_integration_unlock;
    cbconfig->wakeup_from_irq_callback = mira_integration_wakeup_from_irq;
    cbconfig->wakeup_from_app_callback = mira_integration_wakeup_from_app;

    register_swi_callback(miramesh_notification_callback);

    BaseType_t status = xTaskCreate(mira_integration_task,
                                    MIRAMESH_TASK_NAME,
                                    MIRAMESH_TASK_STACK_SIZE,
                                    NULL,
                                    MIRAMESH_TASK_PRIO,
                                    &mira_integration_task_handle);
    configASSERT(status == pdPASS);
}