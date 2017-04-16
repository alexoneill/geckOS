# config.mk
# aoneill - 03/01/17

################################################################################
# Exports
################################################################################

LIB_DIR := lib
BLD_DIR := build

LIBS := $(patsubst %/,%, \
         $(filter-out $(LIB_DIR)/, \
           $(sort $(dir $(wildcard $(LIB_DIR)/*/)))))
DIRS := boot int drv sys mem proc thr kern
KERN := kernel
IMG  := bootfd.img

# Kernel-specific variables
KERNEL_MKIMG := $(MK_SUPPORT)/mkimg/mkimg.sh
KERNEL_HEAD  := boot/head.o
KERNEL_LOBJS := $(addsuffix .o, \
                  $(basename $(shell find $(LIBS) -name "*.[cS]")))
KERNEL_UOBJS := $(addsuffix .o, \
                  $(basename \
                    $(shell find $(DIRS) -name "*.[cS]" \
                      -not -path "$(basename $(KERNEL_HEAD)).[cS]")))

################################################################################
# Flags
################################################################################

CCFLAGS := -nostdinc \
           -fno-strict-aliasing \
           -fno-builtin \
           -fno-stack-protector \
           -fno-omit-frame-pointer \
           -fno-aggressive-loop-optimizations \
           -fno-asynchronous-unwind-tables \
           --std=gnu99 \
           -Wall \
           -Werror \
           -gstabs+ \
           -O0 \
           -m32

LDFLAGS := -static \
           -Ttext 100000 \
           --fatal-warnings \
           -melf_i386

INCLUDE := -I$(MK_ROOT) -I$(MK_ROOT)/inc -I$(MK_ROOT)/lib

################################################################################
# Specific modules
################################################################################

# Libraries
include $(MK_ROOT)/$(LIB_DIR)/config.mk
