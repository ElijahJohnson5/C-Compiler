 .globl _main
_main:
movl  $12,%eax
cmpl  $0, %eax
movl  $0, %eax
sete  %al
ret
