// fold.c
// Autor: Marek Marusic, FIT
// Řešení IJC-DU2, příklad 1 a), 20.4.2014

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>

#define UINT_MIN +0U
#define W_LIMIT 4096

int printW(char * word, unsigned int length, unsigned int line_max,unsigned int wLen)
{
	
	length += wLen;
	if(length >= line_max && length != wLen)
	{
		printf("\n");
		length = wLen;
	}
	else if(length < line_max && length > wLen)
	{
		printf(" ");
		length++;
	}

	printf("%s",word);
 //	int te = strlen(word);
 //	if(te != wLen)
 //		printf("-----");
 //	printf("\n %d wLen= %d strlen = %d\n",length,wLen,te);
	return length;
}

int main(int argc, char * argv[])
{
	int ErrWLINE = 0;
	int ErrWlen = 0;
	long long_line;
	unsigned int i;
	unsigned int line = 80;
	char * strERR;
	int Cut = 0;
	char * filename;

//-----------------ARGUMENTY---------------------
	filename = NULL;
	for(i = 1; i<argc;i++)
	{
		if ((strcmp(argv[i],"-w") == 0))
		{
			if(argc <= i+1 )
			{
    		fprintf(stderr,"ERROR Not num \n");
    		return 1;
    	}
			long_line = strtol(argv[i+1],&strERR,10);
			if(*strERR != 0 || long_line > INT_MAX || long_line < INT_MIN)
    	{
    		fprintf(stderr,"ERROR num too big \n");
    		return 1;
    	}	
    	line = long_line;
    	i++;
		}
		else if((strcmp(argv[i],"-c") == 0) )
		{
			Cut = 1;
		}
		else
		{
			filename = argv[i];
		}
	}
	
	//printf("line max = %d, cut = %d, filename = %s\n",line,Cut,filename);

//-----------------ARGUMENTY---------------------
	FILE *fr;
	char word[W_LIMIT];
	char c;
	int newline = 0;
	int length = 0;

	if(filename == NULL)
		fr = stdin;
	else{
		fr = fopen(filename,"r");
		if(!fr)
		{
			fprintf(stderr,"Cant open file\n");
			return 1;
		}
	}
	
	i = 0;
	

	while(( c = fgetc(fr) ) != EOF)
	{
		if(isspace(c))
		{
			if(c == '\n')
			{
				if(newline == 1)
				{ 
					printf("\n");
					//aby sa pri tisku nasledujuceho slova vytlacil aj novy riadok
					length = line; 
				}
					newline = 1;
			}
			word[i] = '\0';
			length = printW(word,length,line,i);
			i = 0;
			word[0] = '\0';
		}

		else if(i < W_LIMIT)
		{
			if(Cut == 1 && i >= line)
			{
				if(ErrWLINE == 0)
				{
					fprintf(stderr,"Chyba: Prilis dlhe slovo v riadku\n");
					ErrWLINE = 1;
				}
			}
			else
			{
				word[i] = c; 
				i++;
			}
			newline = 0;
		}
		else
		{
			if(ErrWlen == 0)
			{
				fprintf(stderr,"Chyba: Prilis dlhe slovo\n");
				ErrWlen = 1;
			}
			else 
				continue;
		}
	}

	if(word[0] != '\0');
		length = printW(word,length,line,i);

	if(filename != NULL)
		fclose(fr);

	return 0;
}