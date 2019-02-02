 .globl _main
_main:
movl  $0, %eax
push  %eax
movl  $1, %eax
pop  %ecx
cmpl  %ecx, %eax
movl  $0, %eax
setl  %al
ret  
