 .globl _main
_main:
movl  $3, %eax
push  %eax
movl  $1, %eax
pop  %ecx
subl  %ecx, %eax
push  %eax
movl  $2, %eax
pop  %ecx
imul  %ecx, %eax
push  %eax
movl  $5, %eax
push  %eax
movl  $3, %eax
pop  %ecx
xor  %edx, %edx
idivl  %ecx
push  %eax
movl  $1, %eax
pop  %ecx
addl  %ecx, %eax
pop  %ecx
imul  %ecx, %eax
push  %eax
movl  $2, %eax
push  %eax
movl  $4, %eax
neg  %eax
pop  %ecx
addl  %ecx, %eax
pop  %ecx
imul  %ecx, %eax
ret  
