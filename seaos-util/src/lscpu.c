#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/dirent.h>
#include <errno.h>
int main(int argc, char **argv)
{
	int i=0;
	DIR *d = opendir("/proc/cpu");
	if(!d)
	{
		fprintf(stderr, "%s: %s: %s\n", basename(argv[0]), "/proc/cpu", strerror(errno));
		return 1;
	}
	struct dirent *de;
	while((de = readdir(d))) {
		char path[128];
		sprintf(path, "/proc/cpu/%s", de->d_name);
		int pid;
		if(!(pid=fork()))
			execl("/bin/cat", "/bin/cat", path);
		waitpid(pid, 0, 0);
		i++;
	}
	closedir(d);
	return 0;
}
