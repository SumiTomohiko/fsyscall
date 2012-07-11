
#include <sys/cdefs.h>
#include <sys/limits.h>
#include <sys/param.h>
#include <sys/proc.h>
#include <sys/syscallsubr.h>
#include <sys/sysproto.h>
#include <sys/systm.h>
#include <sys/uio.h>

#include <fsyscall/encode.h>
#include <fsyscall/module.h>
#include <fsyscall/syscall.h>
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

	if (INT_MAX < uap->nbyte)
		return (EINVAL);
	aiov.iov_base = (void *)(uintptr_t)uap->buf;
	aiov.iov_len = uap->nbyte;
	auio.uio_iov = &aiov;
	auio.uio_iovcnt = 1;
	auio.uio_resid = uap->nbyte;
	auio.uio_segflg = UIO_SYSSPACE;
	error = kern_writev(td, uap->fd, &auio);
	return(error);
}

static int
write_int(struct thread *td, int fd, int n)
{
	char buf[BUFSIZE_UINT];
	int nbyte = encode_uint(n, buf, BUFSIZE_UINT);
	if (nbyte < 1)
		return (-1);
	struct write_args args;
	args.fd = fd;
	args.buf = buf;
	args.nbyte = nbyte;
	return (do_write(td, &args));
}

static int
write_syscall(struct thread *td, int fd)
{
	return write_int(td, fd, SYSCALL_EXIT);
}

void
sys_fsyscall_exit(struct thread *td, struct fsyscall_exit_args *uap)
{
	void *p_emuldata = td->td_proc->p_emuldata;
	struct fsyscall_data *data = (struct fsyscall_data *)p_emuldata;
	int wfd = data->wfd;
	if (write_syscall(td, wfd) != 0)
		return;
	if (write_int(td, wfd, uap->rval) != 0)
		return;
	exit1(td, uap->rval);
	/* NOTREACHED */
}
