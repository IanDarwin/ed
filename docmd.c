/*
 * docmd -- handle all commands except globals
 * $Id$
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "edit.h"
#include "editdefs.h"

static int ckp(char *lin, int *i, int *pflag);
static int getfn(char *lin, int *i, char *fil);

int
docmd(char *lin, int *i, int glob)
{
	char	fname[MAXSTR], sub[MAXSTR];
	int	gflag = 0, line3 = 0, pflag = 0, status = ERR;

	switch (lin[*i]) {
	case '\0':
		if (glob)
			status = doprint(curln, curln);
		else {
			if (nlines == 0)
				line2 = nextln(curln);
			status = doprint(line2, line2);
		}
		break;
	case 'a':
		if (lin[*i+1] == '\0')
			status = append(line2, glob);
		break;
	case 'b':
		status = doprint(line2,
			line2+BLINES > lastln? lastln: line2+BLINES);
		break;
	case 'c':
		if (lin[*i+1] == '\0')
			if ((status = deflt(curln, curln)) == OK)
				if ((status = lndelete(line1, line2)) == OK)
					status = append(prevln(line1), glob);
		break;
	case 'd':
		if ((status = ckp(lin, i, &pflag)) == OK)
			if ((status = deflt(curln, curln)) == OK)
				if ((status = lndelete(line1, line2)) == OK)
					if (nextln(curln) != 0)
						curln = nextln(curln);
		break;
	case 'e':
		if ((status = getfn(lin, i, fname)) == OK)
			status = edit(fname);
		break;
	case 'f':
		if (savefile[0] == '\0')
			printf("no filename\n");
		else
			printf("%s\n", savefile);
		status = OK;
		break;
	case 'h':
		if (errmsg[0] != '\0') {
			puts(errmsg);
			strcpy(errmsg, "");
		} else
			puts("no previous error message");
		status = OK;
		break;
	case 'i':
		if (lin[*i+1] == '\0')
			status = append((line2 == 0? 0: prevln(line2)), glob);
		else
			status = ERR;
		break;
	case 'l':
		if ((status = deflt(curln, curln)) == OK)
			status = dolist(line1, line2);
		break;
	case 'm':
		++*i;
		if ((status = getone(lin, i, &line3)) == ENDDATA)
			status = ERR;
		if (status == OK)
			if ((status = ckp(lin, i, &pflag)) == OK)
				if ((status = deflt(curln, curln)) == OK)
					status = move(line3);
		break;
	case 'p':
		if ((status = deflt(curln, curln)) == OK)
			status = doprint(line1, line2);
		break;
	case 'q':
		return ENDDATA;
	case 'r':
		if ((status = getfn(lin, i, fname)) == OK)
			status = doread(line2, fname);
		break;
	case 's':
		++*i;
		if ((status = optpat(lin, i)) == OK)
			if ((status = getrhs(lin, i, sub, &gflag)) == OK)
				if ((status = ckp(lin, i, &pflag)) == OK)
					if ((status = deflt(curln, curln)) ==OK)
						status =subst(sub, gflag, glob);
		break;
	case 'w':
		if ((status = getfn(lin, i, fname)) == OK)
			if ((status = deflt(1, lastln)) == OK)
				status = dowrite(line1, line2, fname);
		break;
	case '=':
		if ((status = ckp(lin, i, &pflag)) == OK) {
			curln = line2;
			printf("%d %d\n", line2, lastln);
		}
		break;
	case SHELLESCAPE:
		if (system(lin+1) == 0)
			status = OK;
		printf("!\n");
		break;
	}
	if ((status == OK) && pflag)
		status = doprint(curln, curln);
	return status;
}

/* ckp -- check for 'p' after command */
static int
ckp(char *lin, int *i, int *pflag)
{
	*pflag = 0;
	++*i;			/* skip over command character */
	if (lin[*i] == 'p') {
		++*pflag;
		++*i;
	}
	if (lin[*i] == '\0')
		return OK;
	else
		return ERR;
}

/* getfn -- get file name from lin[i]... */
static int
getfn(char *lin, int *i, char *fil)
{
	int j = 0;

	(*i)++;			/* skip the cmd character */
	while (isspace(lin[*i]))
		(*i)++;
	if (lin[*i] == '\0') {	/* no filename in cmd */
		if (*savefile == '\0')
			return ERR;	/* nor any memory? */
		else
			strcpy(fil, savefile);	/* copy saved name */
		return OK;
	}
	while (lin[*i])
		fil[j++] = lin[(*i)++];
	fil[j] = '\0';
	return OK;
}
