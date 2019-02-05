	.file	"complicated_statements.c"
	.def	___main;	.scl	2;	.type	32;	.endef
	.text
	.globl	_main
	.def	_main;	.scl	2;	.type	32;	.endef
_main:
LFB0:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	andl	$-16, %esp
	subl	$16, %esp
	call	___main
	movl	$25, 12(%esp)
	movl	12(%esp), %edx
	movl	%edx, %eax
	sall	$3, %eax
	subl	%edx, %eax
	addl	$3, %eax
	movl	%eax, 8(%esp)
	movl	8(%esp), %eax
	addl	$2, %eax
	movl	%eax, 12(%esp)
	movl	$190, 4(%esp)
	movl	4(%esp), %eax
	imull	12(%esp), %eax
	movl	%eax, %edx
	movl	%edx, %eax
	sall	$3, %eax
	addl	%edx, %eax
	addl	$4, %eax
	movl	%eax, 4(%esp)
	movl	4(%esp), %eax
	addl	%eax, %eax
	movl	%eax, (%esp)
	movl	(%esp), %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE0:
	.ident	"GCC: (i686-posix-dwarf-rev2, Built by MinGW-W64 project) 7.1.0"
