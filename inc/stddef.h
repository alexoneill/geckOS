/**
 * @file inc/stddef.h
 * @brief Standard definitions used in most programs
 *
 * Inspired by CMU's 2016 OS course
 *
 * @author Alex O'Neill <me@aoneill.me>
 *
 * @bugs No known bugs.
 */

#ifndef __INC_STDDEF_H__
#define __INC_STDDEF_H__

#include <types.h>

#ifndef NULL
#define NULL ((void *)0)
#endif

/**
 * @brief Determine the memory offset for a field in a packed struct.
 *
 * @param stype The struct to look into
 * @param field The field of the struct to get the offset for
 *
 * @return The offset (in bytes) of the field from the start of the
 *   struct
 */
#define offsetof(stype,field)	((size_t)(&((stype*)NULL)->field))

#endif /* __INC_STDDEF_H__ */
