/**
 * @file mem/malloc.h
 * @brief Memory allocation definitions
 *
 * @author Alex O'Neill <me@aoneill.me>
 */

#ifndef __MEM_MALLOC_H__
#define __MEM_MALLOC_H__

#include <types.h>
#include <lib/malloc/malloc_internal.h>

void *malloc(size_t size);
void *memalign(size_t alignment, size_t size);
void *calloc(size_t nelt, size_t eltsize);
void *mustcalloc(size_t nelt, size_t eltsize);
void *realloc(void *buf, size_t new_size);
void free(void *buf);
void *smalloc(size_t size);
void *smemalign(size_t alignment, size_t size);
void sfree(void *buf, size_t size);

#endif /* __MEM_MALLOC_H__ */
