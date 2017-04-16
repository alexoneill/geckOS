/**
 * @file lib/qemu/qemu.h
 * @brief
 *
 * @author Alex O'Neill <me@aoneill.me>
 */

#ifndef __LIB_QEMU_QEMU_H__
#define __LIB_QEMU_QEMU_H__

#include <doprnt.h>
#include <stdarg.h>
#include <stddef.h>

#include <sys/coms.h>

#define QEMU_PORT 0x3f8

void qemu_init(void);
int lprintf(const char *fmt, ...);

#endif /* __LIB_QEMU_QEMU_H__ */
