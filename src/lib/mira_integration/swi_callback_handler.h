#ifndef SWI_CALLBACK_HANDLER
#define SWI_CALLBACK_HANDLER

/* This module is used to trigger notifications of FreeRTOS tasks from high
 * priority interrupts. Due to FreeRTOS being configured with a max priority of
 * 2 in this example, one needs to ensure that higher priority interrupts do
 * not call interrupt safe FreeRTOS API functions. This module implements a way
 * to register and invoke callback functions that can call interrupt safe
 * FreeRTOS API functions from a lower interrupt priority.
 */

typedef void (*swi_callback_t)(void);

/* Initializes the SWI callback handler.
 * Clears all previously registered callbacks.
 * Sets priority for and enables SWI_CALLBACK_HANDLER_IRQn.
 */
void swi_callback_handler_init(void);

/**
 * Registers a callback to be available for invoking.
 *
 * Triggers an assert if re-registering a callback or list is full
 */
void register_swi_callback(swi_callback_t callback);

/* Invokes a SWI callback to be called at next SWI_CALLBACK_HANDLER_IRQn.
 * A callback must first be registered before being invoked.
 * Sets SWI_CALLBACK_HANDLER_IRQn to pending after a successful invoke.
 */
void invoke_swi_callback(swi_callback_t callback);

#endif