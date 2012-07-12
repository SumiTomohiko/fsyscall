
#include <sys/cdefs.h>
#include <sys/param.h>
#include <sys/libkern.h>
#include <sys/proc.h>

#include <fsyscall/module.h>
#include <fsyscall/syscall.h>
#include <sys/fsyscall/fsyscall_proto.h>

int
sys_fsyscall_link(struct thread *td, struct fsyscall_link_args *uap)
{
	if (sys_fsyscall_write_syscall(td, SYSCALL_LINK) < 0)
		return (-1);
	if (sys_fsyscall_write_str(td, uap->path) < 0)
		return (-1);
	if (sys_fsyscall_write_str(td, uap->link) < 0)
		return (-1);
	return (0);
}
