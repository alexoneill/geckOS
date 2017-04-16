/**
 * @file inc/kern.h
 * @brief Multiboot interface for the kernel
 *
 * Inspired by CMU's 2016 OS course
 *
 * @author Alex O'Neill <me@aoneill.me>
 */

#ifndef __INC_KERN_H__
#define __INC_KERN_H__

/** @brief Memory layout in the kernel */
#define USER_MEM_START 0x01000000

/**
 * @brief Give the number of physical frames available on in the environment
 *
 * @return The number of available physical frames
*/
int machine_phys_frames(void);

#include <boot/multiboot.h>

/**
 * @brief User kernel entrypoint
 *
 * @param mbinfo The kernel's multiboot information structure.
 * @param argc   Argument count, as parsed from multiboot.
 * @param argv   Argument vector, as parsed from multiboot.
 * @param envp   Environment, as parsed from multiboot.
 *
 * @return Should not return...
 */
int kernel_main(mbinfo_t *mbinfo, int argc, char **argv, char **envp);


#endif /* __INC_KERN_H__ */
