 .globl _main
_main:
movl  $4, %eax
push  %eax
movl  $3, %eax
pop  %ecx
xor  %edx, %edx
idivl  %ecx
movl  %edx, %eax
ret  
