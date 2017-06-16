/**
 * @file vm.c
 * @brief
 *
 * @author Alex O'Neill <me@aoneill.me>
 * @bugs No known bugs.
 */

#include "vm.h"

#include <qemu/qemu.h>

uint32_t off = 0;

void *pm_alloc(pm_t *pm) {
  void *out = (void *) (USER_MEM_START + (off++) * PAGE_SIZE);
  bzero(out, PAGE_SIZE);

  // lprintf("pm_alloc(...) => 0x%08x\n", pm, out);
  return out;
}

void pm_free(pm_t *pm, void *addr) {
  // lprintf("pm_free(...) => 0x%08x\n", addr);
}

void vm_init(pt_t *page_tbl, pm_t *pm) {
  assert(page_tbl && pm);

  *page_tbl = smemalign(PAGE_SIZE, PAGE_SIZE);
  bzero(*page_tbl, PAGE_SIZE);
}

void _vm_get_loc_ptrs(pt_t *page_tbl, void *virt_addr,
    pt_loc_t *utbl_loc, pt_loc_t *ltbl_loc) {
  assert(page_tbl && utbl_loc && ltbl_loc);

  // lprintf("_vm_get_loc_ptrs(%p, %p, ...)\n", *page_tbl, virt_addr);

  uint32_t utbl_off = PAGE_DIR_OFFSET(virt_addr);
  *utbl_loc = PAGE_LOC(*page_tbl, utbl_off);

  // lprintf("_vm_get_loc_ptrs(...) => utbl_off = %d\n", utbl_off);
  // lprintf("_vm_get_loc_ptrs(...) => utbl_loc = %p\n", *utbl_loc);

  pflags_t utbl_flags = PAGE_ENTRY_FLAGS(*utbl_loc);
  // lprintf("_vm_get_loc_ptrs(...) => utbl_flags = 0x%08x\n", utbl_flags);

  if(utbl_flags & PAGE_PRESENT) {
    uint32_t tbl_off = PAGE_TBL_OFFSET(virt_addr);
    // lprintf("_vm_get_loc_ptrs(...) => tbl_off = %d\n", tbl_off);

    pt_t lpage_tbl = PAGE_ENTRY_NEXT(*utbl_loc);
    // lprintf("_vm_get_loc_ptrs(...) => lpage_tbl = %p\n", lpage_tbl);

    *ltbl_loc = PAGE_LOC(lpage_tbl, tbl_off);
    // lprintf("_vm_get_loc_ptrs(...) => ltbl_loc = %p\n", *ltbl_loc);
  }
}

int vm_get_flags(pt_t *page_tbl, void *virt_addr,
    pflags_t *utbl_flags, pflags_t *ltbl_flags) {
  assert(page_tbl && utbl_flags && ltbl_flags);

  // lprintf("vm_get_flags(..., %p, ...)\n", virt_addr);

  // Get entires
  pt_loc_t utbl_loc = NULL;
  pt_loc_t ltbl_loc = NULL;
  _vm_get_loc_ptrs(page_tbl, virt_addr, &utbl_loc, &ltbl_loc);

  *utbl_flags = PAGE_ENTRY_FLAGS(utbl_loc);
  // lprintf("vm_get_flags(...) => utbl_flags = 0x%08x\n", *utbl_flags);
  if(ltbl_loc) {
    *ltbl_flags = PAGE_ENTRY_FLAGS(ltbl_loc);
    // lprintf("vm_get_flags(...) => ltbl_flags = 0x%08x\n", *ltbl_flags);
    return 1;
  }

  return 0;
}

void vm_set_flags(pt_t *page_tbl, void *virt_addr,
    pflags_t *utbl_flags, pflags_t *ltbl_flags) {
  assert(page_tbl);

  // lprintf("vm_set_flags(..., %p, %p, %p)\n", virt_addr, utbl_flags, ltbl_flags);
  // if(utbl_flags)
  //   lprintf("vm_set_flags(...) => utbl_flags = 0x%08x\n", *utbl_flags);
  // if(ltbl_flags)
  //   lprintf("vm_set_flags(...) => ltbl_flags = 0x%08x\n", *ltbl_flags);

  // Get entires
  pt_loc_t utbl_loc = NULL;
  pt_loc_t ltbl_loc = NULL;
  _vm_get_loc_ptrs(page_tbl, virt_addr, &utbl_loc, &ltbl_loc);

  if(utbl_loc && utbl_flags) {
    PAGE_SET_FLAGS(utbl_loc, *utbl_flags);

    // lprintf("vm_set_flags(...) => utbl_flags = 0x%08x\n", *utbl_flags);
  }

  if(ltbl_loc && ltbl_flags) {
    PAGE_SET_FLAGS(ltbl_loc, *ltbl_flags);

    // lprintf("vm_set_flags(...) => ltbl_flags = 0x%08x\n", *ltbl_flags);
  }
}

int vm_map(pt_t *page_tbl, void *virt_addr, void *phys_addr) {
  assert(page_tbl);

  // Get entires
  pt_loc_t utbl_loc = NULL;
  pt_loc_t ltbl_loc = NULL;
  _vm_get_loc_ptrs(page_tbl, virt_addr, &utbl_loc, &ltbl_loc);

  if(!ltbl_loc) {
    void *lpage_tbl = smemalign(PAGE_SIZE, PAGE_SIZE);
    bzero(lpage_tbl, PAGE_SIZE);

    lprintf("vm_map(..., %p, %p) (%p, ## %p ##)\n",  virt_addr, phys_addr, *page_tbl, lpage_tbl);
    lprintf("vm_map(...) => lpage_tbl = %p\n", lpage_tbl);

    // Populate and mark as present
    PAGE_SET_ENTRY(utbl_loc, PAGE_ENTRY(lpage_tbl, PAGE_PRESENT));

    uint32_t tbl_off = PAGE_TBL_OFFSET(virt_addr);
    // lprintf("vm_map(...) => tbl_off = 0x%0x\n", tbl_off);

    ltbl_loc = PAGE_LOC(lpage_tbl, tbl_off);
    lprintf("vm_map(...) => ltbl_loc = %p\n", ltbl_loc);
  }

  pflags_t ltbl_flags = PAGE_ENTRY_FLAGS(ltbl_loc);
  if(!(ltbl_flags & PAGE_PRESENT)) {
    // Populate and mark as present
    PAGE_SET_ENTRY(ltbl_loc, PAGE_ENTRY(phys_addr, PAGE_PRESENT));

    // lprintf("vm_map(...) => ltbl_loc = %p\n", ltbl_loc);
  }

  return 1;
}

int vm_alloc(pt_t *page_tbl, pm_t *pm, void *virt_addr) {
  assert(page_tbl && pm);

  // lprintf("vm_alloc(..., %p)\n", virt_addr);
  return vm_map(page_tbl, virt_addr, pm_alloc(pm));
}

void vm_free(pt_t *page_tbl, pm_t *pm, void *virt_addr) {
  assert(page_tbl && pm);

  // Get entires
  pt_loc_t ltbl_loc = NULL;
  _vm_get_loc_ptrs(page_tbl, virt_addr, NULL, &ltbl_loc);

  // Free it
  void *base = (void *) PAGE_ENTRY_NEXT(ltbl_loc);
  pm_free(pm, base);

  // Mark as empty
  PAGE_SET_ENTRY(ltbl_loc, 0);
}

void vm_print(pt_t *page_tbl) {
  assert(page_tbl);
  lprintf("vm :: dir: %p\n", *page_tbl);

  for(int utbl_index = 0; utbl_index < PAGE_DIR_ENTRIES; utbl_index++) {
    pt_loc_t utbl_loc = (pt_loc_t) PAGE_LOC(*page_tbl, utbl_index);
    pflags_t utbl_flags = PAGE_ENTRY_FLAGS(utbl_loc);

    if(utbl_flags & PAGE_PRESENT) {
      pt_t lpage_tbl = PAGE_ENTRY_NEXT(utbl_loc);
      lprintf("  - tbl: %p (#%d, %p) ", lpage_tbl, utbl_index, utbl_loc);
      if(utbl_flags & PAGE_GLOBAL) lprintf("G");
      if(utbl_flags & PAGE_RW) lprintf("R");
      lprintf("P\n");

      for(int ltbl_index = 0; ltbl_index < PAGE_TBL_ENTRIES; ltbl_index++) {
        pt_loc_t ltbl_loc = (pt_loc_t) PAGE_LOC(lpage_tbl, ltbl_index);
        pflags_t ltbl_flags = PAGE_ENTRY_FLAGS(ltbl_loc);

        if(ltbl_flags & PAGE_PRESENT) {
          void* phys_addr = (void *) PAGE_ENTRY_NEXT(ltbl_loc);
          lprintf("    > phys: %p (#%d, %p) ", phys_addr, ltbl_index, ltbl_loc);
          if(ltbl_flags & PAGE_GLOBAL) lprintf("G");
          if(ltbl_flags & PAGE_RW) lprintf("R");
          lprintf("P\n");
        }
      }
    }
  }
}

