#include <stdio.h>

#include "nrf_sdh.h"
#include "nrf_nvic.h"

#include "swi_callback_handler.h"

static void (*registered_swi_callbacks[SWI_CALLBACK_LIST_SIZE])(void);
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
    bool success = false;
    for (int i = 0; i < SWI_CALLBACK_LIST_SIZE; ++i) {
        if (callback == registered_swi_callbacks[i]) {
            swi_callbacks_invoked[i] = 1;
            success = true;
            break;
        }
    }
    if (success) {
        sd_nvic_SetPendingIRQ(SWI_CALLBACK_HANDLER_IRQn);
    } else {
        printf("invoke_swi_callback: callback not registered\n");
    }
}

int register_swi_callback(
    swi_callback_t callback)
{
    for (int i = 0; i < SWI_CALLBACK_LIST_SIZE; ++i) {
        if (callback == registered_swi_callbacks[i]) {
            printf("register_swi_callback: callback already registered\n");
            return -1;
        } else if (registered_swi_callbacks[i] == NULL) {
            registered_swi_callbacks[i] = callback;
            return 0;
        }
    }
    printf("register_swi_callback: callback list full\n");
    return -1;
}

#if SWI_CALLBACK_HANDLER_IRQn == SWI3_EGU3_IRQn
void SWI3_EGU3_IRQHandler(
    void)
{
    for (int i = 0; i < SWI_CALLBACK_LIST_SIZE; ++i) {
        if (swi_callbacks_invoked[i]) {
            swi_callbacks_invoked[i] = 0;
            registered_swi_callbacks[i]();
        }
    }
}
#else
#error "SWI_CALLBACK_HANDLER_IRQn has unsupported value"
#endif
