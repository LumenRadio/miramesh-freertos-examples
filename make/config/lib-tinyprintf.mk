ifeq (1,$(includecount))

SRC += \
	vendor/tinyprintf/tinyprintf.c

INC += \
	vendor/tinyprintf

CFLAGS += \
	-DTINYPRINTF_DEFINE_TFP_PRINTF=0 \
	-DTINYPRINTF_OVERRIDE_LIBC=0 \
	-DTINYPRINTF_DEFINE_TFP_SPRINTF=1

$(OBJ_DIR)/vendor/tinyprintf/tinyprintf.o: CFLAGS += -Wno-pedantic

endif