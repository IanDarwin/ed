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

int addstr(char c, char *outset, int *j, int maxset);
int amatch(char *line, int offset, char *patt, int j);
int append(int line, int glob);
void blkmove(int n1, int n2, int n3);
int ckglob(char *l, int *i);
int deflt(int def1, int def2);
int docmd(char *lin, int *i, int glob);
void dodash(char dlm, char *src, int *i, char *dest, int *j, int maxset);
int doglob(char *s, int *i, int *cursave);
int dolist(int n1, int n2);
int doprint(int n1, int n2);
int doread(int n1, char *fn);
int dowrite(int n1, int n2, char *fn);
int edit(char *filename);
void edsetbuf(void);
int error(char *s1, char *s2);
char esc(char *a, int *i);
char *fgets_nonl(char *s, int n, FILE *f);
char *gettxt(int n, char *s);
int getlist(char *line, int *i);
int getmark(int l);
int getnum(char *line, int *i, int *num);
int getone(char *line, int *i, int *num);
int getrhs(char *line, int *i, char *sub, int *gflag);
int initbuf(void);
int lndelete(int n1, int n2);
int makepat(char *arg, int start, char delim, char *patt);
int match(char *line, char *patt);
int move(int line3);
int nextln(int n);
int omatch(char *line, int *i, char *patt, int j);
int optpat(char *l, int *i);
int patscan(char way, int *n);
int patsize(char *patt, int n);
int prevln(int n);
void putmark(int l, int m);
int puttxt(char *line);
void stclose(char *patt, int *j, int lastj);
int subst(char *sub, int gflag, int glob);
