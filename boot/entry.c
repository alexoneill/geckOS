/**
 * @file boot/entry.c
 * @brief C entrypoint for the kernel
 *
 * Inspired by CMU's 2016 OS course
 *
 * @author Alex O'Neill <me@aoneill.me>
 *
 * @bugs No known bugs.
 */

#include <assert.h>
#include <boot/boot.h>
#include <boot/multiboot.h>
#include <int/pic.h>
#include <kern.h>
#include <lmm/lmm.h>
#include <malloc/malloc_internal.h>
#include <proc/regs.h>
#include <qemu/qemu.h>
#include <x86/page.h>

/** @brief For FLUX compatibility; see lib/kvmphys.h */
vm_offset_t phys_mem_va = 0;

/** @brief Variable to hold the number of available frames */
static int num_phys_frames = 0;

/**
 * @brief C entry point for the kernel
 *
 * @param info A structure for Multiboot information
 * @param istack A pointer to the initial stack space
 *
 * @return Should not return...
 */
void mb_entry(mbinfo_t *info, void *istack) {
  int argc;
  char **argv;
  char **envp;

  // Make sure mem_upper has been set by boot loader
  assert(info->flags & MULTIBOOT_MEMORY);

  // Calculate the number of available frames
  num_phys_frames = (info->mem_upper + 1024) / (PAGE_SIZE / 1024);
  assert(num_phys_frames > (USER_MEM_START/PAGE_SIZE));

  // Initialize LMM and make the executable's memory area off limits
  boot_lmm(info, &malloc_lmm);

  // LMM: Reserve under 1 WB
  lmm_remove_free(&malloc_lmm, (void *) 0, 0x100000);

  // Reserve between USER_MEM_START and infinity
  lmm_remove_free(&malloc_lmm, (void *) USER_MEM_START, -8 - USER_MEM_START);

  // Get parameters from the command line
  boot_cmdline(info, &argc, &argv, &envp);

  // Disable floating-point unit
  set_cr0(get_cr0() | CR0_EM);

  // Initialize the PIC
  pic_setup();

  // Initialize the QEMU interface
  qemu_init();

  // Run the main method
  kernel_main(info, argc, argv, envp);
}
