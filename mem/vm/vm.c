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
  //bzero(out, PAGE_SIZE - 1);

  // lprintf("pm_alloc(...) => 0x%08x\n", pm, out);
  return out;
}

void pm_free(pm_t *pm, void *addr) {
  // lprintf("pm_free(...) => 0x%08x\n", addr);
}

void vm_init(pd_t *page_dir, pm_t *pm) {
  assert(page_dir && pm);

  *page_dir = smemalign(PAGE_SIZE, PAGE_SIZE);
  //bzero(*page_dir, PAGE_SIZE - 1);
}

void _vm_get_ent_ptrs(pd_t *page_dir, void *virt_addr,
    pd_ent_t *dir_ent, pt_ent_t *tbl_ent) {
  assert(page_dir && dir_ent && tbl_ent);

  // lprintf("_vm_get_ent_ptrs(%p, %p, ...)\n", *page_dir, virt_addr);

  uint32_t dir_off = PAGE_DIR_OFFSET(virt_addr);
  *dir_ent = (pd_ent_t) PAGE_ENTRY(*page_dir, dir_off);

  // lprintf("_vm_get_ent_ptrs(...) => dir_off = %d\n", dir_off);
  // lprintf("_vm_get_ent_ptrs(...) => dir_ent = %p\n", *dir_ent);

  pflags_t dir_flags = PAGE_FLAGS(*dir_ent);
  // lprintf("_vm_get_ent_ptrs(...) => dir_flags = 0x%08x\n", dir_flags);

  if(dir_flags & PAGE_PRESENT) {
    uint32_t tbl_off = PAGE_TBL_OFFSET(virt_addr);
    // lprintf("_vm_get_ent_ptrs(...) => tbl_off = %d\n", tbl_off);

    pt_t page_tbl = (pt_t) PAGE_BASE(*dir_ent);
    // lprintf("_vm_get_ent_ptrs(...) => page_tbl = %p\n", page_tbl);

    *tbl_ent = (pt_ent_t) PAGE_ENTRY(page_tbl, tbl_off);
    // lprintf("_vm_get_ent_ptrs(...) => tbl_ent = %p\n", *tbl_ent);
  }
}

int vm_get_flags(pd_t *page_dir, void *virt_addr,
    pflags_t *dir_flags, pflags_t *tbl_flags) {
  assert(page_dir && dir_flags && tbl_flags);

  // lprintf("vm_get_flags(..., %p, ...)\n", virt_addr);

  // Get entires
  pd_ent_t dir_ent = NULL;
  pt_ent_t tbl_ent = NULL;
  _vm_get_ent_ptrs(page_dir, virt_addr, &dir_ent, &tbl_ent);

  *dir_flags = PAGE_FLAGS(dir_ent);
  // lprintf("vm_get_flags(...) => dir_flags = 0x%08x\n", *dir_flags);
  if(tbl_ent) {
    *tbl_flags = PAGE_FLAGS(tbl_ent);
    // lprintf("vm_get_flags(...) => tbl_flags = 0x%08x\n", *tbl_flags);
    return 1;
  }

  return 0;
}

void vm_set_flags(pd_t *page_dir, void *virt_addr,
    pflags_t *dir_flags, pflags_t *tbl_flags) {
  assert(page_dir);

  // lprintf("vm_set_flags(..., %p, %p, %p)\n", virt_addr, dir_flags, tbl_flags);
  // if(dir_flags)
  //   lprintf("vm_set_flags(...) => dir_flags = 0x%08x\n", *dir_flags);
  // if(tbl_flags)
  //   lprintf("vm_set_flags(...) => tbl_flags = 0x%08x\n", *tbl_flags);

  // Get entires
  pd_ent_t dir_ent = NULL;
  pt_ent_t tbl_ent = NULL;
  _vm_get_ent_ptrs(page_dir, virt_addr, &dir_ent, &tbl_ent);

  if(dir_ent && dir_flags) {
    *((pflags_t *) dir_ent) |= *dir_flags;
    // lprintf("vm_set_flags(...) => dir_flags = 0x%08x\n", *dir_flags);
  }

  if(tbl_ent && tbl_flags) {
    *((pflags_t *) tbl_ent) |= *tbl_flags;
    // lprintf("vm_set_flags(...) => tbl_flags = 0x%08x\n", *tbl_flags);
  }
}

int vm_map(pd_t *page_dir, void *virt_addr, void *phys_addr) {
  assert(page_dir);

  // lprintf("vm_alloc(..., %p, %p)\n", virt_addr, phys_addr);

  // Get entires
  pd_ent_t dir_ent = NULL;
  pt_ent_t tbl_ent = NULL;
  _vm_get_ent_ptrs(page_dir, virt_addr, &dir_ent, &tbl_ent);

  if(!tbl_ent) {
    void *page_tbl = smemalign(PAGE_SIZE, PAGE_SIZE);
    //bzero(page_tbl, PAGE_SIZE - 1);
    // lprintf("vm_alloc(...) => page_tbl = %p\n", page_tbl);

    PAGE_SET_BASE(dir_ent, page_tbl);

    // Mark as present
    *((pflags_t *) dir_ent) |= PAGE_PRESENT;

    uint32_t tbl_off = PAGE_TBL_OFFSET(virt_addr);
    // lprintf("vm_alloc(...) => tbl_off = 0x%0x\n", tbl_off);

    tbl_ent = (pt_ent_t) PAGE_ENTRY(page_tbl, tbl_off);
    // lprintf("vm_alloc(...) => tbl_ent = %p\n", tbl_ent);
  }

  pflags_t tbl_flags = PAGE_FLAGS(tbl_ent);
  if(!(tbl_flags & PAGE_PRESENT)) {
    PAGE_SET_BASE(tbl_ent, phys_addr);
    // lprintf("vm_alloc(...) => tbl_ent = %p\n", tbl_ent);

    // Mark as present
    *((pflags_t *) tbl_ent) |= PAGE_PRESENT;
  }

  return 1;
}

int vm_alloc(pd_t *page_dir, pm_t *pm, void *virt_addr) {
  assert(page_dir && pm);

  // lprintf("vm_alloc(..., %p)\n", virt_addr);
  return vm_map(page_dir, virt_addr, pm_alloc(pm));
}

void vm_free(pd_t *page_dir, pm_t *pm, void *virt_addr) {
  assert(page_dir && pm);

  // Get entires
  pt_ent_t tbl_ent = NULL;
  _vm_get_ent_ptrs(page_dir, virt_addr, NULL, &tbl_ent);

  // Free it
  void *base = (void *) PAGE_BASE(tbl_ent);
  pm_free(pm, base);

  // Mark as empty
  *((pflags_t *) tbl_ent) &= ~PAGE_PRESENT;
}

void vm_print(pd_t *page_dir) {
  assert(page_dir);
  lprintf("vm :: dir: %p\n", *page_dir);

  for(int dir_ent = 0; dir_ent < PAGE_DIR_ENTRIES; dir_ent++) {
    pd_ent_t d_ent = (pd_ent_t) PAGE_ENTRY(*page_dir, dir_ent);
    pflags_t d_flags = PAGE_FLAGS(d_ent);

    if(d_flags & PAGE_PRESENT) {
      pt_t page_tbl = (pt_t) PAGE_BASE(d_ent);
      lprintf("  - tbl: %p (#%d, %p) ", page_tbl, dir_ent, d_ent);
      if(d_flags & PAGE_GLOBAL) lprintf("G");
      if(d_flags & PAGE_RW) lprintf("R");
      lprintf("P\n");

      for(int tbl_ent = 0; tbl_ent < PAGE_TBL_ENTRIES; tbl_ent++) {
        pt_ent_t t_ent = (pt_ent_t) PAGE_ENTRY(page_tbl, tbl_ent);
        pflags_t t_flags = PAGE_FLAGS(t_ent);

        if(t_flags & PAGE_PRESENT) {
          void* phys_addr = (void *) PAGE_BASE(t_ent);
          lprintf("    > phys: %p (#%d, %p) ", phys_addr, tbl_ent, t_ent);
          if(t_flags & PAGE_GLOBAL) lprintf("G");
          if(t_flags & PAGE_RW) lprintf("R");
          lprintf("P\n");
        }
      }
    }
  }
}

