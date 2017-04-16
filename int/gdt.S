/**
 * @file int/gdt.S
 * @brief Global descriptor table setup
 *
 * Inspired by CMU's 2016 OS course
 *
 * @author Alex O'Neill <me@aoneill.me>
 * @bugs No known bugs.
 */

#include <x86/seg.h>

.global lgdt
lgdt:
  // Swap the order of the arguments passed in
  movl 4(%esp), %eax
  movl 8(%esp), %ecx
  pushl %eax
  pushw %cx

  // Load the new GDTR...
  lgdt (%esp)

  // Do a longjmp to cause a GDT reload
  ljmp $SEGSEL_KERNEL_CS, $lgdt_reload

lgdt_reload:
  // Load up the data segment selectors
  movl $SEGSEL_KERNEL_DS, %eax
  movw %ax, %ds
  movw %ax, %es
  movw %ax, %fs
  movw %ax, %gs
  movw %ax, %ss

  // Return
  addl $6, %esp
  ret

