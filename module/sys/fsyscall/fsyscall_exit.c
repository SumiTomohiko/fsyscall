
#include <sys/cdefs.h>
#include <sys/param.h>
#include <sys/proc.h>
#include <sys/syscallsubr.h>
#include <sys/sysproto.h>
#include <sys/systm.h>
#include <sys/uio.h>

#include <fsyscall/module.h>
#include <sys/fsyscall/fsyscall_proto.h>

static int do_write(struct thread *, struct write_args *);

static int
do_write(td, uap)
	struct thread *td;
	struct write_args *uap;
{
	struct uio auio;
	struct iovec aiov;
	int error;

#if 0
	if (uap->nbyte > INT_MAX)
		return (EINVAL);
#endif
	aiov.iov_base = (void *)(uintptr_t)uap->buf;
	aiov.iov_len = uap->nbyte;
	auio.uio_iov = &aiov;
	auio.uio_iovcnt = 1;
	auio.uio_resid = uap->nbyte;
	auio.uio_segflg = UIO_SYSSPACE;
	error = kern_writev(td, uap->fd, &auio);
	return(error);
}

void
sys_fsyscall_exit(struct thread *td, struct fsyscall_exit_args *uap)
{
	void *p_emuldata = td->td_proc->p_emuldata;
	struct fsyscall_data *data = (struct fsyscall_data *)p_emuldata;
	int cmd = 0;
	struct write_args args;
	args.fd = data->wfd;
	args.buf = &cmd;
	args.nbyte = sizeof(cmd);
	printf("%s:%u args.fd=%d\n", __FILE__, __LINE__, data->wfd);
	int error = do_write(td, &args);
	printf("%s:%u error=%d\n", __FILE__, __LINE__, error);
	args.buf = &uap->rval;
	error = do_write(td, &args);
	printf("%s:%u error=%d\n", __FILE__, __LINE__, error);
	exit1(td, uap->rval);
	/* NOTREACHED */
}
