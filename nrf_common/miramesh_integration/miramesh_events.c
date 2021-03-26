#include "miramesh_sys.h"
#include "nrf_sdh_soc.h"

static void sd_evt_observer(
    uint32_t evt_id,
    void *context)
{
    (void) evt_id;
    (void) context;

    miramesh_handle_sd_event(evt_id);
}

NRF_SDH_SOC_OBSERVER(m_sd_evt_miramesh, 0, sd_evt_observer, 0);

void RTC2_IRQHandler(
    void)
{
    miramesh_rtc_irq_handler();
}

void SWI0_EGU0_IRQHandler(
    void)
{
    miramesh_swi_irq_handler();
}

void SWI1_EGU1_IRQHandler(
    void)
{
    miramesh_swi1_irq_handler();
}
