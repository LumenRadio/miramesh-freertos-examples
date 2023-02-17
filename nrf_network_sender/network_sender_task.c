#include <stdint.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"

#include "semphr.h"

#include "miramesh.h"
#include "miramesh_sys.h"

#include "bsp.h"
#include "freertos_miramesh_integration.h"
#include "swi_callback_handler.h"

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
    .mode = MIRA_NET_MODE_MESH,
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
    for (i = 0; i < data_len - 1; i++) {
        printf("%c", ((char *) data)[i]);
    }
    printf("\n");
}

xTaskHandle controllerTaskHandle;
xTaskHandle ledTaskHandle;

/* When to call the time_callback next time: */
static volatile uint32_t next_tick;

static void ledTask_notification_callback(
    void)
{
    if (ledTaskHandle != NULL) {
        portBASE_TYPE should_yield = pdFALSE;
        xTaskNotifyFromISR(ledTaskHandle, 1, eSetBits, &should_yield);
        portYIELD_FROM_ISR(should_yield);
    }
}

static void time_callback(
    mira_net_time_t tick,
    void *storage)
{
    (void) storage;

    /*
     * Make pin blink every 1.28 seconds, assuming 10ms ticks
     *
     * Using 1.28 seconds, to keep calculation fast, and therefore latency low
     */
    bsp_board_led_invert(BSP_BOARD_LED_2);

    /* Invoke notification from lower priority interrupt */
    invoke_swi_callback(ledTask_notification_callback);

    /* Calculate next tick */
    next_tick = (tick & 0xffffff80) + 0x00000080;
}

static void ledTask(
    void *pvParameters)
{
    (void) pvParameters;

    while (1) {
        uint32_t val;
        if (xTaskNotifyWait(0, 1, &val, portMAX_DELAY) == pdPASS) {
            if (val & 1) {
                /* Schedule next wakeup */
                mira_net_time_schedule(next_tick, time_callback, NULL);
            }
        }
    }
}

/**
 * Start the MiraMesh stack and send UDP messages every minute.
 */
static void controller(
    void *pvParameters)
{
    static mira_net_udp_connection_t *udp_connection;

    static mira_net_address_t net_address;
    char buffer[MIRA_NET_MAX_ADDRESS_STR_LEN];
    static mira_status_t res;
    static const char *message = "Hello Network";

    printf("Starting Node (Sender).\n");

    /* Wait for MiraMesh integration to be ready */
    freertos_miramesh_integration_wait_for_ready();

    vTaskDelayMs(1000); // Give miramesh time to start

    printf("Sending one packet every %d seconds\n", SEND_INTERVAL);

    mira_net_init(&net_config);

    printf("Controller task started\r\n");

    /*
     * Open a connection, but don't specify target address yet, which means
     * only mira_net_udp_send_to() can be used to send packets later.
     */
    udp_connection = mira_net_udp_connect(NULL, 0, udp_listen_callback, NULL);

    bool start_schedule = true;
    while (1) {
        mira_net_state_t net_state = mira_net_get_state();

        if (net_state != MIRA_NET_STATE_JOINED) {
            start_schedule = true;
            printf(
                "Waiting for network (state is %s)\n",
                net_state == MIRA_NET_STATE_NOT_ASSOCIATED ? "not associated"
                : net_state == MIRA_NET_STATE_ASSOCIATED   ? "associated"
                : net_state == MIRA_NET_STATE_JOINED       ? "joined"
                : "UNKNOWN");
            vTaskDelayMs(CHECK_NET_INTERVAL * 1000);
        } else {
            if (start_schedule) {
                if (mira_net_time_get_time((uint32_t *) &next_tick)
                    == MIRA_SUCCESS) {
                    start_schedule = false;
                    mira_net_time_schedule(next_tick, time_callback, NULL);
                } else {
                    printf("ERROR: mira_net_time_get_time failed.\n");
                }
            }

            /* Try to retrieve the root address. */
            res = mira_net_get_root_address(&net_address);

            if (res != MIRA_SUCCESS) {
                printf("Waiting for root address (res: %d)\n", res);

                vTaskDelayMs(CHECK_NET_INTERVAL * 1000);
            } else {
                /*
                * Root address is successfully retrieved, send a message to the
                * root node on the given UDP Port.
                */
                printf("Sending to address: %s\n",
                    mira_net_toolkit_format_address(buffer, &net_address));
                mira_net_udp_send_to(udp_connection, &net_address, UDP_PORT,
                    message, strlen(message));
                vTaskDelayMs(SEND_INTERVAL * 1000);
            }
        }
    }

    mira_net_udp_close(udp_connection);
}

void start_miramesh_app(
    void)
{
    register_swi_callback(ledTask_notification_callback);

    /*
     * Create the controller task
     */
    if (pdPASS != xTaskCreate(controller,
        (const char *const) "Hello MiraMesh",
        256,
        NULL,
        3,
        &controllerTaskHandle) ) {
        // printf("xTaskCreate failed");
    }

    if (pdPASS != xTaskCreate(ledTask,
        (const char *const) "ledTask",
        100,
        NULL,
        3,
        &ledTaskHandle) ) {
        // printf("xTaskCreate failed");
    }
}
