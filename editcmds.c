/*
 * editcmds - main command routines such as
 * append, doprint/dolist, lndelete, move
 * $Id$
 */

#include <stdio.h>
#include "edit.h"
#include "editdefs.h"

/* append -- append lines after `line' */
append(line, glob)
int	line, glob;
{
	char	inpline[MAXSTR];
	int	stat, done;

	if (glob)
		return ERR;		/* can't append globally */
	curln = line;
	stat = OK;
	done = FALSE;
	while (!done && stat == OK) {
		if (!fgets_nonl(inpline, MAXSTR-1, stdin))
			stat = ENDDATA;
		else if (inpline[0] == '.' && inpline[1] == '\0')
			done = TRUE;
		else if (puttxt(inpline) == ERR)
			stat = ERR;
	}
	return stat;
}

/* doprint -- print lines n1 through n2 */
doprint(n1, n2)
int n1, n2;
{
	int i;
	char line[MAXSTR];

	if (n1 <= 0 || n1 > n2 || n2 > lastln)
		return ERR;
	else
		for (i = n1; i <= n2; i++) {
			gettxt(i, line);
			puts(line);
		}
	curln = n2;
	return OK;
}

/* dolist -- print lines n1 through n2 with decoding */
/* keep logic similar to doprint! */
dolist(n1, n2)
int n1, n2;
{
	int i;
	char line[MAXSTR];

	if (n1 <= 0 || n1 > n2 || n2 > lastln)
		return ERR;
	else
		for (i = n1; i <= n2; i++) {
			gettxt(i, line);
			putlist(line);
			putchar('\n');
		}
	curln = n2;
	return OK;
}

putlist(s)
char	*s;
{
	char c;

	while (c = (*s++))
		if (c >= ' ')
			putchar(c);
		else
			switch (c) {
			case '\t':
				printf("\\t");
				break;
			case '\b':
				printf("\\b");
				break;
			default:
				printf("\\%03.3o", c);
			}
}

/* move -- move line1 .. line2 after line3 */
move(line3)
int	line3;
{
	if (line1 <= 0 || line3 >= line1 && line3 < line2)
		return ERR;
	blkmove(line1, line2, line3);
	if (line3 > line1)
		curln = line3;
	else
		curln = line3 + (line2 - line1 + 1);
	return OK;
}

/* lndelete -- delete lines n1 through n2 */
lndelete(n1, n2)
int	n1, n2;
{
	int i;

	if (n1 <= 0)
		return ERR;
	;	/* MAKE THIS: for (i = n1, n <= n2; i++) freeline(i); */
	blkmove(n1, n2, lastln);
	lastln -= (n2 - n1 + 1);
	curln = prevln(n1);
	return OK;
}
