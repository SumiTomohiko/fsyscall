#if !defined(FSYSCALL_PRIVATE_H_INCLUDED)
#define FSYSCALL_PRIVATE_H_INCLUDED

#include <sys/malloc.h>

MALLOC_DECLARE(M_FSYSCALL);

struct fsyscall_data {
	int rfd;
	int wfd;
};

#endif
