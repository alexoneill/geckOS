# Makefile
# aoneill - 02/27/17

################################################################################
# Build utilities
################################################################################
CCX := /usr/bin/gcc
LDX := /usr/bin/ld
OBJ := /usr/bin/objcopy
ARX := /usr/bin/ar
SHL := /usr/bin/bash

################################################################################
# Configuration
################################################################################

# Makefile includes
MK_FILE    := $(abspath $(lastword $(MAKEFILE_LIST)))
MK_ROOT    := $(patsubst %/,%,$(dir $(MK_FILE)))
MK_SUPPORT := $(MK_ROOT)/mk
MK_GENERIC := $(MK_SUPPORT)/generic.mk
MK_CONFIG  := $(MK_SUPPORT)/config.mk
MK_KERNEL  := $(MK_SUPPORT)/kernel.mk

include $(MK_CONFIG)
export

################################################################################
# Build rules
################################################################################

# Directory annotations
BUILD_DIRS := $(LIBS:%=BUILD_%) $(DIRS:%=BUILD_%)
CLEAN_DIRS := $(LIBS:%=CLEAN_%) $(DIRS:%=CLEAN_%)

# All rules
all: $(BUILD_DIRS) $(BLD_DIR)/$(IMG)

# Build
.PHONY: $(BUILD_DIRS)
$(BUILD_DIRS):
	$(MAKE) -C $(@:BUILD_%=%)

# Kernel image
include $(MK_KERNEL)

# Clean
.PHONY: clean $(CLEAN_DIRS)
clean: $(CLEAN_DIRS)
	rm -rf $(BLD_DIR)

$(CLEAN_DIRS):
	$(MAKE) -C $(@:CLEAN_%=%) clean

################################################################################
# Utils
################################################################################

# Print out an arbitrary Makefile variable with `make print-VARIABLE`.
# Sourced from http://stackoverflow.com/a/25817631
print-%  : ; @echo $* = $($*)
