/* dowrite -- write lines n1 through n2 to file */

#include <stdio.h>
#include <assert.h>
#include "edit.h"
#include "editdefs.h"

dowrite(n1, n2, fn)
int n1, n2;
char *fn;
{
	FILE *f;
	int i;
	char line[MAXSTR];
	int nch = 0, nli = 0;

	if (n1 <= 0 || n1 > n2 || n2 > lastln)
		return ERR;
	if ((f = fopen(fn, "w")) == 0) {
		strcpy(errmsg, "open for write failed");
		return ERR;
	} 
	for (i = n1; i <= n2; i++) {
		gettxt(i, line);
		fprintf(f, "%s\n", line);
		nch += strlen(line) + 1;
		++nli;
	}
	fclose(f);
	printf("%u/%u %s\n", nch, nli, fn);
	curln = n2;
	return OK;
}

int
doread(n1, fn)
int n1;
char *fn;
{
	FILE *f;
	int i;
	char line[MAXSTR];
	int nch = 0, nli = 0;

	if (n1 < 0 || n1 > lastln)
		return ERR;

	if ((f = fopen(fn, "r")) == 0) {
		strcpy(errmsg, "open for read failed");
		return ERR;
	} 
	curln = n1;
	while (fgets(line, sizeof line -1, f) != NULL) {
		i = strlen(line);
		nch += i; nli++;
		if (i)
			line[i-1] = '\0';	/* chop \n */
		puttxt(line);
	}
	fclose(f);
	printf("%u/%u %s\n", nch, nli, fn);
	assert(curln == n1 + nli);
	return OK;
}
