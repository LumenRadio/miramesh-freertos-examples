#include "FreeRTOS.h"
#include "task.h"
#include "nrf.h"

void vApplicationStackOverflowHook(
    TaskHandle_t xTask,
    char *pcTaskName)
{
    /* halt if stack overflow, to catch in debugger */
    configASSERT(0);
}

void HardFault_Handler(
    void)
{
    configASSERT(0);
}

void BusFault_Handler(
    void)
{
    configASSERT(0);
}

void MemoryManagement_Handler(
    void)
{
    configASSERT(0);
}

void UsageFault_Handler(
    void)
{
    configASSERT(0);
}

void fault_handlers_init(
    void)
{
    /* Enable MemFault, BusFault and UsageFault handlers */
    /* SCB->SHCSR - MEMFAULTENA | BUSFAULTENA | USGFAULTENA */
    SCB->SHCSR |= 0x00070000;
}