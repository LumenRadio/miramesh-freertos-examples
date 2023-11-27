#include "log.h"
#include <stdbool.h>

static log_line_t newlib_stdout_line = LOG_LINE_INIT;

/**
 * There are libraries, including Mira in rare cases, that uses printf. Wrap
 * those printouts to log.h log_line, by keeping a running buffer
 */
int _write(int handle, char* data, int count)
{
    int i;
    for (i = 0; i < count; i++) {
        if (data[i] == '\n' || newlib_stdout_line.len == LOG_MAX_LINE_LEN) {
            newlib_stdout_line.buffer[newlib_stdout_line.len] = '\0';
            log_line_end(&newlib_stdout_line);
            log_line_start(&newlib_stdout_line);
        }

        if (data[i] != '\n') {
            newlib_stdout_line.buffer[newlib_stdout_line.len] = data[i];
            newlib_stdout_line.len++;
        }
    }
    newlib_stdout_line.buffer[newlib_stdout_line.len] = '\0';

    return count;
}
