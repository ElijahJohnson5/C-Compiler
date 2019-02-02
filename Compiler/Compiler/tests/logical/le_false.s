 .globl _main
_main:
movl  $2, %eax
push  %eax
movl  $4, %eax
pop  %ecx
cmpl  %ecx, %eax
movl  $0, %eax
setle  %al
ret  
