 .globl _main
_main:
movl  $2, %eax
push  %eax
movl  $4, %eax
pop  %ecx
xor  %edx, %edx
idivl  %ecx
ret  
