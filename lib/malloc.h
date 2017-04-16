/**
 * @file lib/malloc.h
 * @brief General memory allocation methods
 *
 * @author Alex O'Neill <me@aoneill.me>
 */

#include <stdlib.h>
#include <types.h>

#ifndef __LIB_MALLOC_H__
#define __LIB_MALLOC_H__

void *malloc(size_t size);
void *memalign(size_t alignment, size_t size);
void *calloc(size_t nelt, size_t eltsize);
void *realloc(void *buf, size_t new_size);
void free(void *buf);
void *smalloc(size_t size);
void *smemalign(size_t alignment, size_t size);
void sfree(void *buf, size_t size);

#endif /* __LIB_MALLOC_H__ */
