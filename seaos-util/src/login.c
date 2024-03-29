/* 
 * Copyright (C) 2012  Daniel Bittman

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <sys/utsname.h>
#include <libgen.h>
char out_tty=0;
int pid;
char *name;
int check_password(char *);
char def_shell[8] = "/bin/sh";
void sigint_h(int g)
{
	
}

int main(int argc, char **argv)
{
	name = basename(argv[0]);
	/* First do some checks */
	unsigned myuid = getuid();
	if(myuid)
	{
		fprintf(stderr, "%s: login cannot be invoked as non-root user.\n", argv[0]);
		return -1;
	}
	FILE *f = fopen("/etc/passwd", "r");
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
