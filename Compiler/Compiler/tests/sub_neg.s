 .globl _main
_main:
movl  $1, %eax
neg  %eax
push  %eax
movl  $2, %eax
pop  %ecx
subl  %eax, %ecx
movl  %ecx, %eax
ret  
