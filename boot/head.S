/**
 * @file boot/head.S
 * @brief The very beginning of the kernel
 *
 * Inspired by CMU's 2016 OS course
 *
 * @author Alex O'Neill <me@aoneill.me>
 *
 * @bugs No known bugs.
 */

#include <x86/seg.h>
#include <x86/video_defines.h>

/** @brief Entry point from the bootloader */
.global _start
/** @brief Start of the kernel image */
.global __kimg_start
/** @brief Start of the init GDT */
.global init_gdt
/** @brief Start of the init IDT */
.global init_idt
/** @brief Start of the init TSS */
.global init_tss
.global init_tss_size

.text

__kimg_start:

mb_header:
  // Multiboot magic word
  .long 0x1BADB002

  // Flags field
  .long 0x00000000

  // Checksum
  .long (0 - 0x1BADB002 - 0x00000000)
  .space 244

// Address: 0x100100
init_idt:
  .space 64
  .long 0x00100998
  .long 0x00108e00
  .space 1976

// Address: 0x100900
init_gdt:
  .long 0x00000000
  .long 0x00000000

  // Next two lines need to be kept in sync with smp/smp.c
  .long 0x09300067
  .long 0x00008910

  .long 0x0000ffff
  .long 0x00cf9b00
  .long 0x0000ffff
  .long 0x00cf9300
  .long 0x0000ffff
  .long 0x00cffb00
  .long 0x0000ffff
  .long 0x00cff200

// 0x100930
init_tss:
  .space 8
  .word 0x0018

  // Fills with 0's
  .space 92

  .word 0x0068

init_tss_end:

_start:
  // Disable interrupts
  cli

  // Load initial stack pointer
  leal istack, %esp

  // gdt limit == sizeof(gdt)-1
  movl $init_tss, %eax
  subl $init_gdt, %eax
  subl $1, %eax
  pushl %eax

  // Initialize the GDT
  pushl $init_gdt
  call lgdt
  addl $8, %esp

  // idt limit == sizeof(idt)-1
  movl $init_gdt, %eax
  subl $init_idt, %eax
  subl $1, %eax
  pushl %eax

  // Initialize the IDT
  pushl $init_idt
  call lidt
  addl $8, %esp

  // Load the task register
  movl $SEGSEL_TSS, %eax
  ltr %ax

  // Pass in the multiboot structure
  pushl %ebx

  // Call the C entry point
  call mb_entry

// Not going anywhere
stuck:
  jmp stuck

init_tss_size:
.long (init_tss_end - init_tss)

.data

// Initial stack
.space 4096
istack:
