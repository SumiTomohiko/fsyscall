
.PATH: fsyscall sys/i386/i386 sys/kern

KMOD = fsyscall
SRCS = syscall.c elf_machdep.c fsyscall_syscalls.c fsyscall_sysent.c \
       imgact_elf32.c kern_exec.c
KERNBUILDDIR = /usr/obj/usr/src/sys/GENERIC

.include <bsd.kmod.mk>
