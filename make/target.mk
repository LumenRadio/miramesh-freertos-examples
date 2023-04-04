include make/once.mk

OBJ_DIR=build/obj-$(APP)
GEN_DIR=$(OBJ_DIR)

include make/targets/$(APP).mk
include make/verbosity.mk

rp = $(shell realpath --relative-to . $1)

SRC_FILES = $(patsubst %,src/%,$(SRC)) $(foreach mod,$(MODULES),$(wildcard src/$(mod)/*.c))
OBJS = \
	$(patsubst src/%.S,build/obj-$(APP)/%.o, \
	$(patsubst src/%.c,build/obj-$(APP)/%.o, \
		$(SRC_FILES) \
	)) \
	$(addprefix $(OBJ_DIR)/,$(GEN_OBJS))
INC_FLAGS = $(patsubst %,-Isrc/%,$(INC)) $(foreach mod,$(MODULES),-Isrc/$(mod))
LIB_FILES = $(addprefix src/,$(LIBS))

default: $(TARGETDIR)$(APP)

$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(@D)
	$(TRACE) "CC" $<
	$Q$(CC) -c -o $@ $(CFLAGS) $(INC_FLAGS) $<
	
$(OBJ_DIR)/%.o: $(GEN_DIR)/%.c
	@mkdir -p $(@D)
	$(TRACE) "CC" $<
	$Q$(CC) -c -o $@ $(CFLAGS) $(INC_FLAGS) $<

$(OBJ_DIR)/%.o: src/%.S
	@mkdir -p $(@D)
	$(TRACE) "CC" $<
	$Q$(CC) -c -o $@ $(CFLAGS) $(INC_FLAGS) $<

$(OBJ_DIR)/%.o: $(GEN_DIR)/%.c
	@mkdir -p $(@D)
	$(TRACE) "CC" $<
	$Q$(CC) -c -o $@ $(CFLAGS) $(INC_FLAGS) $<

$(TARGETDIR)$(APP): $(OBJS) $(LIB_FILES)
	@mkdir -p $(@D)
	$(TRACE) "LD" $@
	$Q$(LD) -o $@ $(LDFLAGS) $^

%.hex: %.elf
	@mkdir -p $(@D)
	$(TRACE) "OBJCOPY" $@
	$Q$(OBJCOPY) -O ihex $^ $@

# Dependency tracking, together with -MMD
-include $(patsubst %.o,%.d,$(filter %.o,$(OBJS)))

FORCE:

.PHONY: FORCE