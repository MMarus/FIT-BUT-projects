//io.c
// Autor: Marek Marusic, FIT
// Řešení IJC-DU2, příklad 2, 20.4.2014

#include <stdio.h>
#include <ctype.h>

int fgetw(char *s, int max, FILE *f)
{
	int len = 0;
	char c=0;
	while((c = fgetc(f)) != EOF)
	{
		if(isspace(c))
			break;
		else if (len < max)
		{
			s[len] = c;
			len++;
		}
		else if(len >= max)
		{
			len = max+1;
		}
	}

	if(len <= max)
		s[len] = '\0';
	else 
		s[max] = '\0';
	//printf("%s %d\n",s,len);

	if(c == EOF && len == 0 )
		return EOF;
	else
	 return len;
}