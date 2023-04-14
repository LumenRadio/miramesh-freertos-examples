
ifeq ($(V),1)
  TRACE=@true
  Q=
else
  TRACE=@printf " %-28s %-7s %s\n" $(APP)
  Q=@
  MAKE_FLAGS += --silent
endif