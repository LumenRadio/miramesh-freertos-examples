#include "FreeRTOS.h"
#include "task.h"

#include "uart.h"
#include "log.h"
#include "mira_integration.h"
#include "print_freertos_tasks.h"
#include "fault_handlers.h"

#include "nrf.h"

#include "miramesh.h"

static void main_task(
    void *pv_param);

int main(
    void)
{
    /*
     * Initialize what's required before startup of scheduler. Rest is started
     * in a main task
     */

    /* Enable faults */
    fault_handlers_init();
    vPortInitialiseBlocks();

    /* Initialize miramesh. Also needed for scheduling */
    mira_integration_init();

    /* Initialize serial port for debug interface */
    uart_init(&(const uart_config_t) {
        .baudrate = UART_BAUDRATE_BAUDRATE_Baud1M,
        .pin_txd = 6,
        .pin_rxd = 0xffffffff,
        .irq_priority = _PRIO_APP_HIGH
    });
    log_init();

    /* Activate deep sleep mode */
    // SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

    /* Start main task, which will start up rest of application */
    xTaskCreate(main_task, "main", 512, NULL, 1, NULL);

    /* Start scheduler */
    vTaskStartScheduler();
    return 0;
}

static void main_task(
    void *pv_param)
{
    mira_sys_device_id_t devid;

    /*
     * Start up application. From this point, I/O should be running, and
     * therefore logging is possible. Each submodule can therefore be started
     * from this task one at a time, with proper status logging.
     */
    log_line(" ");
    log_line("Started uart_tx");
    log_line(" ");
    mira_sys_get_device_id(&devid);
    log_line("Device ID: %02x%02x%02x%02x%02x%02x%02x%02x",
        devid.u8[0], devid.u8[1], devid.u8[2], devid.u8[3],
        devid.u8[4], devid.u8[5], devid.u8[6], devid.u8[7]
    );
    log_line(" ");

    vTaskDelay(pdMS_TO_TICKS(1000));
    for (;;) {
        print_freertos_tasks();
        vTaskDelay(pdMS_TO_TICKS(5000));
    }

    /* A task can not exit, but it can suspend itself. */
    vTaskSuspend(NULL);
}