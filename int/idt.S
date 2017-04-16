/**
 * @file int/idt.S
 * @brief Interrupt descriptor table setup
 *
 * Inspired by CMU's 2016 OS course
 *
 * @author Alex O'Neill <me@aoneill.me>
 * @bugs No known bugs.
 */

.global lidt
lidt:
  // Swap the order of the arguments passed in
  movl 4(%esp), %eax
  movl 8(%esp), %ecx
  pushl %eax
  pushw %cx

  // Load the IDTR
  lidt (%esp)

  // Return
  addl $6, %esp
  ret
