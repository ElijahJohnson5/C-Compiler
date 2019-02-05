 .globl _main
_main:
push  %ebp
movl  %esp, %ebp
movl  $5, %eax
neg  %eax
push  %eax
movl  $6, %eax
push  %eax
movl  $5, %eax
pop  %ecx
imul  %ecx, %eax
pop  %ecx
addl  %ecx, %eax
push  %eax
movl  $3, %eax
push  %eax
movl  $9, %eax
pop  %ecx
xor  %edx, %edx
idivl  %ecx
push  %eax
movl  $7, %eax
push  %eax
movl  -4(%ebp), %eax
pop  %ecx
imul  %ecx, %eax
pop  %ecx
addl  %ecx, %eax
push  %eax
movl  $2, %eax
push  %eax
movl  -8(%ebp), %eax
pop  %ecx
addl  %ecx, %eax
movl  %eax, -4(%ebp)
movl  $190, %eax
push  %eax
movl  $4, %eax
push  %eax
movl  $9, %eax
push  %eax
movl  -4(%ebp), %eax
pop  %ecx
imul  %ecx, %eax
push  %eax
movl  -12(%ebp), %eax
pop  %ecx
imul  %ecx, %eax
pop  %ecx
addl  %ecx, %eax
movl  %eax, -12(%ebp)
movl  $2, %eax
push  %eax
movl  -12(%ebp), %eax
pop  %ecx
imul  %ecx, %eax
push  %eax
movl  $90, %eax
push  %eax
movl  $2, %eax
push  %eax
movl  -16(%ebp), %eax
pop  %ecx
addl  %ecx, %eax
pop  %ecx
xor  %edx, %edx
idivl  %ecx
push  %eax
movl  -20(%ebp), %eax
movl  %ebp, %esp
pop  %ebp
ret  
