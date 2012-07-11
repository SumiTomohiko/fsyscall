#include <fsyscall/encode.h>

static int
encode_uint_zero(char *buf, int bufsize)
{
	if (bufsize < 1)
		return (0);
	*buf = 0;
	return (1);
}

int
encode_uint(unsigned int n, char *buf, int bufsize)
{
	if (n == 0)
		return (encode_uint_zero(buf, bufsize));

	unsigned int m = n;
	int pos = 0;
	while ((m != 0) && (pos < bufsize)) {
		buf[pos] = (m & 0x7f) | ((m & ~0x7f) != 0 ? 0x80 : 0);

		m = m >> 7;
		pos++;
	}
	if (m != 0)
		return (0);

	return (pos);
}
