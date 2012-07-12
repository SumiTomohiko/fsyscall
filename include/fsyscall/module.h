#if !defined(FSYSCALL_MODULE_H_INCLUDED)
#define FSYSCALL_MODULE_H_INCLUDED

#include <sys/cdefs.h>
#include <sys/malloc.h>
#include <sys/proc.h>

MALLOC_DECLARE(M_FSYSCALL);

struct fsyscall_data {
	int rfd;
	int wfd;
};

int sys_fsyscall_write_int(struct thread *, int);
int sys_fsyscall_write_str(struct thread *, const char*);
int sys_fsyscall_write_syscall(struct thread *, int);

#endif
