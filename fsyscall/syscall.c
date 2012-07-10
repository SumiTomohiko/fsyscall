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
 * $FreeBSD: src/share/examples/kld/syscall/module/syscall.c,v 1.6.4.1.2.1 2011/11/11 04:20:22 kensmith Exp $
 */

#include <sys/param.h>
#include <sys/eventhandler.h>
#include <sys/kernel.h>
#include <sys/malloc.h>
#include <sys/module.h>
#include <sys/proc.h>
#include <sys/sysent.h>
#include <sys/sysproto.h>
#include <sys/systm.h>

MALLOC_DEFINE(M_FSYSCALL, "fsyscall", "fsyscall");

struct fsyscall_args {
	int rfd;
	int wfd;
	const char* path;
	char *const *argv;
	char *const *envp;
};

struct fsyscall_data {
	int rfd;
	int wfd;
};

/*
 * The function for implementing the syscall.
 */
static int
fsyscall_execve(struct thread *td, struct fsyscall_args *uap)
{
	printf("rfd: %d\n", uap->rfd);
	printf("wfd: %d\n", uap->wfd);
	printf("path: %s\n", uap->path);
	int i;
	for (i = 0; uap->argv[i] != NULL; i++) {
		printf("argv[%d]: %s\n", i, uap->argv[i]);
	}
	for (i = 0; uap->envp[i] != NULL; i++) {
		printf("envp[%d]: %s\n", i, uap->envp[i]);
	}

	struct malloc_type *type = M_FSYSCALL;
	struct fsyscall_data *data = malloc(sizeof(*data), type, M_NOWAIT);
	data->rfd = uap->rfd;
	data->wfd = uap->wfd;
	td->td_proc->p_emuldata = data;
	return (sys_execve(td, (struct execve_args *)(&uap->path)));
}

/*
 * The `sysent' for the new syscall
 */
static struct sysent fsyscall_sysent = {
	5,				/* sy_narg */
	(sy_call_t *)fsyscall_execve	/* sy_call */
};

/*
 * The offset in sysent where the syscall is allocated.
 */
static int offset = NO_SYSCALL;

static eventhandler_tag fsyscall_exit_tag;

extern struct sysentvec elf32_freebsd_sysvec;

static void
process_exit(void *_, struct proc *p)
{
	if (p->p_sysent != &elf32_freebsd_sysvec)
		return;
	free(p->p_emuldata, M_FSYSCALL);
}

/*
 * The function called at load/unload.
 */
static int
fsyscall_modevent(struct module *_, int cmd, void *__)
{
	int error = 0;

	switch (cmd) {
	case MOD_LOAD :
		fsyscall_exit_tag = EVENTHANDLER_REGISTER(process_exit, process_exit, NULL, EVENTHANDLER_PRI_ANY);
		break;
	case MOD_UNLOAD :
		EVENTHANDLER_DEREGISTER(process_exit, fsyscall_exit_tag);
		break;
	default :
		error = EOPNOTSUPP;
		break;
	}
	return (error);
}

SYSCALL_MODULE(fsyscall, &offset, &fsyscall_sysent, fsyscall_modevent, NULL);
