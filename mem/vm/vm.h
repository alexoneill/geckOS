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

#define PAGE_UTBL_BITS_OFFSET 22
#define PAGE_LTBL_BITS_OFFSET 12
#define PAGE_FLAGS_BITS 12

#define PAGE_DIR_ENTRIES (1 << (WORD_BITS - PAGE_UTBL_BITS_OFFSET))
#define PAGE_TBL_ENTRIES (1 << (PAGE_UTBL_BITS_OFFSET - PAGE_LTBL_BITS_OFFSET))

#define PAGE_PRESENT (1 << 0)
#define PAGE_RW      (1 << 1)
#define PAGE_GLOBAL  (1 << 8)

#define PAGE_DIR_OFFSET(addr) \
  (((uint32_t) (addr)) >> PAGE_UTBL_BITS_OFFSET)

#define PAGE_TBL_OFFSET(addr) \
  ((((uint32_t) (addr)) >> PAGE_LTBL_BITS_OFFSET) \
     & BIT_SET_LO(PAGE_UTBL_BITS_OFFSET - PAGE_LTBL_BITS_OFFSET))

#define PAGE_ENT_OFFSET(addr) \
  (((uint32_t) (addr)) & BIT_SET_LO(PAGE_LTBL_BITS_OFFSET))

typedef void *pt_t;
typedef void *pt_loc_t;
typedef uint32_t pflags_t;

#define PAGE_LOC(base, off) \
  (pt_loc_t) (((uint32_t) (base)) + sizeof(uint32_t) * (off))

#define PAGE_ENTRY(next, flags) \
  ((((uint32_t) (next)) & (~ BIT_SET_LO(PAGE_FLAGS_BITS))) \
      | ((flags) & BIT_SET_LO(PAGE_FLAGS_BITS)))

#define PAGE_SET_ENTRY(loc, ent) \
  *((uint32_t *) (loc)) = (uint32_t) ent

#define PAGE_SET_FLAGS(loc, flags) \
  *((uint32_t *) (loc)) |= (flags) & BIT_SET_LO(PAGE_FLAGS_BITS)

#define PAGE_ENTRY_NEXT(loc) \
  (pt_t) (*((uint32_t *) (loc)) & (~ BIT_SET_LO(PAGE_FLAGS_BITS)))

#define PAGE_ENTRY_FLAGS(loc) \
  (pflags_t) (*((uint32_t *) (loc)) & BIT_SET_LO(PAGE_FLAGS_BITS))

typedef void *pm_t;
void *pm_alloc(pm_t *pm);
void pm_free(pm_t *pm, void *addr);

void vm_init(pt_t *page_dir, pm_t *pm);
void _vm_get_loc_ptrs(pt_t *page_dir, void *virt_addr, pt_loc_t *dir_loc, pt_loc_t *tbl_loc);
int vm_get_flags(pt_t *page_dir, void *virt_addr, pflags_t *dir_flags, pflags_t *tbl_flags);
void vm_set_flags(pt_t *page_dir, void *virt_addr, pflags_t *dir_flags, pflags_t *tbl_flags);
int vm_map(pt_t *page_dir, void *virt_addr, void *phys_addr);
int vm_alloc(pt_t *page_dir, pm_t *pm, void *virt_addr);
void vm_free(pt_t *page_dir, pm_t *pm, void *virt_addr);
void vm_print(pt_t *page_dir);

#endif /* __VM_H__ */
