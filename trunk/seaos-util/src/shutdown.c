#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <pwd.h>
#include <errno.h>
#include <unistd.h>
extern char **environ;
int logout=1;
int reset=0;
void run_script_exact(char *path, int re)
{
	int pid;
	if(!(pid=fork())) {
		execl("/bin/sh", "sh", path, 0);
		exit(255);
	}
	if(!re)
		waitpid(pid, 0, 0);
}

void whoami(char *usr_name)
{
	struct passwd *pw = getpwuid(get_uid());
	if(!pw)
		return;
	memset(usr_name, 0, 128);
	strcpy(usr_name, pw->pw_name);
}

int main(int argc, char **argv)
{
	int i;
	int pid=0;
	for(i=0;i<argc;i++)
	{
		if(!strcmp(argv[i], "-r"))
			reset=1;
		if(!strcmp(argv[i], "-l"))
			logout=0;
	}
	i=0;
	char sc[128];
	char enter[128];
	int qp;
	if(!(qp = fork()))
	{
		if(reset)
			execl("/bin/init", "init", "reset");
		else
			execl("/bin/init", "init", "down");
	}
	waitpid(qp, 0, 0);
	fprintf(stderr, "shutdown: Shutdown failed. You may need to be root\n");
	return 0;
}
