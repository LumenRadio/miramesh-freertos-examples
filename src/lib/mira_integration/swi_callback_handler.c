#include "nrf_sdh.h"
#include "nrf_nvic.h"

#include "swi_callback_handler.h"

#include "mira_integration_config.h"
#include "FreeRTOS.h"

#if SWI_CALLBACK_HANDLER_SWI_ID == 0
#define SWI_CALLBACK_HANDLER_IRQn SWI0_EGU0_IRQn
#define SWI_CALLBACK_HANDLER_IRQHandler SWI0_EGU0_IRQHandler
#elif SWI_CALLBACK_HANDLER_SWI_ID == 1
#define SWI_CALLBACK_HANDLER_IRQn SWI1_EGU1_IRQn
#define SWI_CALLBACK_HANDLER_IRQHandler SWI1_EGU1_IRQHandler
#elif SWI_CALLBACK_HANDLER_SWI_ID == 2
#define SWI_CALLBACK_HANDLER_IRQn SWI2_EGU2_IRQn
#define SWI_CALLBACK_HANDLER_IRQHandler SWI2_EGU2_IRQHandler
#elif SWI_CALLBACK_HANDLER_SWI_ID == 3
#define SWI_CALLBACK_HANDLER_IRQn SWI3_EGU3_IRQn
#define SWI_CALLBACK_HANDLER_IRQHandler SWI3_EGU3_IRQHandler
#elif SWI_CALLBACK_HANDLER_SWI_ID == 4
#define SWI_CALLBACK_HANDLER_IRQn SWI4_EGU4_IRQn
#define SWI_CALLBACK_HANDLER_IRQHandler SWI4_EGU4_IRQHandler
#elif SWI_CALLBACK_HANDLER_SWI_ID == 5
#define SWI_CALLBACK_HANDLER_IRQn SWI5_EGU5_IRQn
#define SWI_CALLBACK_HANDLER_IRQHandler SWI5_EGU5_IRQHandler
#else
#error unknown EGU/SWI number
#endif

static swi_callback_t registered_swi_callbacks[SWI_CALLBACK_LIST_SIZE];
static volatile int swi_callbacks_invoked[SWI_CALLBACK_LIST_SIZE];

void swi_callback_handler_init(
    void)
{
    for (int i = 0; i < SWI_CALLBACK_LIST_SIZE; ++i) {
        registered_swi_callbacks[i] = NULL;
        swi_callbacks_invoked[i] = 0;
    }
    sd_nvic_SetPriority(SWI_CALLBACK_HANDLER_IRQn, SWI_CALLBACK_HANDLER_IRQ_PRIO);
    sd_nvic_ClearPendingIRQ(SWI_CALLBACK_HANDLER_IRQn);
    sd_nvic_EnableIRQ(SWI_CALLBACK_HANDLER_IRQn);
}

void invoke_swi_callback(
    swi_callback_t callback)
{
    for (int i = 0; i < SWI_CALLBACK_LIST_SIZE; ++i) {
        if (callback == registered_swi_callbacks[i]) {
            swi_callbacks_invoked[i] = 1;
            sd_nvic_SetPendingIRQ(SWI_CALLBACK_HANDLER_IRQn);
            return;
        }
    }
    /* Callback not registered */
    configASSERT(0);
}

void register_swi_callback(
    swi_callback_t callback)
{
    for (int i = 0; i < SWI_CALLBACK_LIST_SIZE; ++i) {
        /* Check if already is registered */
        configASSERT(callback != registered_swi_callbacks[i]);

        /* Register if free slot */
        if (registered_swi_callbacks[i] == NULL) {
            registered_swi_callbacks[i] = callback;
            return;
        }
    }
    /* No slots left, shouldn't happen */
    configASSERT(0);
}

void SWI_CALLBACK_HANDLER_IRQHandler(
    void)
{
    for (int i = 0; i < SWI_CALLBACK_LIST_SIZE; ++i) {
        if (swi_callbacks_invoked[i]) {
            swi_callbacks_invoked[i] = 0;
            registered_swi_callbacks[i]();
        }
    }
}
