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

int amatch(char *line, int offset, char *patt, int j);
int append(int line, int glob);
void blkmove(int n1, int n2, int n3);
int doglob(char *s, int *i, int *cursave);
int dolist(int n1, int n2);
int doprint(int n1, int n2);
int edit(char *filename);
void edsetbuf(void);
char esc(char *a, int *i);
char *gettxt(int n, char *s);
int getlist(char *line, int *i);
int getmark(int l);
int initbuf(void);
int lndelete(int n1, int n2);
int match(char *line, char *patt);
int move(int line3);
void putmark(int l, int m);
int puttxt(char *line);
int subst(char *sub, int gflag, int glob);
