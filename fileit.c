#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char basedir[];
static char filename[128];
static FILE *fp;

int fileit_init()
{
   strcpy(filename,basedir);
   strcat(filename,"/myfile");
   fp = fopen (filename, "w");

   return 0;
}

int fileit(short *sortword)
{
   fprintf(fp, "%d\n", *sortword);
   return 0;
}



int printit(long long *sortword)
{
   printf("Sortword value = %lld\n", *sortword);
   return 0;
}


int hello()
{
	printf("hello, world.\n");
	return 0;
}