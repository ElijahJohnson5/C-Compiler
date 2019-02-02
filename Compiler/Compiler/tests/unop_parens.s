 .globl _main
_main:
	movl  $1, %eax
	push  %eax
	movl  $1, %eax
	pop  %ecx
	addl  %ecx, %eax
	not  %eax
	ret  
