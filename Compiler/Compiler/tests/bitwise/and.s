 .globl _main
_main:
movl  $7, %eax
push  %eax
movl  $85, %eax
pop  %ecx
and  %ecx, %eax
ret  
