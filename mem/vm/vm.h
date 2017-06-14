/**
 * @file vm.h
 * @brief
 *
 * @author Alex O'Neill <me@aoneill.me>
 */

#ifndef __VM_H__
#define __VM_H__

#include <assert.h>
#include <string/string.h>
#include <stddef.h>
#include <stdint.h>
#include <x86/page.h>
#include <util.h>
#include <kern.h>
#include <mem/malloc/malloc.h>

#define PAGE_DIR_BITS_OFFSET 22
#define PAGE_TBL_BITS_OFFSET 12
#define PAGE_FLAGS_BITS 12

#define PAGE_PRESENT (1 << 0)
#define PAGE_RW      (1 << 1)
#define PAGE_GLOBAL  (1 << 8)

#define PAGE_DIR_OFFSET(addr) \
  (((uint32_t) (addr)) >> PAGE_DIR_BITS_OFFSET)

#define PAGE_TBL_OFFSET(addr) \
  (((uint32_t) (addr)) >> PAGE_TBL_BITS_OFFSET)

#define PAGE_ENT_OFFSET(addr) \
  (((uint32_t) (addr)) & BIT_SET_LO(PAGE_TBL_BITS_OFFSET))

typedef void *pd_t;
typedef void *pd_ent_t;

typedef void *pt_t;
typedef void *pt_ent_t;

typedef uint32_t pflags_t;

#define PAGE_ENTRY(base, offset) \
  ((base) + (offset))

#define PAGE_FLAGS(ent) \
  ((*((pflags_t *) (ent))) & BIT_SET_LO(PAGE_FLAGS_BITS))

#define PAGE_BASE(ent) \
    ((*((uint32_t *) (ent))) & (~(BIT_SET_LO(PAGE_FLAGS_BITS))))

#define PAGE_SET_BASE(ent, base) \
  (*((uint32_t *) ent)) \
    = (((*((uint32_t *) ent)) & (BIT_SET_LO(PAGE_FLAGS_BITS))) \
        | ((uint32_t) base))

typedef void *pm_t;


void *pm_alloc(pm_t *pm);
void pm_free(pm_t *pm, void *addr);
void vm_init(pd_t *page_dir, pm_t *pm);
void _vm_get_ent_ptrs(pd_t *page_dir, void *virt_addr, pd_ent_t *dir_ent, pt_ent_t *tbl_ent);
int vm_get_flags(pd_t *page_dir, void *virt_addr, pflags_t *dir_flags, pflags_t *tbl_flags);
void vm_set_flags(pd_t *page_dir, void *virt_addr, pflags_t *dir_flags, pflags_t *tbl_flags);
int vm_map(pd_t *page_dir, void *virt_addr, void *phys_addr);
int vm_alloc(pd_t *page_dir, pm_t *pm, void *virt_addr);
void vm_free(pd_t *page_dir, pm_t *pm, void *virt_addr);
void vm_print(pd_t *page_dir);

#endif /* __VM_H__ */
