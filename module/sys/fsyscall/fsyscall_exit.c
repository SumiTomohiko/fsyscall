
#include <sys/cdefs.h>
#include <sys/param.h>
#include <sys/proc.h>

#include <fsyscall/module.h>
#include <fsyscall/syscall.h>
#include <sys/fsyscall/fsyscall_proto.h>

void
sys_fsyscall_exit(struct thread *td, struct fsyscall_exit_args *uap)
{
	if (sys_fsyscall_write_syscall(td, SYSCALL_EXIT) != 0)
		return;
	int rval = uap->rval;
	if (sys_fsyscall_write_int(td, rval) != 0)
		return;
	exit1(td, rval);
	/* NOTREACHED */
}
