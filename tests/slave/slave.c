/*-
 * Copyright (c) 1999 Assar Westerlund
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $FreeBSD: src/share/examples/kld/syscall/test/call.c,v 1.6.2.1.2.1 2011/11/11 04:20:22 kensmith Exp $
 */

#include <sys/types.h>
#include <sys/module.h>
#include <sys/select.h>
#include <sys/syscall.h>
#include <sys/wait.h>

#include <err.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <fsyscall/syscall.h>

static int
find_syscall()
{
	int modid = modfind("sys/fsyscall");
	if (modid == -1) {
		err(-1, "modified");
	}
	struct module_stat stat;
	stat.version = sizeof(stat);
	if (modstat(modid, &stat) != 0) {
		err(-1, "modstat");
	}
	return (stat.data.intval);
}

static int
read_int(int fd, int *p)
{
	int n = 0;
	char m = 0x80;
	int nbytes = sizeof(m);
	while (((m & 0x80) != 0) && (read(fd, &m, nbytes) == nbytes)) {
		n = (n << 7) + (m & 0x7f);
	}
	if ((m & 0x80) != 0)
		return (-1);
	*p = n;
	return (0);
}

static int
read_syscall(int fd)
{
	int n;
	if (read_int(fd, &n) != 0)
		err(1, "read_syscall");
	return n;
}

#define	array_sizeof(x)	(sizeof(x) / sizeof(x[0]))

static void
print_error(const char* fmt, ...)
{
	char buf[256];
	snprintf(buf, array_sizeof(buf), "%s\n", fmt);
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, buf, ap);
	va_end(ap);
}

static void
process_exit(int fd, int *status)
{
	if (read_int(fd, status) != 0)
		err(1, "read_int");
}

static bool
slave_main(int rfd, int *pstatus)
{
	int syscall_num = read_syscall(rfd);
	switch (syscall_num) {
	case SYSCALL_EXIT:
		process_exit(rfd, pstatus);
		return (false);
	default:
		print_error("Unknown syscall: %d", syscall_num);
		exit(1);
	}
	return (true);
}

int
main(int argc, char **argv)
{
	int syscall_num = find_syscall();
	if (syscall_num == -1) {
		return (1);
	}

	if (argc < 2) {
		return (1);
	}
	char *cmd = argv[1];
	char *args[argc];
	int i;
	for (i = 0; i < argc - 1; i++) {
		args[i] = argv[i + 1];
	}
	args[i] = NULL;
	char *envp[] = { NULL };

	int m2s[2];
	if (pipe(m2s) != 0) {
		err(1, "pipe (master to slave)");
	}
	int s2m[2];
	if (pipe(s2m) != 0) {
		err(1, "pipe (slave to master)");
	}
	int pid = fork();
	if (pid == -1) {
		err(1, "fork");
	}
#define	R	0
#define	W	1
	if (pid == 0) {
		close(m2s[R]);
		close(s2m[W]);
		syscall(syscall_num, s2m[R], m2s[W], cmd, args, envp);
		err(1, "fsyscall_execve");
	}

	close(m2s[W]);
	close(s2m[R]);

	int status = 0;
	while (slave_main(m2s[R], &status));
	if (waitpid(pid, NULL, 0) == -1)
		err(-1, "waitpid");

	return (status);
}
