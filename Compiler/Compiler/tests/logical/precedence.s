 .globl _main
_main:
movl  $2, %eax
push  %eax
movl  $0, %eax
pop  %ecx
cmpl  $0, %ecx
setne  %cl
cmpl  $0, %eax
movl  $0, %eax
setne  %al
andb  %cl, %al
push  %eax
movl  $1, %eax
pop  %ecx
orl  %ecx, %eax
movl  $0, %eax
setne  %al
ret  
