#include "network_receiver.h"
#include "miramesh.h"
#include "FreeRTOS.h"
#include "log.h"

#define UDP_PORT 456

/* Keep static to not take space on the miramesh stack */
static log_line_t network_receiver_line;

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
    .mode = MIRA_NET_MODE_ROOT_NO_RECONNECT,

    .rate = MIRA_NET_RATE_FAST,
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
    char addr_buf[MIRA_NET_MAX_ADDRESS_STR_LEN];
    uint16_t i;

    log_line_start(&network_receiver_line);
    log_line_add(
        &network_receiver_line,
        "Received message from [%s]:%u: ",
        mira_net_toolkit_format_address(addr_buf, metadata->source_address),
        metadata->source_port
    );
    /* Received packets are not necessarily null-terminated */
    for (i = 0; i < data_len && ((char *) data)[i] != '\0'; i++) {
        log_line_add(&network_receiver_line, "%c", ((char *) data)[i]);
    }
    log_line_end(&network_receiver_line);
}

void network_receiver_init(
    void)
{
    /*
     * Startup networking
     *
     * This needs to be called from within a running task during startup
     */

    mira_status_t result = mira_net_init(&net_config);
    configASSERT(result == MIRA_SUCCESS);

    /*
     * Start listening for connections on the given UDP Port.
     *
     * The callback will be executed from within MiraMesh task. To handle the
     * packets in a separate task, copying the packet out and queue it is needed
     * to be handled by the application.
     */
    mira_net_udp_listen(UDP_PORT, udp_listen_callback, NULL);
}