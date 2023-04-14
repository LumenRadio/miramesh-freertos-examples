ifeq (1,$(includecount))

NRF5_SDK = vendor/nrf5-sdk

ifeq ($(wildcard src/$(NRF5_SDK)),)
$(error nrf5-sdk not found, see README.md for more information)
endif

SRC += \
	$(NRF5_SDK)/components/libraries/experimental_section_vars/nrf_section_iter.c \
	$(NRF5_SDK)/components/libraries/util/app_util_platform.c \
  	$(NRF5_SDK)/components/libraries/strerror/nrf_strerror.c \
  	$(NRF5_SDK)/components/libraries/util/app_error.c \
  	$(NRF5_SDK)/components/libraries/util/app_error_handler_gcc.c \
  	$(NRF5_SDK)/components/libraries/util/app_error_weak.c \
	$(NRF5_SDK)/integration/nrfx/legacy/nrf_drv_clock.c \
	$(NRF5_SDK)/modules/nrfx/drivers/src/nrfx_clock.c \
  	$(NRF5_SDK)/modules/nrfx/drivers/src/nrfx_ppi.c

# Without the following file, the build will seem to build and work. But some
# event handles will be missing for the radio to mira
SRC += \
	$(NRF5_SDK)/components/softdevice/common/nrf_sdh_soc.c

INC += \
	$(NRF5_SDK)/components/libraries/experimental_section_vars \
	$(NRF5_SDK)/components/libraries/log \
	$(NRF5_SDK)/components/libraries/log/src \
	$(NRF5_SDK)/components/libraries/util \
	$(NRF5_SDK)/integration/nrfx \
	$(NRF5_SDK)/integration/nrfx/legacy \
	$(NRF5_SDK)/modules/nrfx \
	$(NRF5_SDK)/modules/nrfx/drivers/include \
  	$(NRF5_SDK)/components/libraries/strerror

LDFLAGS += \
  -Wl,-Lsrc/$(NRF5_SDK)/modules/nrfx/mdk

endif