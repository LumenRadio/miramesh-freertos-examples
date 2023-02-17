#ifndef MIRAMESHCONFIG_H
#define MIRAMESHCONFIG_H

#include <nrfx_ppi.h>

/* The FreeRTOS MiraMesh task is running at priority */
#define MIRAMESH_TASK_PRIO  (4)

#define MIRAMESH_RTC_IRQn   1
#define MIRAMESH_SWI_IRQn   0

#define SWI_CALLBACK_HANDLER_IRQn SWI3_EGU3_IRQn
#define SWI_CALLBACK_HANDLER_IRQ_PRIO 5
#define SWI_CALLBACK_LIST_SIZE 10

/* --------------------------------------- */
/* Define the used hardware                */

#if DEFINE_MIRAMESH_HARDWARE_CFG
static const miramesh_hardware_cfg_t *miramesh_get_hardware_cfg(
    void)
{
    static miramesh_hardware_cfg_t hardware_cfg = {
        .rtc = MIRAMESH_RTC_IRQn,
        .rtc_irq_prio = 6,
        .swi = MIRAMESH_SWI_IRQn,
        .swi_irq_prio = 5
    };

    for (int i = 0; i < MIRAMESH_SYS_NUM_PPIS_USED; ++i) {
        if (nrfx_ppi_channel_alloc(&hardware_cfg.ppi_idx[i]) != 0) {
            while (1);
        }
    }
    for (int i = 0; i < MIRAMESH_SYS_NUM_PPI_GROUPS_USED; ++i) {
        if (nrfx_ppi_group_alloc(&hardware_cfg.ppi_group_idx[i]) != 0) {
            while (1);
        }
    }

    return &hardware_cfg;
}

#define MIRAMESH_HARDWARE_CFG miramesh_get_hardware_cfg()
#endif

/* --------------------------------------- */
/* Define the frontend if used             */

/* MIRAMESH_FRONTEND_CFG is only defined if a frontend is used */
#if 0
#define MIRAMESH_FRONTEND_CFG \
    & (const mira_net_frontend_config_t) { \
        .gain_cb_bypass_tx = -30, \
        .gain_cb_bypass_rx = -30, \
        .gain_cb_active_tx = 240, \
        .gain_cb_active_rx = 150, \
 \
        .gpio_mode_control = { \
            MIRA_GPIO_PIN(1, 11), /* CPS */ \
            MIRA_GPIO_PIN(1, 10), /* CSD */ \
            MIRA_GPIO_PIN(1, 12) /* CTX */ \
        }, \
        .gpio_antsel = MIRA_GPIO_PIN(1, 6), \
 \
        /* Bit indexes in these member variables relate to array elements in .gpio_mode_control[] */ \
        .pin_values_bypass_tx = 0x02, \
        .pin_values_bypass_rx = 0x02, \
        .pin_values_active_tx = 0x07, \
        .pin_values_active_rx = 0x03, \
        .pin_values_idle = 0x00, \
 \
        /* Bit 0 in elements of this array relate to levels for the pin defined by .gpio_antsel. */ \
        .pin_values_antsel = { \
            0x00, \
            0x01 \
        }, \
        .pin_values_antsel_idle = 0x00, \
    }
#endif

/* --------------------------------------- */
/* Definitions for the certificate pool:   */

extern const uint8_t __CertificateStart;
extern const uint8_t __CertificateEnd;

#define MIRAMESH_CERTIFICATE_START (&__CertificateStart)
#define MIRAMESH_CERTIFICATE_END (&__CertificateEnd)

/* --------------------------------------- */
/* Should MiraMesh start Softdevice ?      */

#define MIRAMESH_STARTS_SOFTDEVICE (1)

/* --------------------------------------- */

#endif
