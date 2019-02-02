 .globl _main
_main:
movl  $1, %eax
push  %eax
movl  $2, %eax
pop  %ecx
shl  %al, %ecx
ret  
