ifeq (1,$(includecount))

FREERTOS = vendor/freertos

SRC += \
	$(FREERTOS)/list.c \
	$(FREERTOS)/queue.c \
	$(FREERTOS)/stream_buffer.c \
	$(FREERTOS)/tasks.c \
	$(FREERTOS)/timers.c \
	$(FREERTOS)/portable/MemMang/heap_1.c

INC += \
	$(FREERTOS)/include
	
CFLAGS += \
	-DFREERTOS

endif