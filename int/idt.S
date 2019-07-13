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

# asm() to .S translation contributed by asubrama, F'08
.global idt_base
idt_base:
    subl $6, %esp       /* get 6 bytes for sidt */
    sidt (%esp)         /* sidt loads into the address */
    movl 2(%esp), %eax  /* we ignore the lower 16 bits (limit bits) */
    addl $6, %esp       /* reclaim stack */
    ret                 /* return the higher 32 bits */

.global int_disable
int_disable:
	cli
	ret

.global int_enable
int_enable:
	sti
	ret
