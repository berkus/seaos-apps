#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>

int main(int c, char **a)
{
	printf("TEST PROGRAM\n");
	fprintf(stderr, "This goes to stderr\n");
	int i;
	printf("Here are the arguments\n");
	for(i=0;i<c;i++)
	{
		printf("%d: %s\n", i, *(a+i));
	}
	printf("Lets malloc alot of memory\n");
	char *g = malloc(0x100000);
	printf("got %x\n", g);
	free(g);
	printf("Done testing\n");
	if(c > 1)
	{
		if(a[1][0] == 'p')
			syscall(0, 1, 0, 0, 0, 0);
		if(a[1][0] == 'f')
		{
			printf("NULL POINTER\n");
			unsigned *po = (unsigned *)0;
			printf("%x\n", *po);
		}
		printf("Testing usermode!\n");
		asm("cli");
		printf("Failed...\n");
	}
	return 0;
}
