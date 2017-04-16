# kernel.mk
# aoneill - 03/01/17

################################################################################
# Kernel build rules
################################################################################

# Build setup
.PHONY: $(KERN)_SETUP
$(KERN)_SETUP:
	mkdir -p $(BLD_DIR)

# Kernel objects
$(BLD_DIR)/$(KERN): $(KERNEL_HEAD) $(KERNEL_LOBJS) $(KERNEL_UOBJS)
	$(LDX) $(LDFLAGS) -o $(BLD_DIR)/$(KERN) $^

# gzip-ed kernel
$(BLD_DIR)/$(KERN).gz: $(BLD_DIR)/$(KERN)
	$(OBJ) -g $(BLD_DIR)/$(KERN) $(BLD_DIR)/$(KERN).strip
	gzip -c $(BLD_DIR)/$(KERN).strip > $(BLD_DIR)/$(KERN).gz

# Boot image
$(BLD_DIR)/$(IMG): $(KERN)_SETUP $(BLD_DIR)/$(KERN).gz
	sudo $(SHL) $(KERNEL_MKIMG) $(BLD_DIR)/$(KERN).gz $(BLD_DIR)/$(IMG)
	sudo chown $(USER) $(BLD_DIR)/$(IMG)
