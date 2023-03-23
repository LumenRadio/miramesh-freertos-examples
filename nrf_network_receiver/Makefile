TARGET ?= nrf52840
MIRA_LIB ?= $(CURDIR)/../../..
SDK_ROOT ?= ../../../vendor/nrf5-sdk

PROJECT_NAME     := miramesh_FreeRTOS
TARGETS          := $(TARGET)_xxaa
OUTPUT_DIRECTORY := obj_$(TARGET)_$(PROJECT_NAME)

PROJ_DIR := .

$(OUTPUT_DIRECTORY)/$(TARGET)_xxaa.out: \
  LINKER_SCRIPT  := config/blinky_FreeRTOS_gcc_$(TARGET).ld

# Target specific settings:
ifeq ($(TARGET),nrf52832)
	SD_HEX ?= $(SDK_ROOT)/components/softdevice/s132/hex/s132_nrf52_7.2.0_softdevice.hex
	SRC_FILES += \
		$(SDK_ROOT)/modules/nrfx/mdk/gcc_startup_nrf52.S \
		$(SDK_ROOT)/modules/nrfx/mdk/system_nrf52.c
	INC_FOLDERS += \
		$(SDK_ROOT)/components/softdevice/s112/headers/nrf52 \
		$(SDK_ROOT)/components/softdevice/s112/headers
	CFLAGS += -DBOARD_PCA10040
	CFLAGS += -DNRF52
	CFLAGS += -DNRF52832_XXAA
	CFLAGS += -DNRF52_PAN_74
	ASMFLAGS += -DBOARD_PCA10040
	ASMFLAGS += -DNRF52
	ASMFLAGS += -DNRF52832_XXAA
	ASMFLAGS += -DNRF52_PAN_74
else
	SD_HEX ?= $(SDK_ROOT)/components/softdevice/s140/hex/s140_nrf52_7.2.0_softdevice.hex
	SRC_FILES += \
		$(SDK_ROOT)/modules/nrfx/mdk/gcc_startup_nrf52840.S \
		$(SDK_ROOT)/modules/nrfx/mdk/system_nrf52840.c
	INC_FOLDERS += \
		$(SDK_ROOT)/components/softdevice/s140/headers/nrf52 \
		$(SDK_ROOT)/components/softdevice/s140/headers
	CFLAGS += -DBOARD_PCA10056
	CFLAGS += -DNRF52840_XXAA
	ASMFLAGS += -DBOARD_PCA10056
	ASMFLAGS += -DNRF52840_XXAA
endif

# Source files common to all targets
SRC_FILES += \
  $(SDK_ROOT)/components/libraries/log/src/nrf_log_frontend.c \
  $(SDK_ROOT)/components/libraries/log/src/nrf_log_str_formatter.c \
  $(SDK_ROOT)/components/boards/boards.c \
  $(SDK_ROOT)/external/freertos/source/croutine.c \
  $(SDK_ROOT)/external/freertos/source/event_groups.c \
  $(SDK_ROOT)/external/freertos/source/portable/MemMang/heap_1.c \
  $(SDK_ROOT)/external/freertos/source/list.c \
  $(SDK_ROOT)/external/freertos/portable/GCC/nrf52/port.c \
  $(SDK_ROOT)/external/freertos/portable/CMSIS/nrf52/port_cmsis.c \
  $(PROJ_DIR)/../nrf_common/sdk_files/port_cmsis_systick.c \
  $(PROJ_DIR)/../nrf_common/sdk_files/app_timer_freertos.c \
  $(SDK_ROOT)/external/freertos/source/queue.c \
  $(SDK_ROOT)/external/freertos/source/stream_buffer.c \
  $(SDK_ROOT)/external/freertos/source/tasks.c \
  $(SDK_ROOT)/external/freertos/source/timers.c \
  $(SDK_ROOT)/components/libraries/button/app_button.c \
  $(SDK_ROOT)/components/libraries/util/app_error.c \
  $(SDK_ROOT)/components/libraries/util/app_error_handler_gcc.c \
  $(SDK_ROOT)/components/libraries/util/app_error_weak.c \
  $(SDK_ROOT)/components/libraries/util/app_util_platform.c \
  $(SDK_ROOT)/components/libraries/util/nrf_assert.c \
  $(SDK_ROOT)/components/libraries/atomic/nrf_atomic.c \
  $(SDK_ROOT)/components/libraries/balloc/nrf_balloc.c \
  $(SDK_ROOT)/external/fprintf/nrf_fprintf.c \
  $(SDK_ROOT)/external/fprintf/nrf_fprintf_format.c \
  $(SDK_ROOT)/components/libraries/memobj/nrf_memobj.c \
  $(SDK_ROOT)/components/libraries/ringbuf/nrf_ringbuf.c \
  $(SDK_ROOT)/components/libraries/strerror/nrf_strerror.c \
  $(SDK_ROOT)/integration/nrfx/legacy/nrf_drv_clock.c \
  $(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_clock.c \
  $(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_gpiote.c \
  $(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_power.c \
  $(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_ppi.c \
  $(SDK_ROOT)/components/libraries/bsp/bsp.c \
  $(SDK_ROOT)/components/softdevice/common/nrf_sdh.c \
  $(SDK_ROOT)/components/softdevice/common/nrf_sdh_freertos.c \
  $(SDK_ROOT)/components/softdevice/common/nrf_sdh_soc.c \
  $(SDK_ROOT)/components/libraries/experimental_section_vars/nrf_section_iter.c \
  $(PROJ_DIR)/main.c \
  $(PROJ_DIR)/network_receiver_task.c \
  $(PROJ_DIR)/../nrf_common/uart.c \
  $(PROJ_DIR)/../nrf_common/miramesh_integration/miramesh_events.c \
  $(PROJ_DIR)/../nrf_common/miramesh_integration/miramesh_task.c \
  $(PROJ_DIR)/../nrf_common/miramesh_integration/swi_callback_handler.c \
  $(SDK_ROOT)/components/libraries/uart/app_uart.c \
  $(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_uart.c \
  $(SDK_ROOT)/integration/nrfx/legacy/nrf_drv_uart.c \

# Include folders common to all targets
INC_FOLDERS += \
  $(MIRA_LIB)/include \
  $(PROJ_DIR)/config \
  $(SDK_ROOT)/components \
  $(SDK_ROOT)/modules/nrfx/mdk \
  $(PROJ_DIR) \
  $(SDK_ROOT)/components/libraries/timer \
  $(SDK_ROOT)/components/libraries/strerror \
  $(SDK_ROOT)/components/toolchain/cmsis/include \
  $(SDK_ROOT)/external/freertos/source/include \
  $(SDK_ROOT)/external/freertos/config \
  $(SDK_ROOT)/components/libraries/util \
  $(SDK_ROOT)/components/libraries/balloc \
  $(SDK_ROOT)/components/libraries/ringbuf \
  $(SDK_ROOT)/modules/nrfx/hal \
  $(SDK_ROOT)/components/libraries/bsp \
  $(SDK_ROOT)/components/libraries/log \
  $(SDK_ROOT)/components/libraries/button \
  $(SDK_ROOT)/modules/nrfx \
  $(SDK_ROOT)/components/libraries/experimental_section_vars \
  $(SDK_ROOT)/integration/nrfx/legacy \
  $(SDK_ROOT)/external/freertos/portable/CMSIS/nrf52 \
  $(SDK_ROOT)/integration/nrfx \
  $(SDK_ROOT)/components/libraries/atomic \
  $(SDK_ROOT)/components/boards \
  $(SDK_ROOT)/components/libraries/memobj \
  $(SDK_ROOT)/external/freertos/portable/GCC/nrf52 \
  $(SDK_ROOT)/modules/nrfx/drivers/include \
  $(SDK_ROOT)/external/fprintf \
  $(SDK_ROOT)/components/libraries/log/src \
  $(SDK_ROOT)/components/softdevice/common \
  $(PROJ_DIR)/../nrf_common/miramesh_integration \
  $(PROJ_DIR)/../nrf_common/ \
  $(SDK_ROOT)/components/libraries/uart \

# Libraries common to all targets
LIB_FILES += \
	$(MIRA_LIB)/platform/$(TARGET)ble-net/libmira.a

# Optimization flags
OPT = -g3
# Uncomment the line below to enable link time optimization
#OPT += -flto

# C flags common to all targets
CFLAGS += $(OPT)
CFLAGS += -DCONFIG_GPIO_AS_PINRESET
CFLAGS += -DSOFTDEVICE_PRESENT
CFLAGS += -DFREERTOS
CFLAGS += -DNRF_SDH_DISPATCH_MODEL=NRF_SDH_DISPATCH_MODEL_POLLING
CFLAGS += -DNRF_DS_BLE_API_VERSION=6
CFLAGS += -mcpu=cortex-m4
CFLAGS += -mthumb -mabi=aapcs
CFLAGS += -Wall -Werror
# keep every function in a separate section, this allows linker to discard unused ones
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fno-builtin -fshort-enums

# C++ flags common to all targets
CXXFLAGS += $(OPT)

# Assembler flags common to all targets
ASMFLAGS += -g3
ASMFLAGS += -mcpu=cortex-m4
ASMFLAGS += -mthumb -mabi=aapcs
ASMFLAGS += -DCONFIG_GPIO_AS_PINRESET
ASMFLAGS += -DFREERTOS

# Linker flags
LDFLAGS += $(OPT)
LDFLAGS += -mthumb -mabi=aapcs -L$(SDK_ROOT)/modules/nrfx/mdk -T$(LINKER_SCRIPT)
LDFLAGS += -mcpu=cortex-m4
# let linker dump unused sections
LDFLAGS += -Wl,--gc-sections
# use newlib in nano version
LDFLAGS += --specs=nano.specs

# Soft or Hard FP ABI?
# Mira 2.4.0 & 2.4.1 uses Soft FP ABI,
# CFLAGS += -mfloat-abi=soft
# ASMFLAGS += -mfloat-abi=soft
# LDFLAGS += -mfloat-abi=soft
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS += -DFLOAT_ABI_HARD
ASMFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
ASMFLAGS += -DFLOAT_ABI_HARD
LDFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16

$(TARGET)_xxaa: CFLAGS += -D__HEAP_SIZE=8192
$(TARGET)_xxaa: CFLAGS += -D__STACK_SIZE=8192
$(TARGET)_xxaa: ASMFLAGS += -D__HEAP_SIZE=8192
$(TARGET)_xxaa: ASMFLAGS += -D__STACK_SIZE=8192

# Add standard libraries at the very end of the linker input, after all objects
# that may need symbols provided by these libraries.
LIB_FILES += -lc -lnosys -lm


.PHONY: default help

# Default target - first one defined
default: $(TARGET)_xxaa

# Print all targets that can be built
help:
	@echo following targets are available:
	@echo		$(TARGET)_xxaa
	@echo		sdk_config    - starting external tool for editing sdk_config.h
	@echo		flash         - flashing binary
	@echo		flash-with-sd - flashing binary with SoftDevice

TEMPLATE_PATH := $(SDK_ROOT)/components/toolchain/gcc


include $(TEMPLATE_PATH)/Makefile.common

$(foreach target, $(TARGETS), $(call define_target, $(target)))

.PHONY: flash erase

# Flash the program
flash: default
	@echo Flashing: $(OUTPUT_DIRECTORY)/$(TARGET)_xxaa.hex
	nrfjprog -f nrf52 --program $(OUTPUT_DIRECTORY)/$(TARGET)_xxaa.hex --sectorerase
	nrfjprog -f nrf52 --reset

flash-with-sd:
	@echo Flashing $(SD_HEX)
	nrfjprog -f nrf52 --program $(SD_HEX) --sectorerase
	$(MAKE) flash

SDK_CONFIG_FILE := ../config/sdk_config.h
CMSIS_CONFIG_TOOL := $(SDK_ROOT)/external_tools/cmsisconfig/CMSIS_Configuration_Wizard.jar
sdk_config:
	java -jar $(CMSIS_CONFIG_TOOL) $(SDK_CONFIG_FILE)
