#include <stdio.h>
#include <string.h>

int
ctoi(in,i)
char *in;
int *i;
{
	static char digits[] = "0123456789";
	int ret;
	char *t;

	while (isascii(in[*i]) && isspace(in[*i]))
		++*i;
	for (ret=0; in[*i]; ++*i) {
		t = strchr(digits, in[*i]);
		if (t == NULL)
			break;	/* non-digit */
		ret = 10 * ret + (*t)-'0';
	}
	return ret;
}
