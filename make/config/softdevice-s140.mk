ifeq (1,$(includecount))

include make/config/lib-nrf5-sdk.mk

SRC += \
  	$(NRF5_SDK)/components/softdevice/common/nrf_sdh.c \
  	$(NRF5_SDK)/components/softdevice/common/nrf_sdh_freertos.c
	
INC += \
	$(NRF5_SDK)/components/softdevice/common \
	$(NRF5_SDK)/components/softdevice/s140/headers \
	$(NRF5_SDK)/components/softdevice/s140/headers/nrf52

CFLAGS += \
	-DSOFTDEVICE_PRESENT

endif