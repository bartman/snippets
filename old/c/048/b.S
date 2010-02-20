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
