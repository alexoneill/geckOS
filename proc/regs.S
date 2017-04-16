/**
 * @file proc/regs.S
 * @brief Special register set/get methods
 *
 * Inspired by CMU's 2016 OS course
 *
 * @author Alex O'Neill <me@aoneill.me>
 * @bugs No known bugs.
 */
/* Original work Copyright (c) 2014 Carnegie Mellon University */

#include <proc/regs.h>

// Get values from special processor registers

.global get_cr0
get_cr0:
    movl %cr0, %eax
    ret

.global get_cr2
get_cr2:
    movl %cr2, %eax
    ret

.global get_cr3
get_cr3:
    movl %cr3, %eax
    ret

.global get_cr4
get_cr4:
    movl %cr4, %eax
    ret

// Place values into special processor registers

// Changes to cr0 need to be followed by a "far jump"
.global set_cr0
set_cr0:
    movl 4(%esp), %eax   // fetch param
    movl %eax, %cr0      // set %cr0
    pushw %cs            // 16 bits of %cs
    pushl $set_cr0_cont  // 32 bits of %eip
    ljmp *(%esp)         // load %cs:%eip from stack
set_cr0_cont:
    addl $6, %esp        // undo pushes
    ret

.global set_cr2
set_cr2:
    movl 4(%esp), %eax
    movl %eax, %cr2
    ret

.global set_cr3
set_cr3:
    movl 4(%esp), %eax
    movl %eax, %cr3
    ret

.global set_cr4
set_cr4:
    movl 4(%esp), %eax
    movl %eax, %cr4
    ret

// Get the Task Segment address for esp0
tss_addr:
    pushl %ebp
    movl %esp, %ebp

    subl $6, %esp                // allocate 6 bytes for sgdt
    sgdt (%esp)                  // sgdt stores into the address
    movl 2(%esp), %edx           // skip limit bits, fetch base address
    addl $6, %esp                // reclaim stack

    movl $SEGSEL_KERNEL_TSS_IDX,%eax
    movl 4(%edx,%eax,8),%ecx     // high word of TSS segment descriptor
    movl 0(%edx,%eax,8),%eax     // low word of TSS segment descriptor
    shr $16,%eax                 // eax 15:00 is tss-base 15:00

    movl %ecx,%edx               // copy descriptor high word
    shl $16,%edx
    andl $0x00FF0000,%edx
    orl %edx,%eax                // eax 23:00 is tss-base 23:00

    andl $0xFF000000,%ecx        // turn descriptor high word into tss-base 31:24
    orl %ecx,%eax                // eax is tss-base

    leave
    ret

.global get_esp0
get_esp0:
    pushl %ebp
    movl %esp, %ebp
    call tss_addr
    addl $4,%eax                 // esp0 is 4 bytes into tss
    movl (%eax), %eax
    leave
    ret

.global set_esp0
set_esp0:
    pushl %ebp
    movl %esp, %ebp
    call tss_addr
    addl $4,%eax                 // esp0 is 4 bytes into tss
    movl 8(%ebp), %ecx           // fetch new esp0 value from stack
    movl %ecx, (%eax)
    leave
    ret
