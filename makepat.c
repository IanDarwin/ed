/* make pat -- make pattern from arg[i], stop at delim */

#include <stdio.h>
#include <ctype.h>
#include "edit.h"
#include "findcons.h"

/* return ERR or index of closing delimiter */
makepat(arg, start, delim, patt)
char	*arg;
int	start;
char	delim;
char	*patt;
{
	int	i = start, j = 0, lastj = 0, lj = 0, done = FALSE;

	while (!done & arg[i] != '\0' && arg[i] != delim) {
		lj = j;
		if (arg[i] == ANY)
			(void) addstr(ANY, patt, &j, MAXPAT);
		else if (arg[i] == BOL && i == start)
			(void) addstr(BOL, patt, &j, MAXPAT);
		else if (arg[i] == EOL && arg[i+1] == delim)
			(void) addstr(EOL, patt, &j, MAXPAT);
		else if (arg[i] == CCL)
			done = getccl(arg, &i, patt, &j) == FALSE;
		else if (arg[i] == CLOSURE && i > start) {
			lj = lastj;
			if (patt[lj] == BOL || patt[lj] == EOL ||
			    patt[lj] == CLOSURE)
				done = TRUE;	/* terminate loop */
			else
				stclose(patt, &j, lastj);
		} else {
			(void) addstr(LITCHAR, patt, &j, MAXPAT);
			(void) addstr(esc(arg, &i), patt, &j, MAXPAT);
		}
		lastj = lj;
		if (!done)
			++i;
	}
	if (done || arg[i] != delim)		/* finished early */
		return error("incomplete pattern", (char *)NULL);
	else if (!addstr('\0', patt, &j, MAXPAT))
		return error("pattern too long", "All the world's not a VAX");
	else
		return i;
}

/* getccl -- expand char class at arg[i] into pat[j] */
/* return TRUE if pattern OK, FALSE if a problem. */
int
getccl(arg, i, patt, j)
char *arg;
int *i, *j;
char *patt;
{
	int jstart;

	++*i;			/* skip over '[' */
	if (arg[*i] == NEGATE) {
		(void) addstr(NCCL, patt, j, MAXPAT);
		++*i;
	} else
		(void) addstr(CCL, patt, j, MAXPAT);
	jstart = *j;
	(void) addstr(0, patt, j, MAXPAT);	/* room for count */
	dodash(CCLEND, arg, i, patt, j, MAXPAT);
	patt[jstart] = *j - jstart - 1;
	return arg[*i] == CCLEND;
}

/* stclose -- store closure into array at patt[j] */
stclose(patt, j, lastj)
char	*patt;
int	*j, lastj;
{
	int jp, jt;

	for (jp = *j - 1; jp >= lastj; --jp) {
		jt = jp + CLOSIZE;
		(void) addstr(patt[jp], patt, &jt, MAXPAT);
	}
	*j += CLOSIZE;
	patt[lastj] = CLOSURE;	/* where original pattern began */
}

/* dodash - expand dash shorthand set at scr[i] into dest[j], stop at dlm */
dodash(dlm, src, i, dest, j, maxset)
char dlm, *src, *dest;
int *i, *j, maxset;
{
	int k;

	while (src[*i] != dlm && src[*i] != '\0') {
		if (src[*i] == ESCAPE)
			(void) addstr(esc(src, i), dest, j, maxset);
		else if (src[*i] != DASH)
			(void) addstr(src[*i], dest, j, maxset);
		else if (*j <= 0 || src[*i+1] == '\0')
			(void) addstr(DASH, dest, j, maxset);	/* literal - */
		else if (isalnum(src[*i-1]) && isalnum(src[*i+1]) &&
		    src[*i-1] <= src[*i+1]) {
			for (k = src[*i-1]+1; k <= src[*i+1]; k++)
				(void) addstr(k, dest, j, maxset);
			++*i;
		} else
			(void) addstr(DASH, dest, j, maxset);
		++*i;
	}
}
