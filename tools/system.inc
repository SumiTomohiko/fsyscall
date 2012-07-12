section .text
align 4
access.the.bsd.kernel:
	int 80h
	ret

%macro system 1
	mov eax, %1
	call access.the.bsd.kernel
%endmacro

%macro sys.exit 0
	system 1
%endmacro

# vim: filetype=asm
