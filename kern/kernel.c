/**
 * @file kern/kernel.c
 * @brief Implementation of the kernel main routine.
 *
 * @author Alex O'Neill <me@aoneill.me>
 *
 * @bugs No known bugs.
 */

#include "kernel.h"

#include <mem/vm/vm.h>
#include <proc/regs.h>

// Temporary include to make ld happy...
int putbyte(char c) {
  return 0;
}

void woo() {}

/**
 * @brief muchachOS entry-point.
 *
 * @return This does not return in normal operation.
 */
int kernel_main(mbinfo_t *mbinfo, int argc, char **argv, char **envp) {
  lprintf("kernel_main(...)\n");

  // lprintf("LO(1); 0x%x\n", BIT_SET_LO(1));
  // lprintf("LO(2); 0x%x\n", BIT_SET_LO(2));
  // lprintf("LO(7); 0x%x\n", BIT_SET_LO(7));
  // lprintf("LO(10); 0x%x\n", BIT_SET_LO(10));

  // lprintf("HI(1); 0x%x\n", BIT_SET_HI(1));
  // lprintf("HI(2); 0x%x\n", BIT_SET_HI(2));
  // lprintf("HI(7); 0x%x\n", BIT_SET_HI(7));
  // lprintf("HI(10); 0x%x\n", BIT_SET_HI(10));

  pm_t pm;
  pd_t vm;
  vm_init(&vm, &pm);

  pflags_t k_dir = PAGE_RW & PAGE_PRESENT;
  pflags_t k_tbl = PAGE_GLOBAL & PAGE_RW & PAGE_PRESENT;
  for(uint32_t i = 0; i < 1 * PAGE_SIZE; i += PAGE_SIZE) {
    vm_map(&vm, (void *) i, (void *) i);
    vm_set_flags(&vm, (void *) i, &k_dir, &k_tbl);

    // lprintf("Setup 0x%08x\n", i);
  }

  lbreak();

  vm_print(&vm);

  lbreak();

  uint32_t cr3 = get_cr3();
  cr3 |= ((uint32_t) vm) & ~BIT_SET_LO(PAGE_FLAGS_BITS);
  set_cr3(cr3);

  uint32_t cr0 = get_cr0();
  cr0 |= (1 << 31) | (1 << 0);
  set_cr0(cr0);

  uint32_t cr4 = get_cr4();
  cr4 |= (1 << 7);
  set_cr4(cr4);

  while (1);
}

