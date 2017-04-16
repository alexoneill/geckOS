/**
 * @file inc/stdarg.h
 * @brief Barebones definitions required for va_list's
 *
 * Inspired by CMU's 2016 OS course
 *
 * @author Alex O'Neill <me@aoneill.me>
 *
 * @bugs No known bugs.
 */

#ifndef __INC_STDARG_H__
#define __INC_STDARG_H__

/* This is awful, but really these are compiler intrinsics, so we use the
 * GNU compiler intrinsics.
 */

#ifdef __GNUC__
typedef __builtin_va_list va_list;
#define va_start(v,l)   __builtin_va_start(v,l)
#define va_end(v)       __builtin_va_end(v)
#define va_arg(v,l)     __builtin_va_arg(v,l)
#define va_copy(d,s)    __builtin_va_copy(d,s)
#endif

#endif /* __INC_STDARG_H__ */
