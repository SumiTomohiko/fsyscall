
TOP_DIR = ..

.PATH: $(TOP_DIR)/share fsyscall sys/fsyscall sys/i386/i386 sys/kern

KMOD = fsyscall
SRCS = elf_machdep.c encode.c fsyscall_creat.c fsyscall_exit.c \
       fsyscall_io.c fsyscall_link.c fsyscall_syscalls.c fsyscall_sysent.c \
       imgact_elf32.c kern_exec.c syscall.c
KERNBUILDDIR = /usr/obj/usr/src/sys/GENERIC
CFLAGS += -Isys -I$(TOP_DIR)/include

.include <bsd.kmod.mk>
