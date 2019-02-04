 .globl _main
_main:
push  %ebp
movl  %esp, %ebp
movl  $2, %eax
push  %eax
movl  $3, %eax
pop  %ecx
addl  %ecx, %eax
push  %eax
movl  $1, %eax
push  %eax
movl  -4(%ebp), %eax
pop  %ecx
addl  %ecx, %eax
push  %eax
movl  -8(%ebp), %eax
movl  %ebp, %esp
pop  %ebp
ret  
