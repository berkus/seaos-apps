#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/dirent.h>
extern char verbose, no_scr, splash;
typedef struct rc_script_s {
	char filename[128];
	unsigned number;
	char hidden;
	char dont_wait;
	struct rc_script_s *next, *prev;
} rc_t;

rc_t *script_list=0;

rc_t *add_script(char *dir, char *filename)
{
	rc_t *r = (rc_t *)malloc(sizeof(rc_t));
	sprintf(r->filename, "%s%s", dir, filename);
	if(*filename == '.')
	{
		r->hidden = 1;
		filename++;
	}
	char id[3];
	memset(id, 0, 3);
	strncpy(id, filename, 2);
	r->number = atoi(id);
	r->dont_wait = ((r->number >= 90) ? 1 : 0);
	rc_t *s = script_list;
	script_list = r;
	r->next = s;
	r->prev=0;
	if(s)
		s->prev=r;
	return r;
}

void remove_script(rc_t *r)
{
	if(r->next)
		r->next->prev = r->prev;
	if(r->prev)
		r->prev->next = r->next;
	else
		script_list = r->next;
	free(r);
}

rc_t *get_next_script()
{
	rc_t *n=script_list;
	rc_t *t=n;
	while(t)
	{
		if(n->number > t->number)
			n=t;
		t=t->next;
	}
	return n;
}

void init_services()
{

}

void run_script(rc_t *rc)
{
	if(verbose)
		printf("Executing script %s\n", rc->filename);
	int pid;
	if(!(pid = fork()))
	{
		execl("/bin/sh", "sh", rc->filename);
		exit(1);
	}
	if(!rc->dont_wait)
		waitpid(pid, 0, 0);
}

void run_rc_scripts(char *type)
{
	char directory[256];
	sprintf(directory, "/config/run/%s/", type);
	if(verbose)
		printf("Enumerating scripts...\n");
	struct dirent *d;
	DIR *dre = opendir(directory);
	if(dre) { 
		while((d = readdir(dre)))
		{
			if(d->d_type != 1)
				continue;
			add_script(directory, d->d_name);
		}
		closedir(dre);
	} else
		return;
	/* Ok, we've loaded the scripts. Run them in numerical order */
	rc_t *scr=0;
	while((scr = get_next_script()))
	{
		if(!scr->hidden)
			run_script(scr);
		remove_script(scr);
	}
}
