#ifndef LOG_H_
#define LOG_H_

#include "FreeRTOS.h"
#include "task.h"
#include <stdarg.h>

/* Handle at least 80 chars with margin for control chars */
#define LOG_MAX_LINE_LEN 128

typedef struct
{
    TaskHandle_t task;
    TickType_t time;
    int len;
    char buffer[LOG_MAX_LINE_LEN + 1];
} log_line_t;

/**
 * Init values for log line
 *
 * In case of necessary to start a partial log line without log_start()
 */
#define LOG_LINE_INIT \
    {                 \
        .len = 0      \
    }

/**
 * Start log handling
 *
 * This needs to be started before any other methods in logging is called. Logs
 * before this is called with be silently dropped
 */
void log_init(void);

/**
 * Start writing a line, that is filled in multiple steps
 *
 * For outputting a line in a single call, see log_line()
 */
void log_line_start(log_line_t* line);

/**
 * Add a part to a line previously started with log_line_start()
 */
void log_line_add(log_line_t* line, const char* fmt, ...) __attribute__((format(printf, 2, 3)));

/**
 * Add a part to a line previously started with log_line_start() using stdargs
 */
void log_line_addv(log_line_t* line, const char* fmt, va_list ap);

/**
 * Finish up a segmented line, and send to the log queue
 */
void log_line_end(log_line_t* line);

/**
 * Write a line to the log
 *
 * Note, no linebreaks should be included. Maximum of LOG_MAX_LINE_LEN will be
 * outputted
 *
 * The task name will be prepended to the log. Safe to be called from any task
 */
void log_line(const char* fmt, ...) __attribute__((format(printf, 1, 2)));

/**
 * Write a line to the log using stdargs
 *
 * Same as log_line(), but with va_list
 */
void log_linev(const char* fmt, va_list ap);

/**
 * Write a line to the log from IRQ
 *
 * Same as log_line() but callable from IRQ
 *
 * Note: this can not be called from higher IRQ level than FreeRTOS API
 */
void log_line_isr(const char* fmt, ...) __attribute__((format(printf, 1, 2)));

void log_linev_isr(const char* fmt, va_list ap);

void log_line_end_isr(log_line_t* line);

#endif