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

  pflags_t rw = PAGE_RW;
  for(int i = 0; i < 10; i++) {
    void *addr = (void *) (0x100 + i * PAGE_SIZE);
    vm_alloc(&vm, &pm, addr);
    vm_set_flags(&vm, addr, NULL, &rw);

    lbreak();
  }

  while (1);
}

