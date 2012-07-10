# Makefile for syscall tables
#
# $FreeBSD: src/sys/i386/linux/Makefile,v 1.6.2.1.2.1 2011/11/11 04:20:22 kensmith Exp $

all: sysent

TARGETS = fsyscall_sysent.c fsyscall_syscall.h fsyscall_proto.h \
	  fsyscall_syscalls.c fsyscall_systrace_args.c

sysent: $(TARGETS)

SYS_DIR = /usr/src/sys

$(TARGETS): $(SYS_DIR)/kern/makesyscalls.sh syscalls.master syscalls.conf
	@: mv -f fsyscall_sysent.c fsyscall_sysent.c.bak
	@: mv -f fsyscall_syscall.h fsyscall_syscall.h.bak
	@: mv -f fsyscall_proto.h fsyscall_proto.h.bak
	@: mv -f fsyscall_syscalls.c fsyscall_syscalls.c.bak
	@: mv -f fsyscall_systrace_args.c fsyscall_systrace_args.c.bak
	sh $(SYS_DIR)/kern/makesyscalls.sh syscalls.master syscalls.conf

clean:
	rm -f $(TARGETS)
