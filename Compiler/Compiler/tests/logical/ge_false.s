 .globl _main
_main:
movl  $2, %eax
push  %eax
movl  $1, %eax
pop  %ecx
cmpl  %ecx, %eax
movl  $0, %eax
setge  %al
ret  
