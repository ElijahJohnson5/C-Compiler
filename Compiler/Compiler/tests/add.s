	.file	"add.c"
	.def	___main;	.scl	2;	.type	32;	.endef
	.text
	.globl	_main
	.def	_main;	.scl	2;	.type	32;	.endef
_main:
	pushl	%ebp
	movl	%esp, %ebp
	andl	$-16, %esp
	call	___main
	movl	$4, %eax
	leave
	ret
	.ident	"GCC: (i686-posix-dwarf-rev2, Built by MinGW-W64 project) 7.1.0"
