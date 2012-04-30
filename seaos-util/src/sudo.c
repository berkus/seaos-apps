#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <time.h>
#include <string.h>
#include <signal.h>
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
	if(argc == 1)
	{
		fprintf(stderr, "usage: sudo program [arguments]\n");
		return 1;
	}
	setbuf(stdout, 0);
	int r = check_password("god");
	if(r) {
		setuid(0);
		char *buf;
		int i, len=0;
		for(i=1;i<argc;i++)
			len += strlen(argv[i])+2;
		buf = malloc(len+1);
		*buf=0;
		for(i=1;i<argc;i++) {
			strcat(buf, argv[i]);
			strcat(buf, " ");
		}
		execl("/bin/sh", "sudo", "-c", buf);
	} else
		fprintf(stderr, "%s: invalid password\n", (char *)basename(argv[0]));
	
	return 0;
}
