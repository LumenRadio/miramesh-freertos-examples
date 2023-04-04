ifeq (1,$(includecount))

LIBMIRA = vendor/libmira

ifeq ($(wildcard src/$(LIBMIRA)),)
$(error libmira not found, see README.md for more information)
endif

INC += \
  $(LIBMIRA)/include

endif