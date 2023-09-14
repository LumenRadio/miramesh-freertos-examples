#include "mira_integration.h"
#include "mira_integration_hardware.h"
#include "mira_integration_memory.h"
#include "mira_integration_task.h"
#include "mira_integration_config.h"

#include <stddef.h>
#include <string.h>

#if MIRAMESH_STARTS_SOFTDEVICE
#include "softdevice_integration.h"
#endif
#if MIRAMESH_STARTS_SWI_CALLBACK_HANDLER
#include "swi_callback_handler.h"
#endif

/* Location of certificate memory area, specified in .ld-script */
extern const uint8_t __CertificateStart[];
extern const uint8_t __CertificateEnd[];

void mira_integration_init(
    void)
{
    miramesh_config_t miramesh_config;

    /* Zero out the entire struct to guarantee Mira version compatibility */
    memset(&miramesh_config, 0, sizeof(miramesh_config_t));

    #if MIRAMESH_STARTS_SOFTDEVICE
    softdevice_integration_init();
    #endif
    #if MIRAMESH_STARTS_SWI_CALLBACK_HANDLER
    swi_callback_handler_init();
    #endif

    mira_integration_hardware_init(&miramesh_config.hardware);
    mira_integration_task_init(&miramesh_config.callback);
    miramesh_config.certificate.start = __CertificateStart;
    miramesh_config.certificate.end = __CertificateEnd;

    miramesh_init(&miramesh_config, mira_integration_frontend);

    /*
     * miramesh_init resets allocator function, but doesn't allocate anything.
     * Start memory afterwards.
     */
    mira_integration_memory_init();
}
