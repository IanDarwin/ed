/* subst -- substitute "sub" for occurrences of pattern */

#include <stdio.h>
#include "edit.h"
#include "editdefs.h"

subst(sub, gflag, glob)
char	*sub;
int	gflag, glob;
{
	char	old[MAXSTR], new[MAXSTR];
	int j, k, lastm, line, m, stat, done, subbed;

	stat = (glob? OK: ERR);
	done = (line1 <= 0);
	line = line1;
	while (!done && line <= line2) { /* do subst from line1 to line2 */
		j = 0;
		subbed = FALSE;
		gettxt(line, old);
		lastm = 0;
		k = 0;
		while (old[k] != '\0') {
			if (gflag || !subbed)
				m = amatch(old, k, pat, 0);
			else
				m = 0;
			if (m > 0 && lastm != m) { /* replace matched text */
				subbed = TRUE;
				catsub(old, k, m, sub, new, &j, MAXSTR);
				lastm = m;
			}
			if (m == 0 || m == k)	/* no match or null match */
				addstr(old[k++], new, &j, MAXSTR);
			else 		/* skip matched text */
				k = m;
		}
		if (subbed) {
			if (!addstr('\0', new, &j, MAXSTR)) {
				stat = error("substitute too big", NULL);
				done = TRUE;
			}
			else {
				stat = lndelete(line, line);
				stat = puttxt(new);
				line2 += curln - line;
				line = curln;
				if (stat == ERR)
					done = TRUE;
				else
					stat = OK;
			}
		}
		++line;
	}
	return stat;
}

/* getrhs -- get right hand side of "s" command */
getrhs(line, i, sub, gflag)
char	*line;
int	*i;
char	*sub;
int	*gflag;
{
	if (line[*i] == '\0' || line[*i+1] == '\0')
		return ERR;
	*i = makesub(line, *i+1, line[*i], sub);
	if (*i == 0)
		return ERR;
	else if (line[*i+1] == 'g') {
		++*i;
		*gflag = TRUE;
	}
	else
		*gflag = FALSE;
	return OK;
}

/* makesub -- make substitution string from arg into sub */
makesub(arg, from, delim, sub)
char	*arg;
int	from;
char	delim;
char	*sub;
{
	int i, j = 0;

	i = from;
	while (arg[i] != delim && arg[i] != '\0') {
		if (arg[i] == '&')
			addstr(DITTO, sub, &j, MAXPAT);
		else
			addstr(esc(arg, &i), sub, &j, MAXPAT);
		++i;
	}
	if (arg[i] != delim)	/* missing delimiter */
		i = 0;
	else if (!addstr('\0', sub, &j, MAXPAT))
		i = 0;
	return i;
}

catsub(line, s1, s2, sub, new, k, maxnew)
char	*line, *sub, *new;
int s1, s2, *k, maxnew;
{
	int i = 0, j;

	while (sub[i]) {
		if (sub[i] == DITTO)
			for (j = s1; j < s2; j++)
				addstr(line[j], new, k, maxnew);
		else
			addstr(sub[i], new, k, maxnew);
		++i;
	}
}
