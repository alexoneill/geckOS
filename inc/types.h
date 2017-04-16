/**
 * @file inc/types.h
 * @brief Defines some C standard types as well as
 *   wrapper types for some of the imported FLUX library code.
 *
 * Inspired by CMU's 2016 OS course
 *
 * @author Alex O'Neill <me@aoneill.me>
 */

#ifndef __INC_TYPES_H__
#define __INC_TYPES_H__

typedef unsigned int size_t;
typedef unsigned long ptrdiff_t;

typedef unsigned long vm_offset_t;
typedef unsigned long vm_size_t;

typedef enum {
    FALSE = 0,
    TRUE
} boolean_t;

#endif /* __INC_TYPES_H__ */
