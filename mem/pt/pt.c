/**
 * @file pt.c
 * @brief
 *
 * @author Alex O'Neill <me@aoneill.me>
 * @bugs No known bugs.
 */

#include "pt.h"

#include <qemu/qemu.h>

// TODO: Actual physical memory allocator
uint32_t off = 0;
void *pm_alloc(pm_t *pm) {
  void *out = (void *) (USER_MEM_START + (off++) * PAGE_SIZE);
  bzero(out, PAGE_SIZE);

  return out;
}

void pm_free(pm_t *pm, void *addr) {}

/**
 * @brief Initialize a page table
 *
 * @param page_tbl A pointer to the page table object to initialize
 */
void pt_init(pt_t *page_tbl) {
  assert(page_tbl && pm);

  // Get a chunk of memory
  *page_tbl = smemalign(PAGE_SIZE, PAGE_SIZE);
  assert(*page_tbl);

  // Zero it to make sure entries are empty
  bzero(*page_tbl, PAGE_SIZE);
}

/**
 * @private
 * @brief Given a page table and a virtual address, find the corresponding
 *   upper and lower table locations
 *
 * Note, this may not set `ltbl_loc` if there isn't a lower page table defined
 * for the given virtual address yet. This can be dertermined by checking the
 * entry flags after calling the method
 *
 * @param page_tbl The page table
 * @param virt_addr The virtual address
 * @param utbl_loc A place to put the upper table entry result
 * @param ltbl_loc A place to put the lower table entry result
 */
void _pt_get_loc_ptrs(pt_t *page_tbl, void *virt_addr,
    pt_loc_t *utbl_loc, pt_loc_t *ltbl_loc) {
  assert(page_tbl && utbl_loc && ltbl_loc);

  // Get the location within the upper table
  uint32_t utbl_off = PAGE_DIR_OFFSET(virt_addr);
  *utbl_loc = PAGE_LOC(*page_tbl, utbl_off);

  // Determine if the location in the lower table is valid
  pflags_t utbl_flags = PAGE_ENTRY_FLAGS(*utbl_loc);
  if(utbl_flags & PAGE_PRESENT) {
    uint32_t tbl_off = PAGE_TBL_OFFSET(virt_addr);

    // Get the location in the lower table
    pt_t lpage_tbl = PAGE_ENTRY_NEXT(*utbl_loc);
    *ltbl_loc = PAGE_LOC(lpage_tbl, tbl_off);
  }
}

/**
 * @brief Get the upper and lower page table flags corresponding to a
 *   virtual address
 *
 * If the destination pointer is NULL for `utbl_flags` or `ltbl_flags`,
 * the value will not be reported
 *
 * If the flags for the lower page table are reported successfully
 * (they are well defined and the destination pointer is not NULL), then
 * the function will give a non-zero value as a result
 *
 * @param page_tbl The page table
 * @param virt_addr The virtual address
 * @param utbl_flags A place to put the upper table flags
 * @param ltbl_flags A place to put the lower table flags
 *
 * @return An integer as scribed above
 */
int pt_get_flags(pt_t *page_tbl, void *virt_addr,
    pflags_t *utbl_flags, pflags_t *ltbl_flags) {
  assert(page_tbl && utbl_flags && ltbl_flags);

  // Get the locations
  pt_loc_t utbl_loc = NULL;
  pt_loc_t ltbl_loc = NULL;
  _pt_get_loc_ptrs(page_tbl, virt_addr, &utbl_loc, &ltbl_loc);

  // Set the flag containers if defined
  if(utbl_flags) *utbl_flags = PAGE_ENTRY_FLAGS(utbl_loc);
  if(ltbl_loc && ltbl_flags) {
    *ltbl_flags = PAGE_ENTRY_FLAGS(ltbl_loc);
    return 1;
  }

  return 0;
}

/**
 * @brief Set the upper and lower page table flags corresponding to a
 *   virtual address
 *
 * If the pointer is NULL for `utbl_flags` or `ltbl_flags`, the flags
 * corresponding to that table will not be updated
 *
 * Further, if the lower page table does not exist for the given address,
 * the table will not be created, nor will flags be set
 *
 * @param page_tbl The page table
 * @param virt_addr The virtual address
 * @param utbl_flags The flag values to update with for the upper table
 * @param ltbl_flags The flag values to update with for the lower table
 */
void pt_set_flags(pt_t *page_tbl, void *virt_addr,
    pflags_t *utbl_flags, pflags_t *ltbl_flags) {
  assert(page_tbl);

  // Get locations
  pt_loc_t utbl_loc = NULL;
  pt_loc_t ltbl_loc = NULL;
  _pt_get_loc_ptrs(page_tbl, virt_addr, &utbl_loc, &ltbl_loc);

  // Set if well defined
  if(utbl_loc && utbl_flags) PAGE_SET_FLAGS(utbl_loc, *utbl_flags);
  if(ltbl_loc && ltbl_flags) PAGE_SET_FLAGS(ltbl_loc, *ltbl_flags);
}

/**
 * @brief Map a virtual address to a physical address in the page table
 *
 * @param page_tbl the page table
 * @param virt_addr the virtual address
 * @param phys_addr The physical address to map to
 */
void pt_map(pt_t *page_tbl, void *virt_addr, void *phys_addr) {
  assert(page_tbl);

  // Get locations
  pt_loc_t utbl_loc = NULL;
  pt_loc_t ltbl_loc = NULL;
  _pt_get_loc_ptrs(page_tbl, virt_addr, &utbl_loc, &ltbl_loc);

  // Create a lower table if needed
  if(!ltbl_loc) {
    void *lpage_tbl = smemalign(PAGE_SIZE, PAGE_SIZE);
    bzero(lpage_tbl, PAGE_SIZE);

    // Populate and mark as present
    PAGE_SET_ENTRY(utbl_loc, PAGE_ENTRY(lpage_tbl, PAGE_PRESENT));

    // Update the variable
    uint32_t tbl_off = PAGE_TBL_OFFSET(virt_addr);
    ltbl_loc = PAGE_LOC(lpage_tbl, tbl_off);
  }

  // Populate and mark as present if needed
  pflags_t ltbl_flags = PAGE_ENTRY_FLAGS(ltbl_loc);
  if(!(ltbl_flags & PAGE_PRESENT)) {
    PAGE_SET_ENTRY(ltbl_loc, PAGE_ENTRY(phys_addr, PAGE_PRESENT));
  }
}

/**
 * @brief Convenience wrapper for `pt_map`, where a new physical page is
 *   requested and automatically mapped
 *
 * Note, this makes no assumptions about existing mappings, and so memory
 * leaks are possible if such a mapping already exists!
 *
 * @param page_tbl the page table
 * @param pm A physical memory allocator
 * @param virt_addr the virtual address
 *
 * @return
 */
int pt_alloc(pt_t *page_tbl, pm_t *pm, void *virt_addr) {
  return pt_map(page_tbl, virt_addr, pm_alloc(pm));
}

/**
 * @brief Free a given physical page mapping in the page table
 *
 * @param page_tbl the page table
 * @param pm A physical memory allocator
 * @param virt_addr the virtual address
 */
void pt_free(pt_t *page_tbl, pm_t *pm, void *virt_addr) {
  assert(page_tbl && pm);

  // Get entires
  pt_loc_t ltbl_loc = NULL;
  _pt_get_loc_ptrs(page_tbl, virt_addr, NULL, &ltbl_loc);

  // Free it
  void *base = (void *) PAGE_ENTRY_NEXT(ltbl_loc);
  pm_free(pm, base);

  // Mark as empty
  PAGE_SET_ENTRY(ltbl_loc, 0);
}

/**
 * @brief Print the structure of the given page table
 *
 * @param page_tbl The page table
 */
void pt_print(pt_t *page_tbl) {
  assert(page_tbl);
  lprintf("vm :: dir: %p\n", *page_tbl);

  // Go through upper level entries
  for(int utbl_index = 0; utbl_index < PAGE_DIR_ENTRIES; utbl_index++) {
    pt_loc_t utbl_loc = (pt_loc_t) PAGE_LOC(*page_tbl, utbl_index);
    pflags_t utbl_flags = PAGE_ENTRY_FLAGS(utbl_loc);

    // If the next level is present display it
    if(utbl_flags & PAGE_PRESENT) {
      pt_t lpage_tbl = PAGE_ENTRY_NEXT(utbl_loc);
      lprintf("  - tbl: %p (#%d, %p) ", lpage_tbl, utbl_index, utbl_loc);
      if(utbl_flags & PAGE_GLOBAL) lprintf("G");
      if(utbl_flags & PAGE_RW) lprintf("R");
      lprintf("P\n");

      // Go through lower level entries
      for(int ltbl_index = 0; ltbl_index < PAGE_TBL_ENTRIES; ltbl_index++) {
        pt_loc_t ltbl_loc = (pt_loc_t) PAGE_LOC(lpage_tbl, ltbl_index);
        pflags_t ltbl_flags = PAGE_ENTRY_FLAGS(ltbl_loc);

        // If the mapping is present, display it
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

