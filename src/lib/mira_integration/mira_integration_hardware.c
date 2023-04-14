#include "mira_integration_hardware.h"
#include "mira_integration_config.h"
#include "miramesh.h"
#include "FreeRTOS.h"

#include <nrfx_ppi.h>

void mira_integration_hardware_init(
    miramesh_hardware_cfg_t *hwconfig)
{
    int status;
    int i;

    /* Values may be updated to match the application in general */
    hwconfig->rtc = MIRAMESH_RTC_ID;
    hwconfig->rtc_irq_prio = MIRAMESH_RTC_IRQ_PRIO;
    hwconfig->swi = MIRAMESH_SWI_ID;
    hwconfig->swi_irq_prio = MIRAMESH_SWI_IRQ_PRIO;

    /* Allocate PPIs needed for MiraMesh */
    for (i = 0; i < MIRAMESH_SYS_NUM_PPIS_USED; i++) {
        status = nrfx_ppi_channel_alloc(&hwconfig->ppi_idx[i]);
        configASSERT(status == 0);
    }
    for (i = 0; i < MIRAMESH_SYS_NUM_PPI_GROUPS_USED; i++) {
        status = nrfx_ppi_group_alloc(&hwconfig->ppi_group_idx[i]);
        configASSERT(status == 0);
    }
}