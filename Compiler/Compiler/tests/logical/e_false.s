 .globl _main
_main:
movl  $2, %eax
push  %eax
movl  $1, %eax
pop  %ecx
cmpl  %eax, %ecx
movl  $0, %eax
sete  %al
ret  
