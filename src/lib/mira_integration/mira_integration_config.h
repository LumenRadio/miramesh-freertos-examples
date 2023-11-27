#ifndef MIRA_INTEGRATION_CONFIG_H
#define MIRA_INTEGRATION_CONFIG_H

#include <nrf.h>
#include "miramesh.h"

/* Configuration for MiraMesh task */
#define MIRAMESH_TASK_NAME "MiraMesh"
#define MIRAMESH_TASK_PRIO 1
#define MIRAMESH_TASK_STACK_SIZE 2048

#define MIRAMESH_RTC_ID 1
#define MIRAMESH_RTC_IRQ_PRIO 6
#define MIRAMESH_SWI_ID 0
#define MIRAMESH_SWI_IRQ_PRIO 5

/* The SWI used for swi_callback_handler library */
#define SWI_CALLBACK_HANDLER_SWI_ID 3
#define SWI_CALLBACK_HANDLER_IRQ_PRIO 5
#define SWI_CALLBACK_LIST_SIZE 2

/* What mira_integration_init() should start automatically */

#define MIRAMESH_STARTS_SOFTDEVICE 1
#define MIRAMESH_STARTS_SWI_CALLBACK_HANDLER 1

/* Frontend configuration is specified in mira_integration_config.c */
extern const mira_net_frontend_config_t* mira_integration_frontend;

#endif
