/**
 * @file boot/boot.h
 * @brief Definition for boot setup helpers
 *
 * Inspired by CMU's 2016 OS course
 *
 * @author Alex O'Neill <me@aoneill.me>
 *
 * @bugs No known bugs.
 */

#ifndef __BOOT_BOOT_H__
#define __BOOT_BOOT_H__

#include <boot/multiboot.h>
#include <lmm/lmm.h>

void boot_cmdline(mbinfo_t *, int *, char ***, char ***);
void boot_lmm(mbinfo_t *, lmm_t *);

#endif /* __BOOT_BOOT_H__ */
