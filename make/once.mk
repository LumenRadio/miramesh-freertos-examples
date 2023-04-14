# Counts the number of times the current makefile has been included, to make
# sure the file is only run once, even if included multiple times.
#
# Usage:
#
# Put following lines around all content in a given makefile
#
# ifeq (1,$(includecount))
# ... rules that only should be evaluated once ...
# endif
includecount=$(words $(filter $(lastword $(MAKEFILE_LIST)),$(MAKEFILE_LIST)))