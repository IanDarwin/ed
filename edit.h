/*
 * edit.h -- definitions for edit
 * $Id$
 */

#define ENDDATA (-1)
#define ERR	(-2)
#define FALSE	0
#define TRUE	1
#define OK	1
#define MAXLINES 2000
#define MAXSTR	BUFSIZ
#define MAXPAT	MAXSTR
#define BLINES	16		/* # of lines for B command */

#define CURLINE '.'		/* current line == . */
#define SEMICOL ';'
#define COMMA	','
#define LASTLINE '$'		/* metachar for last line */
#define DITTO	(-4)		/* encode of & char in subs */
#define ESCAPE	'\\'		/* escape for special characters */
#define SCAN	'/'		/* delimiter to scan forwards */
#define BACKSCAN '?'		/* delimiter to scan up */
#define SHELLESCAPE '!'

struct buftype {
	char	txt[MAXSTR];	/* MAKE THIS: char *txt; */
	int	mark;
};
