/*
 * Copyright (c) 1995 The University of Utah and
 * the Computer Systems Laboratory at the University of Utah (CSL).
 * All rights reserved.
 *
 * Permission to use, copy, modify and distribute this software is hereby
 * granted provided that (1) source code retains these copyright, permission,
 * and disclaimer notices, and (2) redistributions including binaries
 * reproduce the notices in supporting documentation, and (3) all advertising
 * materials mentioning features or use of this software display the following
 * acknowledgement: ``This product includes software developed by the
 * Computer Systems Laboratory at the University of Utah.''
 *
 * THE UNIVERSITY OF UTAH AND CSL ALLOW FREE USE OF THIS SOFTWARE IN ITS "AS
 * IS" CONDITION.  THE UNIVERSITY OF UTAH AND CSL DISCLAIM ANY LIABILITY OF
 * ANY KIND FOR ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 *
 * CSL requests users of this software to return to csl-dist@cs.utah.edu any
 * improvements that they make and grant CSL redistribution rights.
 */

#ifndef __LIB_MALLOC_MALLOC_INTERNAL_H__
#define __LIB_MALLOC_MALLOC_INTERNAL_H__

#include <types.h>
#include <lmm/lmm.h>

extern lmm_t malloc_lmm;

void *_malloc(size_t size);
void *_mustmalloc(size_t size);
void *_memalign(size_t alignment, size_t size);
void *_calloc(size_t nelt, size_t eltsize);
void *_mustcalloc(size_t nelt, size_t eltsize);
void *_realloc(void *buf, size_t new_size);
void _free(void *buf);

// Memory allocation where the caller keeps track of size
void *_smalloc(size_t size);
void *_smemalign(size_t alignment, size_t size);
void _sfree(void *buf, size_t size);

#endif /* __LIB_MALLOC_MALLOC_INTERNAL_H__ */
