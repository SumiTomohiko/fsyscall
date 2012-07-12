
#include <sys/cdefs.h>
#include <sys/param.h>
#include <sys/libkern.h>
#include <sys/limits.h>
#include <sys/proc.h>
#include <sys/syscallsubr.h>
#include <sys/sysproto.h>
#include <sys/uio.h>

#include <fsyscall/encode.h>
#include <fsyscall/module.h>

static struct fsyscall_data *
data_of_thread(struct thread *td)
{
	return (struct fsyscall_data *)td->td_proc->p_emuldata;
}

static int
do_write(struct thread *td, struct write_args *uap)
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
	return (error);
}

int
sys_fsyscall_write_int(struct thread *td, int n)
{
	char buf[BUFSIZE_UINT];
	int nbyte = encode_uint(n, buf, BUFSIZE_UINT);
	if (nbyte < 1)
		return (-1);
	struct write_args args;
	args.fd = data_of_thread(td)->wfd;
	args.buf = buf;
	args.nbyte = nbyte;
	return (do_write(td, &args));
}

int
sys_fsyscall_write_syscall(struct thread *td, int syscall_num)
{
	return (sys_fsyscall_write_int(td, syscall_num));
}

int
sys_fsyscall_write_str(struct thread *td, const char *s)
{
	size_t len = strlen(s);
	if (sys_fsyscall_write_int(td, len) < 0)
		return (-1);

	struct write_args args;
	args.fd = data_of_thread(td)->wfd;
	args.buf = s;
	args.nbyte = len;
	return (do_write(td, &args));
}
