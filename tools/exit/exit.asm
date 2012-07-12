%include '../system.inc'

section .text
global _start
_start:
	mov ecx, [esp]
	sub ecx, 1

	push ecx
	sys.exit
