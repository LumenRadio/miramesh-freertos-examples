#ifndef UART_H_
#define UART_H_

#include <stdint.h>

typedef struct {
    uint32_t baudrate; /**< Baudrate according to NRF_UART0->BAUDRATE */
    uint32_t pin_txd; /**< TXD pin according to NRF_UART0->PSEL.TXD */
    uint32_t pin_rxd; /**< RXD pin according to NRF_UART0->PSEL.TXD */
    uint32_t irq_priority; /**< Priority of UART interrupt */
} uart_config_t;

/**
 * Start UART module
 */
void uart_init(
    const uart_config_t *config);

/**
 * Transmit data to UART
 *
 * This may block
 */
void uart_tx(
    const char *buffer,
    int len);

/**
 * Receive data from UART
 *
 * This will block until data is available, and then return the amount of data
 * received since last receiption.
 */
int uart_rx(
    char *buffer,
    int max_len);

#endif