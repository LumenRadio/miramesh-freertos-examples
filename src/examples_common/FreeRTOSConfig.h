#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include "nrf.h"
#include "app_util.h"
#include "app_util_platform.h"

/* See http://www.freertos.org/a00110.html */

#define configUSE_PREEMPTION                             1
#define configUSE_PORT_OPTIMISED_TASK_SELECTION          1
#define configUSE_TICKLESS_IDLE                          1
#define configCPU_CLOCK_HZ                               SystemCoreClock
#define configTICK_RATE_HZ                               1000
#define configMAX_PRIORITIES                             16
#define configMINIMAL_STACK_SIZE                         128
#define configMAX_TASK_NAME_LEN                          16
#define configUSE_16_BIT_TICKS                           0
#define configIDLE_SHOULD_YIELD                          1
#define configUSE_TASK_NOTIFICATIONS                     1
#define configTASK_NOTIFICATION_ARRAY_ENTRIES            3
#define configUSE_MUTEXES                                1
#define configUSE_RECURSIVE_MUTEXES                      1
#define configUSE_COUNTING_SEMAPHORES                    1
#define configUSE_ALTERNATIVE_API                        0 /* Deprecated! */
#define configQUEUE_REGISTRY_SIZE                        10
#define configUSE_QUEUE_SETS                             0
#define configUSE_TIME_SLICING                           1
#define configUSE_NEWLIB_REENTRANT                       1 /* see note below */
#define configENABLE_BACKWARD_COMPATIBILITY              0
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS          5
#define configUSE_MINI_LIST_ITEM                         1
#define configSTACK_DEPTH_TYPE                           uint16_t
#define configMESSAGE_BUFFER_LENGTH_TYPE                 size_t
#define configHEAP_CLEAR_MEMORY_ON_FREE                  1

/*
 * Regarding configUSE_NEWLIB_REENTRANT:
 *
 * See comment in src/lib/newlib_integration/newlib_memory.c
 * This should be avoided
 */

/* Memory allocation related definitions. */
#define configSUPPORT_STATIC_ALLOCATION                  0
#define configSUPPORT_DYNAMIC_ALLOCATION                 1
#define configTOTAL_HEAP_SIZE                            (128 * 1024)
#define configAPPLICATION_ALLOCATED_HEAP                 0
#define configSTACK_ALLOCATION_FROM_SEPARATE_HEAP        0

/* Hook function related definitions. */
#define configUSE_IDLE_HOOK                              0
#define configUSE_TICK_HOOK                              0
#define configCHECK_FOR_STACK_OVERFLOW                   1
#define configUSE_MALLOC_FAILED_HOOK                     0
#define configUSE_DAEMON_TASK_STARTUP_HOOK               0
#define configUSE_SB_COMPLETED_CALLBACK                  0

/* Run time and task stats gathering related definitions. */
#define configGENERATE_RUN_TIME_STATS                    0
#define configUSE_TRACE_FACILITY                         1
#define configUSE_STATS_FORMATTING_FUNCTIONS             0

/* Co-routine related definitions. */
#define configUSE_CO_ROUTINES                            0
#define configMAX_CO_ROUTINE_PRIORITIES                  1

/* Software timer related definitions. */
#define configUSE_TIMERS                                 1
#define configTIMER_TASK_PRIORITY                        3
#define configTIMER_QUEUE_LENGTH                         10
#define configTIMER_TASK_STACK_DEPTH                     configMINIMAL_STACK_SIZE

/* Interrupt nesting behaviour configuration. */
#define configKERNEL_INTERRUPT_PRIORITY \
    (_PRIO_APP_LOWEST << (8 - __NVIC_PRIO_BITS))
#define configMAX_SYSCALL_INTERRUPT_PRIORITY \
    (_PRIO_APP_HIGH << (8 - __NVIC_PRIO_BITS))
#define configMAX_API_CALL_INTERRUPT_PRIORITY \
    (_PRIO_APP_HIGH << (8 - __NVIC_PRIO_BITS))

/* Define to trap errors during development. */
#define configASSERT(_X) \
    do { \
        if (!(_X)) { \
            asm ("bkpt 255"); \
            for (;;) {} \
        } \
    } while(0)

/* FreeRTOS MPU specific definitions. */
#define configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS 0
#define configTOTAL_MPU_REGIONS                          8 /* Default value. */
#define configTEX_S_C_B_FLASH                            0x07UL /* Default value. */
#define configTEX_S_C_B_SRAM                             0x07UL /* Default value. */
#define configENFORCE_SYSTEM_CALLS_FROM_KERNEL_ONLY      1
#define configALLOW_UNPRIVILEGED_CRITICAL_SECTIONS       1
#define configENABLE_ERRATA_837070_WORKAROUND   1

/* ARMv8-M secure side port related definitions. */
#define secureconfigMAX_SECURE_CONTEXTS                  5

/* Optional functions - most linkers will remove unused functions anyway. */
#define INCLUDE_vTaskPrioritySet                         1
#define INCLUDE_uxTaskPriorityGet                        1
#define INCLUDE_vTaskDelete                              0
#define INCLUDE_vTaskSuspend                             1
#define INCLUDE_xResumeFromISR                           1
#define INCLUDE_vTaskDelayUntil                          0
#define INCLUDE_vTaskDelay                               1
#define INCLUDE_xTaskGetSchedulerState                   0
#define INCLUDE_xTaskGetCurrentTaskHandle                1
#define INCLUDE_uxTaskGetStackHighWaterMark              0
#define INCLUDE_uxTaskGetStackHighWaterMark2             0
#define INCLUDE_xTaskGetIdleTaskHandle                   0
#define INCLUDE_eTaskGetState                            0
#define INCLUDE_xEventGroupSetBitFromISR                 0
#define INCLUDE_xTimerPendFunctionCall                   0
#define INCLUDE_xTaskAbortDelay                          0
#define INCLUDE_xTaskGetHandle                           0
#define INCLUDE_xTaskResumeFromISR                       0

/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
standard names - or at least those used in the unmodified vector table. */

/*
 * MiraMesh integration overrieds SysTick_Handler, and calls xPortSysTickHandler
 * itself, to be able to share an RTC with MiraMesh
 */
/* #define configSYSTICK_CLOCK_HZ                           1000 */
/* #define xPortSysTickHandler                              SysTick_Handler */
#define vPortSVCHandler                                  SVC_Handler
#define xPortPendSVHandler                               PendSV_Handler

#endif /* FREERTOS_CONFIG_H */
