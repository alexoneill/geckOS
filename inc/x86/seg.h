/**
 * @file inc/x86/seg.h
 * @brief Predefined segment selectors
 *
 * Inspired by CMU's 2016 OS course
 *
 * @author Alex O'Neill <me@aoneill.me>
 */

#ifndef __INC_X86_SEG_H__
#define __INC_X86_SEG_H__

#define SEGSEL_KERNEL_TSS_IDX   1
#define SEGSEL_KERNEL_CS_IDX    2
#define SEGSEL_KERNEL_DS_IDX    3
#define SEGSEL_USER_CS_IDX      4
#define SEGSEL_USER_DS_IDX      5

#define GDT_SEGS                6

#define SEG_TYPE_TSS          0x09
#define SEG_TYPE_DATA         0x03
#define SEG_TYPE_CODE         0x11

/** @brief Task Segment Selector */
#define SEGSEL_TSS         0x08

/** @brief Kernel Code Segment */
#define SEGSEL_KERNEL_CS   0x10

/** @brief Kernel Data Segment */
#define SEGSEL_KERNEL_DS   0x18

/** @brief User Code Segment */
#define SEGSEL_USER_CS     0x23

/** @brief User Data Segment */
#define SEGSEL_USER_DS     0x2b

#endif /* __INC_X86_SEG_H__ */
