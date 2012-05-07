#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <sys/utsname.h>
char out_tty=0;
int pid;
char *name;
char def_shell[8] = "/bin/sh";
void sigint_h(int g)
{
	
}

int check_password(char *username)
{
	int psgood=1;
	FILE *ps = fopen("/config/pswd", "r");
	if(ps) {
		char ps_b[128];
		while(1) {
			if(!fgets((char *)ps_b, 128, ps))
				break;
			if(ps_b[0] == '#') continue;
			char *r = strchr(ps_b, ';');
			if(r) *r=0;
			r = strchr(ps_b, ':');
			if(!strncmp(username, ps_b, r - ps_b) && (strlen(username) == (r - ps_b)))
			{
				printf("Password: ");
				char ps_t[128];
				memset(ps_t, 0, 128);
				ioctl(1, 1, 0);
				read(0, ps_t, 128);
				ioctl(1, 1, 15);
				char *w = strchr(ps_t, '\n');
				if(w) *w=0;
				fclose(ps);
				if(!strcmp(ps_t, r+1))
					return 1;
				return 0;
			}
			
		}
		fclose(ps);
	}
	return 1;
}

int main(int argc, char **argv)
{
	name = argv[0];
	int i;
	for(i=1;i<argc;i++)
	{
		/* Test arguments */
	}
	/* First do some checks */
	unsigned myuid = getuid();
	if(myuid)
	{
		fprintf(stderr, "%s: login cannot be invoked as non-root user.\n", argv[0]);
		return -1;
	}
	FILE *f = fopen("/config/passwd", "r");
	if(!f)
	{
		fprintf(stderr, "%s: unable to read user listing.\n", argv[0]);
		return -2;
	}
	fclose(f);
	if(isatty(1))
		out_tty=1;
	setbuf(stdout, NULL);
	waitpid(pid, 0, 0);
	signal(SIGINT, sigint_h);
	struct utsname name;
	uname(&name);
	/* Main loop */
	while(1)
	{
		printf("\nKernel \"%s\" version %s (tty%d)\nor: How I learned to stop worrying about system crashes and love my computer.\n\n", name.version, name.release, ioctl(0, 8, 0));
		char username[128];
		memset(username, 0, 128);
		printf("Login: ");
		while(!read(0, username, 127))
			;
		char *nl = strchr(username, '\n');
		if(nl) *nl=0;
		struct passwd *pwd = getpwnam(username);
		if(!pwd)
		{
			printf("Invalid Login\n");
			continue;
		}
		
		/* Check password */
		if(!check_password(username))
		{
			printf("Invalid Login\n");
			continue;
		}
		
		/* Okay, login the user */
		pid = fork();
		if(pid) {
			while(waitpid(pid, 0, 0) == -1);
			ioctl(1, 0, 0);
		} else {
			set_uid(pwd->pw_uid);
			char *shell = pwd->pw_shell;
			if(!*shell)
				shell = def_shell;
			chdir(pwd->pw_dir);
			time_t now;
			time(&now);
			printf("Logged in as '%s' on %s\n", username, ctime(&now));
			execl(shell, shell, "--login");
			if(shell != def_shell) 
			{
				shell = def_shell;
				fprintf(stderr, "%s: failed to start user's shell. Falling back to default...\n", name);
				execl(shell, shell, "-l");
			}
			fprintf(stderr, "%s: could not start a login shell.\n", name);
			exit(1);
		}
	}
}
