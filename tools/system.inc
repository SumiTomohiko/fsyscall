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

%macro sys.creat 0
	system 8
%endmacro

%macro sys.link 0
	system 9
%endmacro

# vim: filetype=asm
