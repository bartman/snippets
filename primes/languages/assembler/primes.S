.section .data				# section declaration

ln:
	.string	"+-----------------------------+\n"
ln_len = . - ln

hdr:
	.string	"|      Count            Prime |\n"
hdr_len = . - hdr

buf:
	.string	"|                             |\n"
buf_len = . - buf


ARRAY_LEN = 150000
array:
	.space ARRAY_LEN		# the array

.section .text				# section declaration

			# we must export the entry point to the ELF linker or
    .global _start	# loader. They conventionally recognize _start as their
			# entry point. Use ld -e foo to override the default.

        .align 4
_start:

# write our string to stdout

	movl	$ln_len,%edx	# third argument: message length
	movl	$ln,%ecx	# second argument: pointer to message to write
	movl	$1,%ebx		# first argument: file handle (stdout)
	movl	$4,%eax		# system call number (sys_write)
	int	$0x80		# call kernel

	movl	$hdr_len,%edx	# third argument: message length
	movl	$hdr,%ecx	# second argument: pointer to message to write
	movl	$1,%ebx		# first argument: file handle (stdout)
	movl	$4,%eax		# system call number (sys_write)
	int	$0x80		# call kernel

	movl	$ln_len,%edx	# third argument: message length
	movl	$ln,%ecx	# second argument: pointer to message to write
	movl	$1,%ebx		# first argument: file handle (stdout)
	movl	$4,%eax		# system call number (sys_write)
	int	$0x80		# call kernel

# clear array...

	xorl	%eax,%eax
	movl	$array,%edi
	movl	$ARRAY_LEN,%ecx
clear_string:
	movl	%eax,(%edi)
	addl	$4,%edi
	subl	$4,%ecx
	jns	clear_string

# initialize variables...

	movl	$10000,%ebx	# requested number of primes
	
	movl	$2,%eax		# prime := 2
	movl	$1,%ecx		# count := 1
	movl	$ARRAY_LEN,%esi	# will do the proper thing later
	movl	$array,%edi
	
# start main loop...

	call	print		# display the first prime

main_loop:
	
	movl	%eax,%edx	# tmp := prime
	shl	$1,%edx		# tmp := prime * 2
mark_loop:
	cmpl	%edx,%esi	# if ( np_size <= tmp )
	jle	mark_end	# break out of loop
	
	movb	$1,(%edi,%edx)	# np[tmp] := 1
	addl	%eax,%edx	# tmp := tmp + prime
	
	jmp mark_loop		# loop
mark_end:
	
	
find_loop:
	incl	%eax		# prime := prime + 1
	movb	(%edi,%eax),%dl	# tmp = np[prime]
	test	$1,%dl		# if ( tmp )
	jnz	find_loop	# loop
	
	inc	%ecx		# count := count + 1
	call	print		# print it
	
	cmpl	%ecx,%ebx	# if ( request > count )
	jg	main_loop	# loop
	
# print the last line

	movl	$ln_len,%edx	# third argument: message length
	movl	$ln,%ecx	# second argument: pointer to message to write
	movl	$1,%ebx		# first argument: file handle (stdout)
	movl	$4,%eax		# system call number (sys_write)
	int	$0x80		# call kernel

# and exit

	movl	$0,%ebx		# first argument: exit code
	movl	$1,%eax		# system call number (sys_exit)
	int	$0x80		# call kernel

# ---------------------------------------------------------------------------
# take ECX/EAX pair and print to the screen
print:
	pusha
	
	push	%eax		# backup prime for later
	
	movl	%ecx,%eax	# put index into eax for printing
	
	movl	$buf,%edi
	addl	$11,%edi
	
	movl	$10,%ecx

index_print_loop:
	xorl    %edx,%edx
	div	%ecx	   	# eax = eax/ecx, edx=remainder

	add	$48,%edx	# edx = '0'+remainder
	mov	%dl,(%edi)
	decl	%edi
	
	mov	$32,%dl		# XXX - TODO - remove
	mov	%dl,(%edi)

	test	$0xffffffff,%eax
	jnz	index_print_loop

	
	pop	%eax		# restore prime to eax for printing

	movl	$buf,%edi
	addl	$28,%edi

prime_print_loop:
	xorl    %edx,%edx
	div	%ecx	   	# eax = eax/ecx, edx=remainder

	add	$48,%edx	# edx = '0'+remainder
	mov	%dl,(%edi)
	decl	%edi

	mov	$32,%dl		# XXX - TODO - remove
	mov	%dl,(%edi)

	test	$0xffffffff,%eax
	jnz	prime_print_loop
	
	movl	$buf_len,%edx	# third argument: message length
	movl	$buf,%ecx	# second argument: pointer to message to write
	movl	$1,%ebx		# first argument: file handle (stdout)
	movl	$4,%eax		# system call number (sys_write)
	int	$0x80		# call kernel

	popa
	ret

# END
