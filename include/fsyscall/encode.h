#if !defined(FSYSCALL_ENCODE_H)
#define FSYSCALL_ENCODE_H

#define	BUFSIZE_UINT	(32 / 7 + 1)

int encode_uint(unsigned int, char *, int);

#endif
