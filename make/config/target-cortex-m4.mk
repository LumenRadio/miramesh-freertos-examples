ifeq (1,$(includecount))

CFLAGS-target-cortex-m4 = \
	-g \
	-O3 \
	-Wdouble-promotion \
	-Wall \
	-Werror \
	-MMD \
	-Wl,--gc-sections,--sort-section=alignment \
	--specs=nosys.specs \
	--specs=nano.specs \
	--short-enums \
	-fno-builtin \
	-fdata-sections \
	-ffunction-sections \
	-fsingle-precision-constant \
	-fno-strict-aliasing \
	-mcpu=cortex-m4 \
	-mfloat-abi=hard \
	-mfpu=fpv4-sp-d16

CFLAGS += $(CFLAGS-target-cortex-m4)
LDFLAGS += $(CFLAGS-target-cortex-m4)

CC = arm-none-eabi-gcc
LD = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

endif