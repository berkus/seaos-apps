#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
extern char **environ;
char *progname=0;
#define on_sob(n) change_sob(n, 1)
#define off_sob(n) change_sob(n, 0)
struct dirent *scan_and_open(DIR *d, char *name)
{
	struct dirent *ent;
	while((ent=readdir(d)))
	{
		char *e = strchr(ent->d_name+2, '.');
		if(e) *e=0;
		if(!strcmp(ent->d_name, name) || (!strcmp(ent->d_name+3, name))
			|| (!strcmp(ent->d_name+2, name)))
		{
			if(e) *e='.';
			return ent;
		}
		if(e) *e='.';
	}
	return 0;
}

int sob(char *name_o)
{
	DIR *d = opendir("/config/run/start");
	if(!d)
		return 0;
	struct dirent *f = scan_and_open(d, name_o);
	closedir(d);
	if(!f)
		return 0;
	if(f->d_name[0] == '.')
		return 0;
	return 1;
}

int change_sob(char *name_o, int sob)
{
	DIR *d = opendir("/config/run/start");
	if(!d) {
		fflush(0);
		fprintf(stderr, "%s: could not open start script directory: %s\n", progname, strerror(errno));
		return 0;
	}
	struct dirent *f = scan_and_open(d, name_o);
	closedir(d);
	if(!f) {
		fflush(0);
		fprintf(stderr, "%s: could not open start script for '%s'\n", progname, name_o);
		return 0;
	}
	if(sob && f->d_name[0] != '.')
		return 1;
	if(!sob && f->d_name[0] == '.')
		return 1;
	char new_name[50+strlen(f->d_name)];
	if(sob)
		sprintf(new_name, "/config/run/start/%s", f->d_name+1);
	else
		sprintf(new_name, "/config/run/start/.%s", f->d_name);
	char old_name[50+strlen(f->d_name)];
	sprintf(old_name, "/config/run/start/%s", f->d_name);
	int ret = link(old_name, new_name);
	if(ret == -1)
	{
		fflush(0);
		fprintf(stderr, "%s: %s: %s\n", progname, f->d_name, strerror(errno));
		return 0;
	}
	ret = unlink(old_name);
	if(ret == -1)
	{
		fflush(0);
		fprintf(stderr, "%s: %s: %s\n", progname, f->d_name, strerror(errno));
		unlink(new_name);
	}
	return 1;
}

int bring(char *name_o, char *direct_)
{
	if(!strcmp(direct_, "sob"))
		return on_sob(name_o);
	if(!strcmp(direct_, "nob"))
		return off_sob(name_o);
	char direct[16];
	if(!strcmp(direct_, "up"))
		strcpy(direct, "start");
	else if(!strcmp(direct_, "down"))
		strcpy(direct, "stop");
	else
		strcpy(direct, direct_);
	char dir[128];
	sprintf(dir, "/config/run/%s/", direct);
	DIR *d = opendir(dir);
	if(!d)
	{
		fprintf(stderr, "%s: failed to find method to change service.\n", progname);
		return 0;
	}
	struct dirent *f = scan_and_open(d, name_o);
	closedir(d);
	if(!f)
	{
		fprintf(stderr, "%s: could not find service '%s'\n", progname, name_o);
		return 0;
	}
	char script[strlen(dir) + strlen(f->d_name) + 12];
	sprintf(script, "%s/%s", dir, f->d_name);
	int pid;
	if(!(pid=fork()))
	{
		execl("/bin/sh", "sh", script, 0);
		exit(255);
	}
	waitpid(pid, 0, 0);
	return 1;
}

void list_services()
{
	DIR *d = opendir("/config/run/status");
	if(!d)
	{
		fprintf(stderr, "%s: could not read status information.\n", progname);
		return;
	}
	printf("SERVICE\t\tSTATUS\tAUTOSTART\n");
	struct dirent *e;
	while((e=readdir(d)))
	{
		char file[128];
		sprintf(file, "/config/run/status/%s", e->d_name);
		FILE *f = fopen(file, "r");
		char data[12];
		memset(data, 0, 12);
		fread(data, 1, 10, f);
		printf("%-12s\t%6s\t%3s\n", e->d_name, data, sob(e->d_name) ? "yes" : "no");
	}
	closedir(d);
}

void bring_all_down()
{
	DIR *d = opendir("/config/run/status");
	if(!d)
	{
		fprintf(stderr, "%s: could not read status information.\n", progname);
		return;
	}
	struct dirent *e;
	while((e=readdir(d)))
	{
		char file[128];
		sprintf(file, "/config/run/status/%s", e->d_name);
		FILE *f = fopen(file, "r");
		char data[12];
		memset(data, 0, 12);
		fread(data, 1, 10, f);
		if(!strcmp(data, "up"))
			bring(e->d_name, "down");
	}
	closedir(d);
}

void help()
{
	printf("bring - Utility for service control\n");
	printf("Usage:\n'bring <name> <control>'\n");
	printf("\tTells service <name> to do something specified by <control>\n");
	printf("\tPossible values for <control> are:\n\t\t'up':\tStarts a service\n");
	printf("\t\t'down':\tStops a service\n");
	printf("\t\t'stat':\tReturns status of a service\n");
	printf("\t\t'nob':\tSpecifies that a service shall not start on boot\n");
	printf("\t\t'sob':\tSpecifies that a service shall start on boot\n");
	printf("Switches:\n-l: List all services, their settings and their status\n");
	printf("-d: Bring all services that are up, down.\n");
	printf("Example: 'bring net up': Starts service 'net'.\n");
}

int main(int argc, char **argv)
{
	progname = (char *)basename(argv[0]);
	int i;
	for(i=1;i<argc;i++)
	{
		if(argv[i][0] == '-')
		{
			if(argv[i][1] == 'l')
			{
				list_services();
			}
			if(argv[i][1] == 'd')
			{
				bring_all_down();
			}
			if(argv[i][1] == 'h')
			{
				help();
			}
			
		} else if(i<argc-1)
		{
			int ret = bring(argv[i], argv[i+1]);i++;
			//if(!ret)
			//	fprintf(stderr, "%s: could not bring service '%s' %s\n", progname, argv[i-1], argv[i]);
		}
	}
	
	return 0;
	
}
