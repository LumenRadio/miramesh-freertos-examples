#include <miramesh.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"

#include "netstat.h"

#include <mira_diag_log.h>
typedef enum {
#include "events.h.gen"
} events_t;

static bool always_true (
    void)
{
    return true;
}

#define EVENT_VARS(evt) \
    volatile uint32_t count_ ## evt; \
    uint32_t last_ ## evt

EVENT_VARS(EVT_NET_RF_SLOTS_CALL_END_FAILED);
EVENT_VARS(EVT_NET_RF_SLOTS_CALL_START_FAILED);
EVENT_VARS(EVT_NET_RF_SLOTS_DELAY_END_FAILED);
EVENT_VARS(EVT_NET_RF_SLOTS_DELAY_START_FAILED);
EVENT_VARS(EVT_NET_RF_SLOTS_EXTEND_FAILED);
EVENT_VARS(EVT_NET_RF_SLOTS_IRQ_INVALID);
EVENT_VARS(EVT_NET_RF_SLOTS_RX_END_FAILED);
EVENT_VARS(EVT_NET_RF_SLOTS_RX_START_FAILED);
EVENT_VARS(EVT_NET_RF_SLOTS_RX_POWER_END_FAILED);
EVENT_VARS(EVT_NET_RF_SLOTS_SLOT_DISCARD);
EVENT_VARS(EVT_NET_RF_SLOTS_SLOT_DROP);
EVENT_VARS(EVT_NET_RF_SLOTS_SLOT_END);
EVENT_VARS(EVT_NET_RF_SLOTS_SLOT_FAILED);
EVENT_VARS(EVT_NET_RF_SLOTS_SLOT_START);
EVENT_VARS(EVT_NET_RF_SLOTS_SYNC_FAILED);
EVENT_VARS(EVT_NET_RF_SLOTS_TX_END_FAILED);
EVENT_VARS(EVT_NET_RF_SLOTS_TX_START_FAILED);

static void evt_irq_callback(
    uint32_t event,
    va_list ap)
{

#define HANDLE_EVT(evt) \
case evt: \
    count_ ## evt++; \
    break

    switch (event) {
        HANDLE_EVT(EVT_NET_RF_SLOTS_CALL_END_FAILED);
        HANDLE_EVT(EVT_NET_RF_SLOTS_CALL_START_FAILED);
        HANDLE_EVT(EVT_NET_RF_SLOTS_DELAY_END_FAILED);
        HANDLE_EVT(EVT_NET_RF_SLOTS_DELAY_START_FAILED);
        HANDLE_EVT(EVT_NET_RF_SLOTS_EXTEND_FAILED);
        HANDLE_EVT(EVT_NET_RF_SLOTS_IRQ_INVALID);
        HANDLE_EVT(EVT_NET_RF_SLOTS_RX_END_FAILED);
        HANDLE_EVT(EVT_NET_RF_SLOTS_RX_START_FAILED);
        HANDLE_EVT(EVT_NET_RF_SLOTS_RX_POWER_END_FAILED);
        HANDLE_EVT(EVT_NET_RF_SLOTS_SLOT_DISCARD);
        HANDLE_EVT(EVT_NET_RF_SLOTS_SLOT_DROP);
        HANDLE_EVT(EVT_NET_RF_SLOTS_SLOT_END);
        HANDLE_EVT(EVT_NET_RF_SLOTS_SLOT_FAILED);
        HANDLE_EVT(EVT_NET_RF_SLOTS_SLOT_START);
        HANDLE_EVT(EVT_NET_RF_SLOTS_SYNC_FAILED);
        HANDLE_EVT(EVT_NET_RF_SLOTS_TX_END_FAILED);
        HANDLE_EVT(EVT_NET_RF_SLOTS_TX_START_FAILED);
    }
#undef HANDLE_EVT
}

static const mira_diag_log_callbacks_t evt_callbacks = {
    .is_debug_enabled = always_true,
    .is_info_enabled = always_true,
    .irq_log = {
        .debug = evt_irq_callback,
        .info = evt_irq_callback,
        .warn = evt_irq_callback,
        .fatal = evt_irq_callback,
    },
};

void netstat_report_process(
    void *ctx)
{
    while (1) {
        /* Print the number of times an event has happened during the last
           X seconds, if the event has happened at all.
        */
#define HANDLE_EVT(evt) \
    uint32_t tmp_ ## evt = count_ ## evt - last_ ## evt; \
    last_ ## evt = count_ ## evt; \
    if (tmp_ ## evt != 0) { \
        printf(#evt ":%lu\n", (unsigned long)tmp_ ## evt); \
    }

        HANDLE_EVT(EVT_NET_RF_SLOTS_CALL_END_FAILED);
        HANDLE_EVT(EVT_NET_RF_SLOTS_CALL_START_FAILED);
        HANDLE_EVT(EVT_NET_RF_SLOTS_DELAY_END_FAILED);
        HANDLE_EVT(EVT_NET_RF_SLOTS_DELAY_START_FAILED);
        HANDLE_EVT(EVT_NET_RF_SLOTS_EXTEND_FAILED);
        HANDLE_EVT(EVT_NET_RF_SLOTS_IRQ_INVALID);
        HANDLE_EVT(EVT_NET_RF_SLOTS_RX_END_FAILED);
        HANDLE_EVT(EVT_NET_RF_SLOTS_RX_START_FAILED);
        HANDLE_EVT(EVT_NET_RF_SLOTS_RX_POWER_END_FAILED);
        HANDLE_EVT(EVT_NET_RF_SLOTS_SLOT_DISCARD);
        HANDLE_EVT(EVT_NET_RF_SLOTS_SLOT_DROP);
        /* Skipping slot_end and slot_start since they always happen */
        // HANDLE_EVT(EVT_NET_RF_SLOTS_SLOT_END);
        HANDLE_EVT(EVT_NET_RF_SLOTS_SLOT_FAILED);
        // HANDLE_EVT(EVT_NET_RF_SLOTS_SLOT_START);
        HANDLE_EVT(EVT_NET_RF_SLOTS_SYNC_FAILED);
        HANDLE_EVT(EVT_NET_RF_SLOTS_TX_END_FAILED);
        HANDLE_EVT(EVT_NET_RF_SLOTS_TX_START_FAILED);

#if DUMP_TX_QUEUE
        if ((tmp_EVT_NET_RF_SLOTS_SLOT_DROP > 500)
            || (tmp_EVT_NET_RF_SLOTS_SLOT_DISCARD > 500)) {
            extern void lrtsch_mix_tx_queue_debug_dump(
                void);
            lrtsch_mix_tx_queue_debug_dump();
        }
#endif

#undef HANDLE_EVT

        vTaskDelay(10 * 1000);
    }
}

void netstat_init(
    void)
{
    mira_diag_log_set_callbacks(&evt_callbacks);

    static xTaskHandle netstat_handle;

    portBASE_TYPE rc = xTaskCreate(netstat_report_process,
        (const char *const) "NetStat",
        256, // Stacksize
        NULL,
        3,
        &netstat_handle);
    if (rc != pdPASS) {
        printf("xTaskCreate failed");
        while (1);
    }
    printf("net_stats: init\n");
}
