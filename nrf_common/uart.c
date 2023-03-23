#include "uart.h"
#include "nrf.h"

#include "FreeRTOS.h"
#include "stream_buffer.h"

#include <stdint.h>
#include <stdbool.h>

#define UART_STREAM_SIZE 256
#define UART_ENABLE_PRINTF 1

static StreamBufferHandle_t uart_tx_stream;
static StreamBufferHandle_t uart_rx_stream;
static bool uart_tx_ready;

#if UART_ENABLE_PRINTF

static SemaphoreHandle_t uart_mutex = NULL;

int _write(
    int fd,
    char *ptr,
    int len)
{
    if (uart_mutex == NULL) {
        return -1;
    }

    if (xSemaphoreTake(uart_mutex, portMAX_DELAY) != pdTRUE) {
        return -1;
    }

    /*
     * TODO: This will block if stream is full
     */
    uart_tx(ptr, len);

    xSemaphoreGive(uart_mutex);

    return len;
}

#endif

void uart_init(
    const uart_config_t *config)
{
    NRF_UART0->ENABLE = UART_ENABLE_ENABLE_Enabled;
    NRF_UART0->BAUDRATE = config->baudrate;
    NRF_UART0->CONFIG = 0;

    NRF_UART0->PSEL.RTS = 0xffffffff;
    NRF_UART0->PSEL.CTS = 0xffffffff;
    NRF_UART0->PSEL.TXD = config->pin_txd;
    NRF_UART0->PSEL.RXD = config->pin_rxd;

    if ((config->pin_txd & UART_PSEL_TXD_CONNECT_Msk) == 0) {
        /* TXD enabled */
        uart_tx_stream = xStreamBufferCreate(UART_STREAM_SIZE, 1);
        NRF_UART0->INTENSET = (1 << UART_INTENSET_TXDRDY_Pos);
        uart_tx_ready = true; /* Ready from start */
        NRF_UART0->TASKS_STARTTX = 1;
    } else {
        /* TXD disabled */
        uart_tx_stream = NULL;
        uart_tx_ready = false;
    }

    if ((config->pin_rxd & UART_PSEL_RXD_CONNECT_Msk) == 0) {
        /* RXD enabled */
        uart_rx_stream = xStreamBufferCreate(UART_STREAM_SIZE, 1);
        NRF_UART0->INTENSET = (1 << UART_INTENSET_RXDRDY_Pos);
        NRF_UART0->TASKS_STARTRX = 1;
    } else {
        /* RXD disabled */
        uart_rx_stream = NULL;
    }

    NVIC_ClearPendingIRQ(UARTE0_UART0_IRQn);
    NVIC_SetPriority(UARTE0_UART0_IRQn, config->irq_priority);
    NVIC_EnableIRQ(UARTE0_UART0_IRQn);

#if UART_ENABLE_PRINTF
    uart_mutex = xSemaphoreCreateBinary();
#endif
}

void uart_tx(
    const char *buffer,
    int len)
{
    configASSERT(uart_tx_stream != NULL);
    xStreamBufferSend(uart_tx_stream, buffer, len, portMAX_DELAY);
    /* Let IRQ handle transmission start, to not have race conditions */
    NVIC_SetPendingIRQ(UARTE0_UART0_IRQn);
}

int uart_rx(
    char *buffer,
    int max_len)
{
    configASSERT(uart_rx_stream != NULL);
    return xStreamBufferReceive(uart_rx_stream, buffer, max_len, portMAX_DELAY);
}

void UARTE0_UART0_IRQHandler(
    void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    /* Reception */
    if (NRF_UART0->EVENTS_RXDRDY) {
        NRF_UART0->EVENTS_RXDRDY = 0;
        uint8_t rx_byte = (uint8_t) NRF_UART0->RXD;
        xStreamBufferSendFromISR(
            uart_rx_stream,
            &rx_byte,
            1,
            &xHigherPriorityTaskWoken
        );
    }

    /* Transmission */
    if (NRF_UART0->EVENTS_TXDRDY) {
        NRF_UART0->EVENTS_TXDRDY = 0;
        uart_tx_ready = true;
    }
    if (uart_tx_ready) {
        uint8_t tx_byte;
        size_t len = xStreamBufferReceiveFromISR(
            uart_tx_stream,
            &tx_byte,
            1,
            &xHigherPriorityTaskWoken
        );
        if (len > 0) {
            uart_tx_ready = false;
            NRF_UART0->TXD = tx_byte;
        }
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}