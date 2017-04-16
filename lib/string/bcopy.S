/*
 * Mach Operating System
 * Copyright (c) 1992,1991,1990,1989 Carnegie Mellon University
 * All Rights Reserved.
 *
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 *
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS"
 * CONDITION.  CARNEGIE MELLON DISCLAIMS ANY LIABILITY OF ANY KIND FOR
 * ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 *
 * Carnegie Mellon requests users of this software to return to
 *
 *  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
 *  School of Computer Science
 *  Carnegie Mellon University
 *  Pittsburgh PA 15213-3890
 *
 * any improvements or extensions that they make and grant Carnegie Mellon
 * the rights to redistribute these changes.
 */

/*
 * bcopy - like l2rbcopy, but recognizes overlapping ranges and handles
 *           them correctly.
 *	  bcopy(from, to, bytes)
 *		char *from, *to;
 *		int bytes;
 */
.global bcopy
bcopy:
	pushl	%ebp
	movl	%esp,%ebp
	pushl	%edi
	pushl	%esi
	movl	8(%ebp),%esi
	movl	12(%ebp),%edi
bcopy_common:
	movl	16(%ebp),%edx
	leal	(%esi,%edx), %eax	/* from + bytes */
	cmpl	%eax, %edi		/* <= to 	*/
	jae	0f
	leal	(%edi,%edx), %eax	/* to + bytes	*/
	cmpl	%eax, %esi		/* <= from	*/
	jae	0f
	cmpl	%esi, %edi		/* from > to	*/
	jb	0f
	je 	1f

	addl	%edx, %esi; decl %esi
	addl	%edx, %edi; decl %edi
	std
	movl	%edx,%ecx
/* move bytes backwards */
	rep
	movsb
	cld
1:
/* memcpy: return dest pointer */
	movl	8(%ebp),%eax
	popl	%esi
	popl	%edi
	leave
	ret
/* move words forwards */
0:	cld
	movl	%edx,%ecx
	sarl	$2,%ecx
	js	2f
	rep
	movsl
/* move bytes forwards */
	movl	%edx,%ecx
	andl	$3,%ecx
	rep
	movsb
2:
/* memcpy: return dest pointer */
	movl	8(%ebp),%eax
	popl	%esi
	popl	%edi
	leave
	ret

/* memcpy(to, from, count) */

.global memcpy
.global memmove
memcpy:
memmove:
	pushl	%ebp
	movl	%esp,%ebp
	pushl	%edi
	pushl	%esi
	movl	8(%ebp),%edi
	movl	12(%ebp),%esi
	jmp	bcopy_common

