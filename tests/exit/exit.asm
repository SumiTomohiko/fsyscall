
section .text
global _start
_start:
	push dword 0
	mov eax, 1
	push eax
	int 80h
