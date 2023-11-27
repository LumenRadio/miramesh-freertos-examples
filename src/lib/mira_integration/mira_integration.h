#ifndef MIRA_INTEGRATION_H
#define MIRA_INTEGRATION_H

/**
 * Startup MiraMesh integration
 *
 * This needs to be called before FreeRTOS scheduler starts.
 *
 * This may depending on config start softdevice and swi_callback_handler. If
 * configured to not be called automatically, they needs to be manually started
 * before calling this function.
 *
 * This function will use FreeRTOS memory allocation, which needs to started
 * before.
 */
void mira_integration_init(void);

#endif