/**
 * @file sys/coms.h
 * @brief Function definitions for device communications
 *
 * Inspired by CMU's 2016 OS course
 *
 * @author Alex O'Neill <me@aoneill.me>
 */

#ifndef __SYS_COMS_H__
#define __SYS_COMS_H__

#include <stdint.h>

/** @brief Reads 1 byte from given port */
uint8_t inb(uint16_t port);
/** @brief Reads 2 bytes from given port */
uint16_t inw(uint16_t port);
/** @brief Reads 4 bytes from given port */
uint32_t ind(uint16_t port);

/** @brief Writes 1 byte to target port */
void outb(uint16_t port, uint8_t val);
/** @brief Writes 2 bytes to target port */
void outw(uint16_t port, uint16_t val);
/** @brief Writes 4 bytes to target port */
void outd(uint16_t port, uint32_t val);

#endif /* __SYS_COMS_H__ */
