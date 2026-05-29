/*

Writing C software without the standard library [Linux Edition] - Franc[e]sco's Gopherspace

https://gist.github.com/tcoppex/443d1dd45f873d96260195d6431b0989

*/

.intel_syntax noprefix

.text
	.globl _start

	_start:
		xor rbp, rbp
		pop rdi
		mov rsi, rsp
		and rsp, 0xffffFFFFffffFFf0
		call main
		
		mov rdi, rax
		mov rax, 60
		syscall

