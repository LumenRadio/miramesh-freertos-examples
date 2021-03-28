#include <stdint.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"

#include "semphr.h"

#include "miramesh.h"
#include "miramesh_sys.h"

#include "bsp.h"
#include "freertos_miramesh_integration.h"

#define UDP_PORT 456
#define SEND_INTERVAL 60
#define CHECK_NET_INTERVAL 1

#define vTaskDelayMs(ms) vTaskDelay((ms)) // Close enough

/*
 * Identifies as a node.
 * Sends data to the root.
 */
static const mira_net_config_t net_config = {
    .pan_id = 0x13243546,
    .key = {
        0x11, 0x12, 0x13, 0x14,
        0x21, 0x22, 0x23, 0x24,
        0x31, 0x32, 0x33, 0x34,
        0x41, 0x42, 0x43, 0x44
    },
    .mode = MIRA_NET_MODE_FAST_STARTUP_ROOT,
    .rate = MIRA_NET_RATE_MID,
    .antenna = 0,
    .prefix = NULL /* default prefix */
};

static void udp_listen_callback(
    mira_net_udp_connection_t *connection,
    const void *data,
    uint16_t data_len,
    const mira_net_udp_callback_metadata_t *metadata,
    void *storage)
{
    (void) connection;
    (void) storage;

    char buffer[MIRA_NET_MAX_ADDRESS_STR_LEN];
    uint16_t i;

    printf("Received message from [%s]:%u: ",
        mira_net_toolkit_format_address(buffer, metadata->source_address),
        metadata->source_port);
    for (i = 0; i < data_len; i++) {
        printf("%c", ((char *) data)[i]);
    }
    printf("\n");
}

xTaskHandle controllerTaskHandle;
xTaskHandle ledTaskHandle;

/* When to call the time_callback next time: */
static volatile uint32_t next_tick;

static void time_callback(
    mira_net_time_t tick,
    void *storage)
{
    (void) storage;

    /* Calculate next tick */
    next_tick = (tick & 0xffffff80) + 0x00000080;

    /*
     * Make pin blink every 1.28 seconds, assuming 10ms ticks
     *
     * Using 1.28 seconds, to keep calculation fast, and therefore latency low
     */
    if (ledTaskHandle != NULL) {
        portBASE_TYPE yield = pdFALSE;
        bsp_board_led_invert(BSP_BOARD_LED_2);
        vTaskNotifyGiveFromISR(ledTaskHandle, &yield);
        portYIELD_FROM_ISR(yield);
    }
}

static void ledTask(
    void *pvParameters)
{
    (void) pvParameters;

    while (1) {
        ulTaskNotifyTake(1, portMAX_DELAY);
          /* Schedule next wakeup */
          mira_net_time_schedule(next_tick, time_callback, NULL);
    }
}

/**
 * Start the MiraMesh stack and send UDP messages every minute.
 */
static void controller(
    void *pvParameters)
{
    static mira_net_udp_connection_t *udp_connection;

    printf("Starting Node (Receiver).\n");

    /* Wait for MiraMesh integration to be ready */
    freertos_miramesh_integration_wait_for_ready();

    mira_net_init(&net_config);

    if (mira_net_time_get_time((uint32_t *) &next_tick) == MIRA_SUCCESS) {
        xTaskNotify(ledTaskHandle, 1, eSetBits);
    } else {
        printf("ERROR: mira_net_time_get_time.\n");
    }

    printf("Controller task started\r\n");

    /*
     * Open a connection, but don't specify target address yet, which means
     * only mira_net_udp_send_to() can be used to send packets later.
     */
    udp_connection = mira_net_udp_listen_address(NULL, UDP_PORT, udp_listen_callback, NULL);

    while (1) {
        vTaskDelayMs(SEND_INTERVAL * 1000);
    }

    mira_net_udp_close(udp_connection);
}

void start_miramesh_app(
    void)
{
    if (pdPASS != xTaskCreate(ledTask,
        (const char *const) "ledTask",
        100,
        NULL,
        0,
        &ledTaskHandle) ) {
        // printf("xTaskCreate failed");
    }

    /*
     * Create the controller task
     */
    if (pdPASS != xTaskCreate(controller,
        (const char *const) "Hello MiraMesh",
        256,
        NULL,
        7,
        &controllerTaskHandle) ) {
        // printf("xTaskCreate failed");
    }

}
