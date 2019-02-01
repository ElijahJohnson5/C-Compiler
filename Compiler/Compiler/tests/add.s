 .globl _main
_main:
movl  $10,%eax
push  %eax
movl  $9,%eax
pop  %ecx
addl  %ecx, %eax
push  %eax
movl  $8,%eax
pop  %ecx
addl  %ecx, %eax
push  %eax
movl  $3,%eax
pop  %ecx
addl  %ecx, %eax
push  %eax
movl  $2,%eax
pop  %ecx
addl  %ecx, %eax
push  %eax
movl  $1,%eax
pop  %ecx
addl  %ecx, %eax
ret
