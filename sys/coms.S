/**
 * @file sys/coms.S
 * @brief Device communications
 *
 * Inspired by CMU's 2016 OS course
 *
 * @author Alex O'Neill <me@aoneill.me>
 * @bugs No known bugs.
 */

// Communication wrappers for receiving data from a device

.global inb
inb:
	xorl %eax, %eax
	movw 4(%esp), %dx
	in %dx, %al
	ret

.global inw
inw:
	xorl %eax, %eax
	movw 4(%esp), %dx
	in %dx, %ax
	ret

.global ind
ind:
	xorl %eax, %eax
	movw 4(%esp), %dx
	in %dx, %eax
	ret

// Communication wrappers for sending data to a device

.global outb
outb:
	movw 4(%esp), %dx
	movb 8(%esp), %al
	out %al, %dx
	ret

.global outw
outw:
	movw 4(%esp), %dx
	movw 8(%esp), %ax
	out %ax, %dx
	ret

.global outd
outd:
	movw 4(%esp), %dx
	movl 8(%esp), %eax
	out %eax, %dx
	ret
