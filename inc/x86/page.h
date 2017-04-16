/*
 * @file inc/x86/page.h
 * @brief Defines for paging
 *
 * Inspired by CMU's 2016 OS course
 *
 * @author Alex O'Neill <me@aoneill.me>
 */

#ifndef __INC_X86_PAGE_H__
#define __INC_X86_PAGE_H__

/* Pages are 4 KB */
#define PAGE_SHIFT  12
#define PAGE_SIZE   (1 << PAGE_SHIFT)

#endif /* __INC_X86_PAGE_H__ */
