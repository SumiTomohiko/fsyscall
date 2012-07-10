
#include <sys/cdefs.h>
#include <sys/param.h>
#include <sys/proc.h>
#include <sys/systm.h>

#include <sys/fsyscall/fsyscall_proto.h>

void
sys_fsyscall_exit(struct thread *td, struct fsyscall_exit_args *_)
{
    printf("%s:%u exit\n", __FILE__, __LINE__);
}

/**
 * vim: tabstop=4 shiftwidth=4 expandtab softtabstop=4
 */
