#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "init.h"
char splash=1;
char verbose=0;
char no_scr=0;
void run_rc_scripts(char *type);
void init_services();

void shutdown_system(int reset)
{
	run_rc_scripts("stop");
	init_services();
	
	setuid(0);
	reset == 0 ? kernel_shutdown() : kernel_reset();
	fprintf(stderr, "init: shutdown failed\n");
}

int main(int argc, char **argv)
{
	setbuf(stdout, 0);
	if(!isatty(1))
		splash=0;
	int i;
	for(i=0;i<argc;i++)
	{
		if(!strcmp(argv[i], "no_scr"))
			no_scr=1;
		else if(!strcmp(argv[i], "down"))
		{
			shutdown_system(0);
			return 0;
		}
		else if(!strcmp(argv[i], "reset"))
		{
			shutdown_system(1);
			return 0;
		}
		else if(!strcmp(argv[i], "verbose"))
		{
			splash=0;
			verbose=1;
		}
		else if(!strcmp(argv[i], "nosplash"))
			splash=0;
	}
	if(get_uid() != 0)
	{
		fprintf(stderr, "init: cannot invoke as non-root user!\n");
		return -1;
	}
	init_services();
	run_rc_scripts("start");
	
	int pid;
	if(!(pid = fork()))
	{
		signal(111, 0);
		signal(112, 0);
		execl("/bin/login", "login");
		exit(1);
	}
	exit(0);
	return 0;
}
