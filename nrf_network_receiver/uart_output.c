#include "bsp.h"
#include "nordic_common.h"
#include "app_error.h"
#include "app_uart.h"
#if defined(UART_PRESENT)
#include "nrf_uart.h"
#endif
#if defined(UARTE_PRESENT)
#include "nrf_uarte.h"
#endif

static void uart_eventhandler()
{
}

int _write(
    int fd,
    char *ptr,
    int len)
{
    // TODO: This busywaits for every character.
    for(int i = 0; i < len; ++i) {
        while (app_uart_put(ptr[i]) != NRF_SUCCESS)
            ;
    }
    while (app_uart_flush())
        ;
    return len;
}

void init_uart(
    void)
{
    ret_code_t err_code;

    app_uart_comm_params_t comm_params =
        {
            8, // RX pin
            6, // TX pin
            0,
            0,
            (app_uart_flow_control_t)0,
            false,
            NRF_UARTE_BAUDRATE_115200};

    err_code = app_uart_init(&comm_params,
                             NULL,
                             uart_eventhandler,
                             APP_IRQ_PRIORITY_LOWEST);

    APP_ERROR_CHECK(err_code);
}
