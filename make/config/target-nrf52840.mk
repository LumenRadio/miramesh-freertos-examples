ifeq (1,$(includecount))

include make/config/target-cortex-m4.mk
include make/config/lib-nrf5-sdk.mk
include make/config/softdevice-s140.mk

# This build is all about freertos, so include it as target, so we here can map
# correct freertos related files too
include make/config/lib-freertos.mk

MODULES += \
	lib/freertos_portable/nrf52_softdevice

SRC += \
	$(NRF5_SDK)/modules/nrfx/mdk/gcc_startup_nrf52840.S \
	$(NRF5_SDK)/modules/nrfx/mdk/system_nrf52840.c
	
INC += \
	$(NRF5_SDK)/modules/nrfx/mdk \
  	$(NRF5_SDK)/components/toolchain/cmsis/include

CFLAGS += \
	-DNRF52840_XXAA

endif