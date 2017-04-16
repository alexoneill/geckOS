/**
 * @file kern/kernel.h
 * @brief Kernel entry definitions
 *
 * @author Alex O'Neill <me@aoneill.me>
 */

#ifndef __KERN_KERNEL_H__
#define __KERN_KERNEL_H__

#include <kern.h>
#include <boot/multiboot.h>
#include <lib/qemu/qemu.h>
#include <stdio.h>

/**
 * @brief Kernel functions.
 *
 * @see kern/kernel.c
 */
int kernel_main(mbinfo_t *mbinfo, int argc, char **argv, char **envp);

#endif /* __KERN_KERNEL_H__ */

