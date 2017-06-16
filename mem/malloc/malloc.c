/**
 * @file mem/malloc.c
 * @brief Thread-safe memory allocation
 *
 * @author Alex O'Neill <me@aoneill.me>
 * @bugs No known bugs.
 */

#include "malloc.h"
#include <qemu/qemu.h>

void *malloc(size_t size) {
  lprintf("malloc(0x%x) => ", size);
  void *out = _malloc(size);
  lprintf("%p\n", out);
  return out;
}

// Coming soon to a theater near you...
// void *_mustmalloc(size_t size);
// void *_memalign(size_t alignment, size_t size);
// void *_calloc(size_t nelt, size_t eltsize);
// void *_mustcalloc(size_t nelt, size_t eltsize);
// void *_realloc(void *buf, size_t new_size);
// void _free(void *buf);
//
// // Memory allocation where the caller keeps track of size
// void *_smalloc(size_t size);
// void _sfree(void *buf, size_t size);

void *smemalign(size_t alignment, size_t size) {
  lprintf("smemalign(0x%x, 0x%x) => ", alignment, size);
  void *out = _smemalign(alignment, size);
  lprintf("%p\n", out);
  return out;
}
