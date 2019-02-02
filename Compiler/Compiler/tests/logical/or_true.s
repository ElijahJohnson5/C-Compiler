 .globl _main
_main:
movl  $1, %eax
push  %eax
movl  $0, %eax
pop  %ecx
orl  %ecx, %eax
movl  $0, %eax
setne  %al
ret  
