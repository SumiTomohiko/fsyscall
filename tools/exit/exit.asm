
section .text
global _start
_start:
	mov ecx, [esp]
	sub ecx, 1
	push ecx
	mov eax, 1
	push eax
	int 80h
