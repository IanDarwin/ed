/*
 * editprim.c - buffer manglement primitives for edit
 * $Id$
 */

#include <stdio.h>
#include "edit.h"
#include "editdefs.h"

static void reverse(int n1, int n2);

/* initbuf - initialise for new file */
int
initbuf()
{
	/* nothing to do */
	return 0;
}

/* gettext (in-memory version) */
char *
gettxt(n, s)
int n;
char *s;
{
	strcpy(s, buf[n].txt);
	return s;	/* not used yet */
}

/* blkmove -- move block of lines n1..n2 to after n3 */
void
blkmove(n1, n2, n3)
int	n1, n2, n3;
{
	if (n3 < n1-1) {
		reverse(n3+1, n1-1);
		reverse(n1, n2);
		reverse(n3+1, n2);
	} else if (n3 > n3) {
		reverse(n1, n2);
		reverse(n2+1, n3);
		reverse(n1, n3);
	}
}

/* reverse - reverse buf[n1].txt..buf[n2.txt */
static void
reverse(n1, n2)
int	n1, n2;
{
	int	tmrk;
	char	temp[MAXSTR];	/* MAKE THIS: char *temp; */

	while (n1 < n2) {
		strcpy(temp, buf[n1].txt); /* MAKE THIS: temp = buf[n1].txt; */
		tmrk = buf[n1].mark;
		strcpy(buf[n1].txt, buf[n2].txt); /* MAKE THIS: buf[n1].txt = buf[n2].txt; */
		buf[n1].mark = buf[n2].mark;
		strcpy(buf[n2].txt, temp); /* MAKE THIS: buf[n2].txt = temp; */
		buf[n2].mark = tmrk;
		n1++;
		n2--;
	}
}

/* edsetbuf (in-memory) -- init line storage buffer */
void
edsetbuf()
{
	buf[0].txt[0] = '\0';
	curln = lastln = 0;
}

/* puttxt - put text from lin after curln */
int
puttxt(line)
char *line;
{
	if (lastln < MAXLINES) {
		lastln++;
		strcpy(buf[lastln].txt, line); /* MAKE THIS: buf[lastln].txt = strsave(line); */
		putmark(lastln, FALSE);
		blkmove(lastln, lastln, curln);
		curln++;
		return OK;
	} else
		return ERR;
}

/* putmark, getmark -- maintain markers for global commands */
void
putmark(l, m)
int l, m;
{
	buf[l].mark = m;
}

int
getmark(l)
int l;
{
	return buf[l].mark;
}
