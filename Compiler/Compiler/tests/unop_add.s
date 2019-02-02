 .globl _main
_main:
movl  $3, %eax
push  %eax
movl  $2, %eax
not  %eax
pop  %ecx
addl  %ecx, %eax
ret  
