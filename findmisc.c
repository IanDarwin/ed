/* editpatt -- optpat and patscan for editor */

#include <stdio.h>
#include "edit.h"
#include "editdefs.h"

/* optpat -- get optional pattern from line[i], incr i */
int
optpat(char *l, int *i)
{
	if (l[*i] == '\0')
		*i = 0;
	else if (l[*i+1] == '\0')
		*i = 0;
	else if (l[*i+1] == l[*i])
		++*i;		/* //, leave pattern alone */
	else
		*i = makepat(l, *i+1, l[*i], pat);
	if (pat[0] == '\0')
		*i = 0;
	if (*i == 0) {
		pat[0] = '\0';
		return ERR;
	} else
		return OK;
}

/* patscan -- find next occurrence of pattern after line n */
int
patscan(char way, int *n)
{
	int	done, status;
	char	line[MAXSTR];

	*n = curln;
	status = ERR;
	done = FALSE;
	do {
		if (way == SCAN)
			*n = nextln(*n);
		else
			*n = prevln(*n);
		gettxt(*n, line);
		if (match(line, pat)) {
			status = OK;
			done = TRUE;
		}
	} while (*n != curln && !done);
	return status;
}
