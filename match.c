/* match -- actual matching routines match amatch omatch */

#include <stdio.h>
#include "edit.h"
#include "findcons.h"

extern int errno;

static int locate(char c, char *patt, int offset);

/* match -- find pattern match anywhere on line */
int
match(char *line, char *patt)
{
	int	i = 0, j = 0;

	while (line[i] != '\0' && j == 0) {
		j = amatch(line, i, patt, 0);
		i++;
	}
	return j > 0;
}

/* amatch - look for match of patt[j]... at lin[offset]... */
int
amatch(char *line, int offset, char *patt, int j)
{
	int done = FALSE, i, k;

	while (!done && patt[j] != '\0')
		if (patt[j] == CLOSURE) {
			j += patsize(patt, j);	/* step over CLOSURE */
			i = offset;
			/* match as many as possible */
			while (!done && line[i] != '\0')
				if (omatch(line, &i, patt, j) != TRUE)
					done = TRUE;
			/*
			 * i points to input char that made omatch fail.
			 * match rest of pattern against rest of input.
			 * shrink closure by 1 after each failure.
			 */
			done = FALSE;
			while (!done && i >= offset) {
				k = amatch(line, i, patt, j+patsize(patt, j));
				if (k > 0)	/* matched rest of pattern */
					done = TRUE;
				else
					--i;
			}
			offset = k;	/* if k=0 failure, else success */
			done = TRUE;
		}
		else if (omatch(line, &offset, patt, j) != TRUE) {
			offset = 0;	/* non-closure */
			done = TRUE;
		}
		else	/* omatch succeeded on this pattern element */
			j += patsize(patt, j);
	return offset;
}

/* patsize -- return size of pattern entry at patt[n] */
int
patsize(char *patt, int n)
{
	switch(patt[n]) {
	case LITCHAR:
		return 2;
	case BOL:
	case EOL:
	case ANY:
		return 1;
	case CCL:
	case NCCL:
		return patt[n+1] + 2;
	case CLOSURE:
		return CLOSIZE;
	default:
		errno = 0;
		return error("in patsize: invalid case, can't happen", NULL);
	}
}

/* omatch -- match one pattern element at patt[j], return boolean */
int
omatch(char *line, int *i, char *patt, int j)
{
	int advance = -1;

	if (line[*i] == '\0')
		return FALSE;
	else
		switch(patt[j]) {
		case LITCHAR:
			if (line[*i] == patt[j+1])
				advance = 1;
			break;
		case BOL:
			if (*i == 0)
				advance = 0;
		case ANY:
			if (line[*i] != '\0')
				advance = 1;
			break;
		case EOL:
			if (line[*i] == '\0')
				advance = 0;
			break;
		case CCL:
			if (locate(line[*i], patt, j+1))
				advance = 1;
			break;
		case NCCL:
			if (line[*i] != '\0' && !locate(line[*i], patt, j+1))
				advance = 1;
			break;
		default:
			errno = 0;
			return error("omatch: bad case, can't happen", NULL);
		}
	if (advance >= 0) {
		*i += advance;
		return TRUE;
	}
	else
		return FALSE;
}

/* locate -- look for c in character class at patt[offset] */
static int
locate(char c, char *patt, int offset)
{
	int i, status = FALSE;

	/* size of class is at patt[offset], characters follow*/
	i = offset + patt[offset];	/* last position */
	while (i > offset)
		if (c == patt[i]) {
			status = TRUE;
			i = offset; /* force loop termination - should break? */
		}
		else
			--i;
	return status;
}
