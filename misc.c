/* misc -- misc short routines for editor */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "edit.h"
#include "editdefs.h"

/* addstr -- put c into outset[j] if it fits, increment j */
int
addstr(char c, char *outset, int *j, int maxset)
{
	if (*j > maxset)
		return FALSE;
	outset[*j] = c;
	++*j;
	return TRUE;
}

/* ckglob - if global prefix, mark lines to be affected */
int
ckglob(char *l, int *i)
{
	int n, gflag;
	char temp[BUFSIZ];
	int status = 0;

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
int
deflt(int def1, int def2)
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
int
doglob(char *s, int *i, int *cursave)
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
int
edit(char *filename)
{
	int status = 0;

	if (filename[0] == NULL)
		status = ERR;
	else {
		initbuf();
		edsetbuf();
		if (doread(0, filename) == OK)
			strcpy(savefile, filename);
		else
			savefile[0] = '\0';
	}
	return status;
}

/* error -- build comprehensive error message, print if non-interactive */
int
error(char *s1, char *s2)
{
	extern char *progname;
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
	/* return ERR so you can say return error(...) - just shorthand */
	return ERR;
}

/* esc - handle C-like escapes
 * if a[i] is \, following char may be special.
 * updates i if so.
 * in any case, returns the character.
 * Will NOT work for multi-byte characters!
 */
char
esc(char *a, int *i)
{
	if (a[*i] != '\\')
		return a[*i];
	if (a[*i+1] == '\0')
		return '\\';	/* not special at end */
	i++;
	if (a[*i] == 'n')
		return '\n';
	if (a[*i] == 't')
		return '\t';
	return a[*i];
}

/* nextln -- get line after n */
int
nextln(int n)
{
	if (n >= lastln)
		return 1;	/* allow wraparound */
	else
		return ++n;
}

/* prevln -- get line before n */
int
prevln(int n)
{
	if (n <= 1)		/* ditto */
		return lastln;
	else
		return --n;
}
