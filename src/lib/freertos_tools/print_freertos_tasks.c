#include "print_freertos_tasks.h"
#include "FreeRTOS.h"
#include "task.h"
#include "log.h"

#define MAX_NUM_TASKS_IN_TRACE 16

/* Only add content to the function if trace facility is active */
#if configUSE_TRACE_FACILITY

static const char *task_state_name[] = {
    [eRunning] = "running",
    [eReady] = "ready",
    [eBlocked] = "blocked",
    [eSuspended] = "suspended",
    [eDeleted] = "deleted",
    [eInvalid] = "invalid"
};

/**
 * Temporary storage for task list.
 *
 * Since it's a big chunk of memory, don't keep it on stack, so the calling
 * process won't get stack overflow.
 */
static TaskStatus_t task_trace_task_list[MAX_NUM_TASKS_IN_TRACE];

void print_freertos_tasks(
    void)
{
    UBaseType_t num_tasks;
    uint32_t runtime;
    UBaseType_t i;
    int state;
    long stack_total_free;
    log_line(" ");
    log_line("ID ................name ....state ...prio free");
    num_tasks = uxTaskGetSystemState(task_trace_task_list,
        MAX_NUM_TASKS_IN_TRACE,
        &runtime);
    stack_total_free = 0;
    for (state = 0; state <= eInvalid; state++) {
        for (i = 0; i < num_tasks; i++) {
            TaskStatus_t *t = &task_trace_task_list[i];
            if (t->eCurrentState == state) {
                log_line("%2lu %-20s %-9s %2lu (%2lu) %4u",
                    t->xTaskNumber,
                    t->pcTaskName,
                    task_state_name[t->eCurrentState],
                    t->uxCurrentPriority,
                    t->uxBasePriority,
                    t->usStackHighWaterMark
                );
                stack_total_free += t->usStackHighWaterMark;
            }
        }
    }
    log_line("Total: unused stack: %ld B", stack_total_free);
}
#else
void print_freertos_tasks(
    void)
{
}
#endif