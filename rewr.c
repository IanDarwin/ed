/* fgets_nonl - fgets but chops the newline */
/* dowrite -- write lines n1 through n2 to file */
/* doread -- read file fn after line n1 */

#include <stdio.h>
#include <assert.h>
#include "edit.h"
#include "editdefs.h"

char *
fgets_nonl(char *s, int n, FILE *f)
{
	char *t = fgets(s, n, f);

	if (t) {
		int l = strlen(s);
		if (l)
			s[l-1] = '\0';
	}
	return t;
}

int
dowrite(int n1, int n2, char *fn)
{
	FILE *f;
	int i, status = OK;
	char line[MAXSTR];
	int nch = 0, nli = 0;

	if (n1 <= 0 || n1 > n2 || n2 > lastln)
		status = ERR;
	if ((f = fopen(fn, "w")) == 0)
		status = error("open of %s for write failed", fn);
	else { 
		for (i = n1; i <= n2; i++) {
			gettxt(i, line);
			fprintf(f, "%s\n", line);
			if (ferror(f)) {
				status = error("Write error on %s", fn);
				break;
			}
			nch += strlen(line) + 1;
			++nli;
		}
		fclose(f);
		printf("%u/%u %s\n", nch, nli, fn);
		curln = n2;
	}
	return status;
}

int
doread(int n1, char *fn)
{
	FILE *f;
	int status = OK;
	char line[MAXSTR];
	int nch = 0, nli = 0;

	if (n1 < 0 || n1 > lastln)
		status = ERR;
	else if ((f = fopen(fn, "r")) == 0)
		status = error("open of %s for read failed", fn);
	else {
		curln = n1;
		while (fgets_nonl(line, sizeof line -1, f) != NULL) {
			if (ferror(f)) {
				status = error("Read error on %s", fn);
				break;
			}
			nch += strlen(line)+1;
			nli++;
			puttxt(line);
		}
		fclose(f);
		printf("%u/%u %s\n", nch, nli, fn);
		assert(curln == n1 + nli);
	}
	return status;
}
