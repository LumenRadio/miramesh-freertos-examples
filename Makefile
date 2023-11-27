# Since includes may make targets, add a proxy default rule first
default: all

include make/once.mk
# Include all buildconfigs to get all apps and test fixtures
include make/targets/*.mk
include make/verbosity.mk

# Filter out all input .elf files and generate hex variant of it
hexapp=$(patsubst %.elf,%.hex,$(filter %.elf,$(1)))

# Some apps may be .elf, should also have a .hex variant
ALLAPPS=$(APPS) $(call hexapp,$(APPS))

all: $(APPS:.elf=.hex)

# For verbosity
APP=""

help: FORCE
	@echo "    Build all apps:        make"
	@echo "    Clean all build files: make clean"

# Both *.elf and *.hex is built when building the elf version as hex
define TARGET_BUILD
$(target) $(call hexapp,$(target)): FORCE
	$(Q)$$(MAKE) $(MAKE_FLAGS) -f make/target.mk $(target:elf=hex) APP=$(target)
endef
$(foreach target,$(APPS),$(eval $(TARGET_BUILD)))

clean: FORCE
	rm -rf build
	rm -rf reports
	rm -rf $(ALLAPPS)

fmt:
	find -name '*.[ch]' -print0 | xargs -0 clang-format -i --style=file

FORCE:
.PHONY: all clean fmt FORCE
