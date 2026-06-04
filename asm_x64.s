/*

Writing C software without the standard library [Linux Edition] - Franc[e]sco's Gopherspace

https://gist.github.com/tcoppex/443d1dd45f873d96260195d6431b0989

*/

.text
	.globl _start

	_start:
		xor %rbp, %rbp
		pop %rdi
		mov %rsp, %rsi
		and $0xffffFFFFffffFFf0, %rsp
		call main
		
		mov %rax, %rdi
		mov $60, %rax
		syscall

