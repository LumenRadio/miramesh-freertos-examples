#include "network_sender.h"
#include "miramesh.h"
#include "FreeRTOS.h"
#include "log.h"
#include "task.h"
#include <string.h>

#include "tinyprintf.h"

#define UDP_PORT 456

#define CHECK_NET_INTERVAL pdMS_TO_TICKS(1000)
#define SEND_INTERVAL      pdMS_TO_TICKS(10000)

/*
 * Identifies as a root.
 * Retrieves data from the nodes.
 */
static const mira_net_config_t net_config = {
    .pan_id = 0x13243546,
    .key = {
        0x11, 0x12, 0x13, 0x14,
        0x21, 0x22, 0x23, 0x24,
        0x31, 0x32, 0x33, 0x34,
        0x41, 0x42, 0x43, 0x44
    },
    /* Prioritize initial network startup, which is good for testing.
     *
     * This has the drawback that the network takes a much
     * longer time to recover if the root node is restarted. */
    .mode = MIRA_NET_MODE_MESH,

    .rate = MIRA_NET_RATE_MID,
    .antenna = 0,
    .prefix = NULL /* default prefix */
};

void network_sender_task(
    void *pv_param)
{
    char addr_buf[MIRA_NET_MAX_ADDRESS_STR_LEN];
    mira_net_udp_connection_t *conn;
    mira_status_t res;
    mira_net_state_t net_state;
    mira_net_address_t net_address;
    char message[64];

    conn = mira_net_udp_connect(NULL, 0, NULL, NULL);

    for (;;) {

        net_state = mira_net_get_state();
        if (net_state != MIRA_NET_STATE_JOINED) {
            log_line(
                "Waiting for network (state is %s)",
                net_state == MIRA_NET_STATE_NOT_ASSOCIATED ? "not associated"
                : net_state == MIRA_NET_STATE_ASSOCIATED ? "associated"
                : net_state == MIRA_NET_STATE_JOINED ? "joined"
                : "UNKNOWN"
            );
            vTaskDelay(CHECK_NET_INTERVAL);
        } else {
            res = mira_net_get_root_address(&net_address);
            if (res != MIRA_SUCCESS) {
                log_line("Waiting for root address (res: %d)", res);
                vTaskDelay(CHECK_NET_INTERVAL);
            } else {
                /*
                 * Root address is successfully retrieved, send a message to the
                 * root node on the given UDP Port.
                 */
                log_line("Sending to address: %s",
                    mira_net_toolkit_format_address(addr_buf, &net_address));
                tfp_sprintf(message, "Hello from FreeRTOS");
                mira_net_udp_send_to(conn, &net_address, UDP_PORT,
                    message, strlen(message));
                vTaskDelay(SEND_INTERVAL);
            }
        }

    }
}

void network_sender_init(
    void)
{
    /*
     * Startup networking
     *
     * This needs to be called from within a running task during startup
     */

    mira_status_t result = mira_net_init(&net_config);
    configASSERT(result == MIRA_SUCCESS);

    xTaskCreate(network_sender_task,
        "net TX",
        512, // Stacksize
        NULL,
        3,
        NULL);
}