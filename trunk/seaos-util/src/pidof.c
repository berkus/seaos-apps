#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
int main(int argc, char **argv)
{
	int i;
	int t=0;
	struct task_stat s;
	while(!task_stat(i, &s))
	{
		if(s.exe)
		{
			char b[128];
			strcpy(b, (char *)s.exe);
			if(b[strlen(b)-1] == '/')
				b[strlen(b)-1]=0;
			int q;
			for(q=1;q<argc;q++)
			{
				if(!strcmp(argv[q], b))
					printf("%d ", s.pid, t++);
				else
				{
					
					char *st = strrchr(b, '/');
					if(st && *(st++)) {
						char *a = strrchr(argv[q], '/');
						if(!a || !*a)
						{
							if(!strcmp(argv[q], st))
								printf("%d ", s.pid, t++);
						} else
						{
							if(!strcmp(a, st))
								printf("%d ", s.pid, t++);
						}
					}
				}
			}
		}
		i++;
	}
	if(t)
		printf("\n");
}
