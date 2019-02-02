 .globl _main
_main:
movl  $4, %eax
push  %eax
movl  $3, %eax
pop  %ecx
imul  %ecx, %eax
push  %eax
movl  $2, %eax
pop  %ecx
addl  %ecx, %eax
ret  
