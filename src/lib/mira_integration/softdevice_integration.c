#include "softdevice_integration.h"

#include "nrf_sdh.h"
#include "nrf_sdh_freertos.h"

#include "nrf_drv_clock.h"

#include "FreeRTOS.h"

void softdevice_integration_init(void)
{
    ret_code_t err_code;

    /* Initialize clock driver for better time accuracy in FREERTOS */
    err_code = nrf_drv_clock_init();
    configASSERT(err_code == NRFX_SUCCESS);

    nrf_sdh_freertos_init(NULL, NULL);
    ret_code_t err = nrf_sdh_enable_request();
    configASSERT(err == NRF_SUCCESS);
}