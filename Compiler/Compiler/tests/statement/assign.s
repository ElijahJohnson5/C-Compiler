 .globl _main
_main:
push  %ebp
movl  %esp, %ebp
push  %eax
movl  $2, %eax
movl  %eax, -4(%ebp)
movl  -4(%ebp), %eax
movl  %ebp, %esp
pop  %ebp
ret  
