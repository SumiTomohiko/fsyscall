
.PATH: fsyscall sys/fsyscall sys/i386/i386 sys/kern

KMOD = fsyscall
SRCS = elf_machdep.c fsyscall_exit.c fsyscall_syscalls.c fsyscall_sysent.c \
       imgact_elf32.c kern_exec.c syscall.c
KERNBUILDDIR = /usr/obj/usr/src/sys/GENERIC
CFLAGS += -Isys -Iinclude

.include <bsd.kmod.mk>
