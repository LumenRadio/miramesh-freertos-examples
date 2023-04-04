#include "mira_integration_memory.h"
#include "miramesh.h"
#include "FreeRTOS.h"

#include <errno.h>

/**
 * Application heap, used by heap_1.c
 *
 * Placing it at .heap should place it at the end of allocated RAM, making it
 * possible to grow up to main stack.
 *
 * Note: heap1.c does not have any protection against over-allocation
 */
//uint8_t ucHeap[configTOTAL_HEAP_SIZE] __attribute__ ((section(".heap")));

static void *mira_integration_memory_malloc(
    mira_size_t size,
    void *storage)
{
    (void) storage;

    void *buffer = pvPortMalloc(size);

    /* MiraMesh will fail if allocation fails */
    configASSERT(buffer != NULL);

    return buffer;
}

void mira_integration_memory_init(
    void)
{
    /*
     * Mira uses dynamic memory allocation during startup, same way as FreeRTOS
     * is usually setup. Therefore, instead of providing a memory buffer to
     * Mira for dynamic allocation, as done normally in MiraOS, provide a
     * wrapper for FreeRTOS pvPortMalloc function, so they share the same heap.
     */
    mira_mem_set_alloc_callback(mira_integration_memory_malloc, NULL);
}