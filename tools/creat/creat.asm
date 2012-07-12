%include '../system.inc'

section .text
global _start
_start:
	push dword [esp + 8]
	push 0x644
	sys.creat

	push 0
	sys.exit
