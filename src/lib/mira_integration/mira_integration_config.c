#include "mira_integration_config.h"
#include "miramesh.h"
#include <stddef.h>

/* Frontend configuration. Can be used to set mira_integration_frontend below */
static const mira_net_frontend_config_t frontend_config __attribute__ ((unused)) = {
    .gain_cb_bypass_tx = -30,
    .gain_cb_bypass_rx = -30,
    .gain_cb_active_tx = 240,
    .gain_cb_active_rx = 150,

    .gpio_mode_control = {
        MIRA_GPIO_PIN(1, 11),     /* CPS */
        MIRA_GPIO_PIN(1, 10),     /* CSD */
        MIRA_GPIO_PIN(1, 12)     /* CTX */
    },
    .gpio_antsel = MIRA_GPIO_PIN(1, 6),

    /* Bit indexes in these member variables relate to array elements in .gpio_mode_control[] */
    .pin_values_bypass_tx = 0x02,
    .pin_values_bypass_rx = 0x02,
    .pin_values_active_tx = 0x07,
    .pin_values_active_rx = 0x03,
    .pin_values_idle = 0x00,

    /* Bit 0 in elements of this array relate to levels for the pin defined by .gpio_antsel. */
    .pin_values_antsel = {
        0x00,
        0x01
    },
    .pin_values_antsel_idle = 0x00,
};

/* Frontend to use, can be either a frontend config or NULL */
const mira_net_frontend_config_t* mira_integration_frontend = NULL;