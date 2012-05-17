#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <pwd.h>
#include <errno.h>
#include <unistd.h>
extern char **environ;
int reset=0;

int main(int argc, char **argv)
{
	int i;
	int pid=0;
	for(i=0;i<argc;i++)
	{
		if(!strcmp(argv[i], "-r"))
			reset=1;
	}
	i=0;
	char sc[128];
	char enter[128];
	system("sh /config/rc/shutdown");
	if(reset) 
		kernel_reset();
	else
		kernel_shutdown();
	fprintf(stderr, "shutdown: Shutdown failed. You may need to be root\n");
	return 0;
}
