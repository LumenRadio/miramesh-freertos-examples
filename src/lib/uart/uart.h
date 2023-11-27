#ifndef UART_H_
#define UART_H_

/**
 * UART driver, supporting both TX and RX.
 *
 * uses the UART0 peripheral on nrf52840 and nrf52832, together with FreeRTOS
 * stream buffers.
 */

#include <stdint.h>

/**
 * Configuration for UART
 *
 * All parameters according to register values in UART0 peripheral
 */
typedef struct
{
    uint32_t baudrate;     /**< Baudrate according to NRF_UART0->BAUDRATE */
    uint32_t pin_txd;      /**< TXD pin according to NRF_UART0->PSEL.TXD */
    uint32_t pin_rxd;      /**< RXD pin according to NRF_UART0->PSEL.TXD */
    uint32_t irq_priority; /**< Priority of UART interrupt */
} uart_config_t;

/**
 * Start UART module.
 */
void uart_init(const uart_config_t* config);

/**
 * Transmit data to UART
 *
 * This may block
 *
 * @note Must be called from a FreeRTOS task, and must only be called from one
 * task at a time
 */
void uart_tx(const char* buffer, int len);

/**
 * Receive data from UART
 *
 * This will block until data is available, and then return the amount of data
 * received since last receiption.
 *
 * @note Must be called from a FreeRTOS task, and must only be called from one
 * task at a time
 */
int uart_rx(char* buffer, int max_len);

#endif