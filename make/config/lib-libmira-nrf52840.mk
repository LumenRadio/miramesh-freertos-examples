ifeq (1,$(includecount))

include make/config/lib-libmira.mk

LIBS += \
	$(LIBMIRA)/platform/nrf52840ble-net/libmira.a

endif