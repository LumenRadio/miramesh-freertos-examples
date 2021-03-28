#include "miramesh_sys.h"
#include "nrf_sdh_soc.h"
#include "MiraMeshConfig.h"

static void sd_evt_observer(
    uint32_t evt_id,
    void *context)
{
    (void) evt_id;
    (void) context;

    miramesh_handle_sd_event(evt_id);
}

NRF_SDH_SOC_OBSERVER(m_sd_evt_miramesh, 0, sd_evt_observer, 0);

#if MIRAMESH_RTC_IRQn == 0
void RTC0_IRQHandler(void)
#elif MIRAMESH_RTC_IRQn == 1
void RTC1_IRQHandler(void)
#elif MIRAMESH_RTC_IRQn == 2
void RTC2_IRQHandler(void)
#else
#error "Incorrect MIRAMESH_RTC_IRQn value"
#endif
{
    miramesh_rtc_irq_handler();
}

#if MIRAMESH_SWI_IRQn == 0
void SWI0_EGU0_IRQHandler(void)
#elif MIRAMESH_SWI_IRQn == 2
void SWI2_EGU2_IRQHandler(void)
#elif MIRAMESH_SWI_IRQn == 3
void SWI3_EGU3_IRQHandler(void)
#elif MIRAMESH_SWI_IRQn == 4
void SWI4_EGU4_IRQHandler(void)
#elif MIRAMESH_SWI_IRQn == 5
void SWI5_EGU5_IRQHandler(void)
#else
#error "Incorrect MIRAMESH_SWI_IRQn value"
#endif
{
    miramesh_swi_irq_handler();
}

void SWI1_EGU1_IRQHandler(
    void)
{
    miramesh_swi1_irq_handler();
}
