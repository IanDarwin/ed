/*
 * getlist -- get list of line numbs at line[i], increment i
 * $Id$
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "edit.h"
#include "editdefs.h"

int
getlist(char *line, int *i)
{
	int	done, num;

	line2 = nlines = 0;
	done = getone(line, i, &num) != OK;
	while (!done) {
		line1 = line2;
		line2 = num;
		++nlines;
		if (*line == SEMICOL)
			curln = num;
		if (line[*i] == SEMICOL || line[*i] == COMMA) {
			++*i;
			done = getone(line, i, &num) != OK;
		} else
			done = 1;
	}
	nlines = (nlines > 2? 2: nlines);	/* min(nlines,2) */
	/* if no lines were specified, line1=line2=curln. */
	/* if one line was given, line1=line2=number given */
	if (nlines == 0)
		line2 = curln;
	if (nlines <= 1)
		line1 = line2;
	if (num != ERR)
		return OK;
	else
		return ERR;
}

/* getone -- get one line number from line */
/* pass back value in `num'; return status */
int
getone(char *line, int *i, int *num)
{
	int	istart, mul, pnum;
	int	status;

	istart = *i;
	*num = 0;
	if ((status = getnum(line, i, num)) == OK)	/* first term */
		do {
			if (line[*i] != '+' && line[*i] != '-')
				status = ENDDATA;
			else {
				if (line[*i] == '+')
					mul = 1;
				else
					mul = -1;
				++*i;
				if ((status = getnum(line, i, &pnum)) == OK)
					*num += mul * pnum;
				if (status == ENDDATA)
					status = ERR;
			}
		} while (status == OK);
	if (*num < 0 || *num > lastln)
		status = ERR;
	if (status != ERR) {
		if (*i <= istart)
			status = ENDDATA;
		else
			status = OK;
	}
	return status;
}

/*
 * ctoi - character to integer. like atoi but updates array position.
 * Used only by getnum!
 */
static int
ctoi(char *in, int *i)
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

/*
 * getnum -- get single line number.
 * pass back value in `num', return status
 */
int
getnum(char *line, int *i, int *num)
{
	int	status = OK;

	if (isdigit(line[*i])) {
		int j = ctoi(line, i);
		*num = j;
		status = OK;
		/* decrement i, if all well will advance at end. */
		/* must skip over delimiter/special char in all other cases */
		--*i;
	} else if (line[*i] == CURLINE) {
		*num = curln;
		status = OK;
	} else if (line[*i] == LASTLINE) {
		*num = lastln;
		status = OK;
	} else if (line[*i] == SCAN || line[*i] == BACKSCAN) {
		if ((status = optpat(line, i)) != ERR)
			status = patscan(line[*i], num);
	} else
		status = ENDDATA;
	if (status == OK)
		++*i;
	return status;
}
