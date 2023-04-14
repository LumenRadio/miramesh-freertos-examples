ifeq (1,$(includecount))

include make/config/target-nrf52840.mk
include make/config/lib-libmira-nrf52840.mk
include make/config/lib-tinyprintf.mk

MODULES += \
	lib/freertos_tools \
	lib/mira_integration \
	lib/newlib_integration \
	lib/uart \
	examples_common \
	uart_tx

LDFLAGS += \
	-Tsrc/examples_common/example-nrf52840.ld

APPS += uart_tx-nrf52840.elf

endif