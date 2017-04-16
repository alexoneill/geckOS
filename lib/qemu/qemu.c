/**
 * @file lib/qemu/qemu.c
 * @brief QEMU-related methods.
 *
 * Some inspiration taken from http://wiki.osdev.org/Serial_Ports
 *
 * @author Alex O'Neill <me@aoneill.me>
 * @bugs No known bugs.
 */

#include "qemu.h"

static int com_empty() {
   return inb(QEMU_PORT + 5) & 0x20;
}

void qemu_init(void) {
  outb(QEMU_PORT + 1, 0x00); // Disable all interrupts
  outb(QEMU_PORT + 3, 0x80); // Enable DLAB (set baud rate divisor)
  outb(QEMU_PORT + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
  outb(QEMU_PORT + 1, 0x00); //                  (hi byte)
  outb(QEMU_PORT + 3, 0x03); // 8 bits, no parity, one stop bit
  outb(QEMU_PORT + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
  outb(QEMU_PORT + 4, 0x0B); // IRQs enabled, RTS/DSR set
}

static void write_serial(char a) {
  // Spin while the COM port is not ready
  while (com_empty() == 0);

  // Output our byte
  outb(QEMU_PORT, a);
}

static void lprintf_char(char *arg, int c) {
  write_serial((char) c);
}

int lprintf(const char *fmt, ...) {
	va_list	args;
	va_start(args, fmt);
	_doprnt(fmt, args, 0, (void (*)())lprintf_char, NULL);
	va_end(args);

	return 0;
}
