%include '../system.inc'

section .text
global _start
_start:
	mov ecx, [esp + 8]
	mov edx, [esp + 12]
	push edx
	push ecx
	sys.link

	push 0
	sys.exit
