 .globl _main
_main:
movl  $5, %eax
push  %eax
movl  $1, %eax
pop  %ecx
cmpl  %ecx, %eax
setl  %al
ret  
