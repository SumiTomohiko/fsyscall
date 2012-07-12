
#include <sys/cdefs.h>
#include <sys/param.h>
#include <sys/libkern.h>
#include <sys/proc.h>

#include <fsyscall/module.h>
#include <fsyscall/syscall.h>
#include <sys/fsyscall/fsyscall_proto.h>

int
sys_fsyscall_creat(struct thread *td, struct fsyscall_creat_args *uap)
{
	if (sys_fsyscall_write_syscall(td, SYSCALL_CREAT) < 0)
		return (-1);
	if (sys_fsyscall_write_str(td, uap->path) < 0)
		return (-1);
	if (sys_fsyscall_write_int(td, uap->mode) < 0)
		return (-1);
	return (0);
}
