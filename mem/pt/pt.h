/**
 * @file pm.h
 * @brief Definitions and constants for multi-layered page tables
 *
 * This provides the interface for an arbitrarily nested page table
 * implementation. Below is a two layered implementation, with a upper
 * and lower table, encoding 4KB pages
 *
 * @author Alex O'Neill <me@aoneill.me>
 */

#ifndef __PM_H__
#define __PM_H__

#include <assert.h>
#include <kern.h>
#include <mem/malloc/malloc.h>
#include <qemu/qemu.h>
#include <stddef.h>
#include <stdint.h>
#include <string/string.h>
#include <util.h>
#include <x86/page.h>

/** @brief The offset into the memory address for the upper table */
#define PAGE_UTBL_BITS_OFFSET 22
/** @brief The offset into the memory address for the lower table */
#define PAGE_LTBL_BITS_OFFSET 12

/** @brief The number of bits dedicated to flags in the table entry */
#define PAGE_FLAGS_BITS 12
/** @brief The number of upper table entries */
#define PAGE_DIR_ENTRIES (1 << (WORD_BITS - PAGE_UTBL_BITS_OFFSET))
/** @brief The number of lower table entries */
#define PAGE_TBL_ENTRIES (1 << (PAGE_UTBL_BITS_OFFSET - PAGE_LTBL_BITS_OFFSET))

/** @brief The bit representing a page which is present */
#define PAGE_PRESENT (1 << 0)
/** @brief The bit representing a writeable page */
#define PAGE_RW      (1 << 1)
/** @brief The bit representing a page which is global */
#define PAGE_GLOBAL  (1 << 8)

/**
 * @brief Determine the offset into the upper page table given a memory address
 *
 * @param addr The address to extract the information from
 *
 * @return The numerical offset
 */
#define PAGE_DIR_OFFSET(addr) \
  (((uint32_t) (addr)) >> PAGE_UTBL_BITS_OFFSET)

/**
 * @brief Determine the offset into the lower page table given a memory address
 *
 * @param addr The address to extract the information from
 *
 * @return The numerical offset
 */
#define PAGE_TBL_OFFSET(addr) \
  ((((uint32_t) (addr)) >> PAGE_LTBL_BITS_OFFSET) \
     & BIT_SET_LO(PAGE_UTBL_BITS_OFFSET - PAGE_LTBL_BITS_OFFSET))

/** @brief The base page table type */
typedef void *pt_t;
/** @brief The type of a location in a page table */
typedef void *pt_loc_t;
/** @brief The container for flag information */
typedef uint32_t pflags_t;

/**
 * @brief Determine the location of an offset in a given page table
 *
 * @param base The page table
 * @param off The offset into the table
 *
 * @return The page table location
 */
#define PAGE_LOC(base, off) \
  (pt_loc_t) (((uint32_t) (base)) + sizeof(uint32_t) * (off))

/**
 * @brief Construct a page table entry given the base address of a page in the
 *   system and a flag set
 *
 * @param next The base address of the associated page
 * @param off The flag set
 *
 * @return A properly "formatted" page table entry
 */
#define PAGE_ENTRY(next, flags) \
  ((((uint32_t) (next)) & (~ BIT_SET_LO(PAGE_FLAGS_BITS))) \
      | ((flags) & BIT_SET_LO(PAGE_FLAGS_BITS)))

/**
 * @brief Set the entry for a location in the page table
 *
 * @param loc The location to set the entry for
 * @param ent The entry to populate the location with
 */
#define PAGE_SET_ENTRY(loc, ent) \
  *((uint32_t *) (loc)) = (uint32_t) ent

/**
 * @brief Set only the flags for a location in the page table
 *
 * @param loc The location to set the entry for
 * @param ent The flags to set in the entry
 */
#define PAGE_SET_FLAGS(loc, flags) \
  *((uint32_t *) (loc)) &= (~ ((flags) & BIT_SET_LO(PAGE_FLAGS_BITS))); \
  *((uint32_t *) (loc)) |= (flags) & BIT_SET_LO(PAGE_FLAGS_BITS)

/**
 * @brief Get the base for the next page pointed to by the location
 *   in the page table
 *
 * @param loc The location in the page table
 *
 * @return The memory address of the next page in the system
 */
#define PAGE_ENTRY_NEXT(loc) \
  (pt_t) (*((uint32_t *) (loc)) & (~ BIT_SET_LO(PAGE_FLAGS_BITS)))

/**
 * @brief Get the flags contained in the location in the page table
 *
 * @param loc The location in the page table
 *
 * @return The flags associated with the location
 */
#define PAGE_ENTRY_FLAGS(loc) \
  (pflags_t) (*((uint32_t *) (loc)) & BIT_SET_LO(PAGE_FLAGS_BITS))

// TODO: Actual physical memory allocator
typedef void *pm_t;
void *pm_alloc(pm_t *);
void pm_free(pm_t *, void *);

// Function specs
void pt_init(pt_t *);
void _pt_get_loc_ptrs(pt_t *, void *, pt_loc_t *, pt_loc_t *);
int pt_get_flags(pt_t *, void *, pflags_t *, pflags_t *);
void pt_set_flags(pt_t *, void *, pflags_t *, pflags_t *);
void pt_map(pt_t *, void *, void *);
void pt_alloc(pt_t *, pm_t *, void *);
void pt_free(pt_t *, pm_t *, void *);
void pt_print(pt_t *);

#endif /* __PM_H__ */
