/**
 * @file kern/kernel.c
 * @brief Implementation of the kernel main routine.
 *
 * @author Alex O'Neill <me@aoneill.me>
 *
 * @bugs No known bugs.
 */

#include "kernel.h"

// Temporary include to make ld happy...
int putbyte(char c) {
  return 0;
}

/**
 * @brief muchachOS entry-point.
 *
 * @return This does not return in normal operation.
 */
int kernel_main(mbinfo_t *mbinfo, int argc, char **argv, char **envp) {
  lprintf("kernel_main(...)");
  while (1);
}

