/*
 * main.c -- main routine for the Software Tools editor in C
 * $Id$
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>
#include "edit.h"
#include "editvars.h"

static jmp_buf jumper;

static void onintr(int signal);
static void usage(void);

int
main(int argc, char **argv)
{
	char	*edps, *getenv(const char *);
	int	c, cursave, more, status;
	extern int optind;

	progname = argv[0];
	initbuf();
	edsetbuf();
	strcpy(pat, "");
	strcpy(errmsg, "");
	strcpy(savefile, "");
	edps = getenv("EDPS");

	/* simple check for command line options */
	while ((c = getopt(argc, argv, "d")) != EOF)
		switch (c) {
		case 'd':
			++debug;
			break;
		case 'x':
			fprintf(stderr, "%s: warning -x not implemented\n",
				progname);
		default:
			usage();
		}
	status = OK;
	if (argc - optind == 1)
		if ((status = edit(argv[optind])) == ERR)
			printf("?\n");

	if (signal(SIGINT, SIG_IGN) != SIG_IGN)
		(void) signal(SIGINT, onintr);
	(void) setjmp(jumper);

	if (edps != NULL)
		printf("%s", edps);
	more = (fgets_nonl(lin, sizeof lin -1, stdin) != NULL);
	while (more) {
		linptr = 0;
		cursave = curln;
		if ((status = getlist(lin, &linptr)) != ERR) {
			if (ckglob(lin, &linptr) == OK)
				status = doglob(lin, &linptr, &cursave);
			else
				status = docmd(lin, &linptr, FALSE);
		/* else error, do nothing */
		}
		if (status == ERR) {
			printf("?\n");
			curln = cursave;
		} else if (status == ENDDATA)
			more = FALSE;
		if (more) {
			if (edps != NULL)
				printf("%s", edps);
			more = (fgets_nonl(lin, sizeof lin -1, stdin) != NULL);
		}
	}
	initbuf();
	return(0);
}

static void
usage(void)
{
	fprintf(stderr, "usage: %s [-d] [file]\n", progname);
}

static void
onintr(int signal)
{
	/* (void) signal(SIGINT, onintr);	/* reset for next time */
	strcpy(errmsg, "Interrupt");
	printf("\n?\n");
	longjmp(jumper, 1);		/* return to loop */
}
