/*
 * Globals for the editor.
 * $Id$
 * File editvars is built from file editdefs - DO NOT EDIT editvars.h!
 */
extern struct buftype buf[MAXLINES];	/* the actual edit buffer */
extern int  line1, line2;		/* lines given in a command */
extern int  nlines;			/* # of line #'s in cmd */
extern int  curln, lastln;		/* value of dot, $ */
extern char pat[MAXSTR];		/* regular expression */
extern char lin[MAXSTR];		/* command line */
extern int  linptr;			/* pointer into line */
extern char *progname;			/* name we are running as, for messages */
extern char savefile[MAXSTR];		/* remembered file name */
extern char errmsg[BUFSIZ];		/* an error message */
extern int  debug;			/* self explanatory */
