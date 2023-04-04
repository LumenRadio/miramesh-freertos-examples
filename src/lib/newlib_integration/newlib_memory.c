#include "FreeRTOS.h"
#include <errno.h>

#define NEWLIB_MEMORY_BUFFER_SIZE 4096

static uint8_t *newlib_memory = NULL;
static size_t newlib_memory_ptr;

/**
 * Newlib memory allocation.
 *
 * Newlib needs some memory for some internal functions. Take over that
 * allocation and give it some RAM to work with.
 *
 * The default implementation of _sbrk() from newlib would interfere with
 * FreeRTOS heap implementation, when placing heap after allocated RAM
 *
 * Better is however not to depend on newlib, since those functions are not
 * properly reentrant anyway
 */
void *_sbrk(
    int incr)
{
    if (newlib_memory == NULL) {
        newlib_memory = pvPortMalloc(NEWLIB_MEMORY_BUFFER_SIZE);
        newlib_memory_ptr = 0;
    }

    if (newlib_memory_ptr + incr > NEWLIB_MEMORY_BUFFER_SIZE) {
        errno = ENOMEM;
        return (char *) -1;
    } else {
        newlib_memory_ptr += incr;
        return newlib_memory + newlib_memory_ptr - incr;
    }
}