/* misc -- misc short routines for editor */

#include <stdio.h>
#include "edit.h"
#include "editdefs.h"

/* addstr -- put c into outset[j] if it fits, increment j */
addstr(c, outset, j, maxset)
char	c, *outset;
int	*j;
int	maxset;
{
	if (*j > maxset)
		return FALSE;
	outset[*j] = c;
	++*j;
	return TRUE;
}

/* ckglob - if global prefix, mark lines to be affected */
ckglob(l, i)
char	*l;
char	*i;
{
	int n, gflag;
	char temp[BUFSIZ];
	int status;

	if (l[*i] != 'g' && l[*i] != 'v')
		status = ENDDATA;
	else {
		gflag = l[*i] == 'g';
		++*i;
		if (optpat(l, i) == ERR)
			status = ERR;
		else if ((status = deflt(1, lastln)) != ERR) {
			++*i;	/* mark affected lines */
			for (n = line1; n <= line2; ++n) {
				gettxt(n, temp);
				putmark(n, (match(temp, pat) == gflag));
			}
			for (n = 1; n < line1; ++n)	/* erase other marks */
				putmark(n, FALSE);
			for (n = line2+1; n <= lastln; ++n)
				putmark(n, FALSE);
			status = OK;
		}
	}
	return status;
}

/* deflt -- set defaulted line numbers */
deflt(def1, def2)
int	def1, def2;
{
	if (nlines == 0) {
		line1 = def1;
		line2 = def2;
	}
	if (line1 > line2 || line1 <= 0)
		return ERR;
	else
		return OK;
}

/* doglob - do command at s[i] on all marked lines */
doglob(s, i, cursave)
char	*s;
int	*i, *cursave;
{
	int count = 0, istart = *i, n = line1, status = OK;
	/* count is to prevent wraparound while looking for marked lines */

	do {
		if (getmark(n)) {
			putmark(n, FALSE);
			curln = n;
			*cursave = curln;
			*i = istart;
			if ((status = getlist(s, i)) == OK)
				if ((status = docmd(s, i, TRUE)) == OK)
					count = 0;
		}
		else {
			n = nextln(n);
			count++;
		}
	} while (count <= lastln && status == OK);
	return status;
}

/* edit -- edit one file */
edit(filename)
char	*filename;
{
	/* MAKE THIS: int status; */

	if (filename[0] == NULL)
		return ERR;	/* MAKE THIS: status = ERR; */
	/* MAKE THIS: else { */
	initbuf();
	edsetbuf();
	if (doread(0, filename) == OK)
		strcpy(savefile, filename);
	else
		savefile[0] = '\0';
	/* MAKE THIS: } */
	return OK;		/* MAKE THIS: return status */
}

/* error -- build comprehensive error message, print if non-interactive */
error(s1, s2)
char	*s1, *s2;
{
	extern int errno, sys_nerr;
	extern char *sys_errlist[], *progname;
	int interactive = isatty(fileno(stdin));
	char mymsg[BUFSIZ];

	strcpy(errmsg, "");
	/* if not running from terminal, prepend program name to message */		if (!interactive)
		if (progname) {
			strcat(errmsg, progname);
			strcat(errmsg, ": ");
		}
	sprintf(mymsg, s1, s2);
	strcat(errmsg, mymsg);
	if (errno > 0 && errno < sys_nerr) {
		strcat(errmsg, " (");
		strcat(errmsg, sys_errlist[errno]);
		strcat(errmsg, ")");
	}
	if (!interactive)
		puts(errmsg);
	/* return ERR so you can say status=error(...) - just shorthand */
	return ERR;
}

/* nextln -- get line after n */
nextln(n)
int	n;
{
	if (n >= lastln)
		return 0;	/* MAKE THIS: return 1; */
	else
		return ++n;
}

/* prevln -- get line before n */
prevln(n)
int	n;
{
	if (n <=0)	/* MAKE THIS: if (n <= 1) */
		return lastln;
	else
		return --n;
}
