/* esc - handle C-like escapes
 * if a[i] is \, following char may be special.
 * updates i if so.
 * in any case, returns the character.
 * Will NOT work for multi-byte characters!
 */
char
esc(a, i)
char *a;
int *i;
{
	if (a[*i] != '\\')
		return a[*i];
	if (a[*i+1] == '\0')
		return '\\';	/* not special at end */
	*i++;
	if (a[*i] == 'n')
		return '\n';
	if (a[*i] == 't')
		return '\t';
	return a[*i];
}
