 .globl _main
_main:
movl  $98, %eax
push  %eax
movl  $6, %eax
pop  %ecx
imul  %ecx, %eax
push  %eax
movl  $5, %eax
pop  %ecx
imul  %ecx, %eax
push  %eax
movl  $3, %eax
pop  %ecx
imul  %ecx, %eax
push  %eax
movl  $2, %eax
pop  %ecx
imul  %ecx, %eax
ret  
