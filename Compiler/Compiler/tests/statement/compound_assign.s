 .globl _main
_main:
push  %ebp
movl  %esp, %ebp
movl  $4, %eax
push  %eax
movl  $5, %eax
push  %eax
movl  -4(%ebp), %eax
pop  %ecx
imul  %ecx, %eax
movl  %eax, -4(%ebp)
movl  -4(%ebp), %eax
movl  %ebp, %esp
pop  %ebp
ret  
