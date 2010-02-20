	.file	"a.c"
	.text
.globl foo
	.type	foo, @function
foo:
.LFB2:
	pushq	%rbp
.LCFI0:
	movq	%rsp, %rbp
.LCFI1:
	movl	%edi, -4(%rbp)
	movl	-4(%rbp), %eax
	addl	$1, %eax
	leave
	ret
.LFE2:
	.size	foo, .-foo
	.section	.rodata
.LC0:
	.string	"%d\n"
	.text
.globl main
	.type	main, @function
main:
.LFB3:
	pushq	%rbp
.LCFI2:
	movq	%rsp, %rbp
.LCFI3:
	subq	$32, %rsp
.LCFI4:
	movl	$0, -8(%rbp)
	jmp	.L4
.L5:
	addl	$1, -4(%rbp)
	addl	$1, -8(%rbp)
.L4:
	cmpl	$0, -8(%rbp)
	jle	.L5
	movl	-4(%rbp), %esi
	movl	$.LC0, %edi
	movl	$0, %eax
	call	printf
	leave
	ret
.LFE3:
	.size	main, .-main
	.section	.eh_frame,"a",@progbits
.Lframe1:
	.long	.LECIE1-.LSCIE1
.LSCIE1:
	.long	0x0
	.byte	0x1
	.string	"zR"
	.uleb128 0x1
	.sleb128 -8
	.byte	0x10
	.uleb128 0x1
	.byte	0x3
	.byte	0xc
	.uleb128 0x7
	.uleb128 0x8
	.byte	0x90
	.uleb128 0x1
	.align 8
.LECIE1:
.LSFDE1:
	.long	.LEFDE1-.LASFDE1
.LASFDE1:
	.long	.LASFDE1-.Lframe1
	.long	.LFB2
	.long	.LFE2-.LFB2
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI0-.LFB2
	.byte	0xe
	.uleb128 0x10
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI1-.LCFI0
	.byte	0xd
	.uleb128 0x6
	.align 8
.LEFDE1:
.LSFDE3:
	.long	.LEFDE3-.LASFDE3
.LASFDE3:
	.long	.LASFDE3-.Lframe1
	.long	.LFB3
	.long	.LFE3-.LFB3
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI2-.LFB3
	.byte	0xe
	.uleb128 0x10
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI3-.LCFI2
	.byte	0xd
	.uleb128 0x6
	.align 8
.LEFDE3:
	.ident	"GCC: (Debian 4.3.2-1.1) 4.3.2"
	.section	.note.GNU-stack,"",@progbits
