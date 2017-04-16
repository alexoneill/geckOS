################################################################################
# Generic build rules
#
# Note: In a Makefile, include this **after** the 'all:' rule
################################################################################

%.o: %.S
	$(CCX) $(CCFLAGS) -DASSEMBLER $(INCLUDE) -c -o $@ $<
	$(OBJ) -R .comment -R .note $@ $@

%.o: %.c
	$(CCX) $(CCFLAGS) $(INCLUDE) -c -o $@ $<
	$(OBJ) -R .comment -R .note $@ $@

%.a:
	rm -f $@
	$(ARX) rc $@ $^

clean:
	rm -rf *.o
