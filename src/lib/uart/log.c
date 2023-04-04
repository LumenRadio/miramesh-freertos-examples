#include "log.h"
#include "tinyprintf.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "uart.h"

#include <stdbool.h>

#define LOG_QUEUE_NUM_ENTRIES 16

static QueueHandle_t log_queue;

static void log_task(
    void *storage);

/**
 * For formatting in line callback
 *
 * Output format: DDdHH:MM:SS.TTT
 *
 * where:
 * - DD = day
 * - HH = hour
 * - MM = minute
 * - SS = second
 * - TT = tick within second
 *
 * For 32 bit ticks at 1000 ticks/sec, DD can be up to 49
 *
 * returns dst for convenience
 */
char *log_tick_to_string(
    char *dst,
    TickType_t tick)
{
    uint32_t sec = tick / configTICK_RATE_HZ;
    uint32_t min = sec / 60;
    uint32_t hour = min / 60;
    uint32_t day = hour / 24;
    int pos;

    hour %= 24;
    min %= 60;
    sec %= 60;
    tick %= configTICK_RATE_HZ;

    pos = 0;
    if (day > 0) {
        pos += tfp_sprintf(dst + pos, "%lud", day);
    }
    if (day > 0 || hour > 0) {
        pos += tfp_sprintf(dst + pos, "%lu:", hour);
    }
    if (day > 0 || hour > 0 || min > 0) {
        pos += tfp_sprintf(dst + pos, "%2lu:", min);
    }
    pos += tfp_sprintf(dst + pos, "%2lu.%03lu", sec, tick);
    return dst;
}

/**
 * Low level log output
 * 
 * May only be used from log_task, since uart_tx is not re-entrant
 */
static void log_output(
    TickType_t time,
    const char *task,
    const char *line)
{
    int len;
    /* Handle at least 80 chars with margin for control chars + task name */
    char buffer[10 + LOG_MAX_LINE_LEN];
    char tstamp[20];

    len = tfp_snprintf(
        buffer,
        sizeof(buffer),
        "%12s : %-15s: %s\n",
        log_tick_to_string(tstamp, time), /* 12 char for <24h runtime */
        task,
        line);

    /* Make sure the second to last character is line break if truncated */
    if (len > sizeof(buffer) - 1) {
        len = sizeof(buffer) - 1;
        buffer[len - 1] = '\n';
        buffer[len] = '\0';
    }

    uart_tx(buffer, len);
}

void log_init(
    void)
{
    log_queue = xQueueCreate(LOG_QUEUE_NUM_ENTRIES, sizeof(log_line_t));
#if configQUEUE_REGISTRY_SIZE > 0
    vQueueAddToRegistry(log_queue, "log");
#endif

    xTaskCreate(log_task,
        "sys log",
        192, /* Stack size */
        NULL,
        4, /* Priority */
        NULL);
}

static void log_task(
    void *storage)
{
    log_line_t line;
    for (;;) {
        if (pdTRUE == xQueueReceive(log_queue, &line, portMAX_DELAY)) {
            const char *task_name;
            if (line.task != NULL) {
                task_name = pcTaskGetName(line.task);
            } else {
                task_name = "ISR";
            }
            log_output(line.time, task_name, line.buffer);
        }
    }
}

void log_line_start(
    log_line_t *line)
{
    line->len = 0;
}

void log_line_add(
    log_line_t *line,
    const char *fmt,
    ...)
{
    va_list ap;
    va_start(ap, fmt);
    log_line_addv(line, fmt, ap);
    va_end(ap);
}

void log_line_addv(
    log_line_t *line,
    const char *fmt,
    va_list ap)
{
    int len;

    if (line->len >= LOG_MAX_LINE_LEN) {
        return;
    }

    len = tfp_vsnprintf(
        line->buffer + line->len,
        LOG_MAX_LINE_LEN - line->len + 1,
        fmt, ap);

    if (len > LOG_MAX_LINE_LEN - line->len) {
        len = LOG_MAX_LINE_LEN - line->len;
    }
    line->len += len;
}

void log_line_end(
    log_line_t *line)
{
    if(log_queue == NULL) {
        return;
    }
    line->task = xTaskGetCurrentTaskHandle();
    line->time = xTaskGetTickCount();
    xQueueSendToBack(log_queue, line, portMAX_DELAY);
}

void log_line(
    const char *fmt,
    ...)
{
    va_list ap;
    va_start(ap, fmt);
    log_linev(fmt, ap);
    va_end(ap);
}

void log_linev(
    const char *fmt,
    va_list ap)
{
    log_line_t line;
    log_line_start(&line);
    log_line_addv(&line, fmt, ap);
    log_line_end(&line);
}

void log_line_isr(
    const char *fmt,
    ...)
{
    va_list ap;
    va_start(ap, fmt);
    log_linev_isr(fmt, ap);
    va_end(ap);
}

void log_linev_isr(
    const char *fmt,
    va_list ap)
{
    log_line_t line;
    log_line_start(&line);
    log_line_addv(&line, fmt, ap);
    log_line_end_isr(&line);
}

void log_line_end_isr(
    log_line_t *line)
{
    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
    line->task = NULL;
    line->time = xTaskGetTickCountFromISR();
    xQueueSendToBackFromISR(log_queue, line, &pxHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
}