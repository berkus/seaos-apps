#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
void help()
{
	printf("spawn - Create a seperate process\n");
	printf("usage: spawn [options] -c <command> <arg1> <arg2> <...>\n");
	printf("All extra arguments will be passed to the new process\n");
	printf("Options:\n-o\tSpecify stdout\n-i\tSpecify stdin\n-e\tSpecify stderr\n");
	printf("-a\tSpecify all (std[in/out/err]) device with one option (shortcut)\n");
	printf("-c\tSpecify command to run\n");
}

int main(int argc, char **argv)
{
	int i;
	char *out=0, *in=0, *err=0, *cmd=0, **a=0;
	opterr=0;
	int opt;
	while((opt = getopt(argc, argv, "c:a:i:o:e:")) != -1) {
		switch(opt) {
			case 'c':
				cmd = optarg;
				break;
			case 'a':
				out = in = err = optarg;
				break;
			case 'e':
				err = optarg;
				break;
			case 'i':
				in = optarg;
				break;
			case 'o':
				out = optarg;
				break;
		}
	}
	if(optind < argc)
		a = argv + optind-1;
	else {
		a = argv;
		a[1]=0;
	}
	a[0]=cmd;
	if(!cmd)
	{
		fprintf(stderr, "spawn: you must specify a command!\n");
		return 1;
	}
	int pid;
	if(!(pid=fork()))
	{
		if(in) {
			close(0);
			if(open(in, O_RDONLY) != 0)
			{
				fprintf(stderr, "spawn: could not open %s for stdin\n", in);
				exit(1);
			}
		}
		if(out) {
			close(1);
			if(open(out, O_RDWR) != 1)
			{
				fprintf(stderr, "spawn: could not open %s for stdout\n", out);
				exit(1);
			}
		}
		if(err) {
			close(2);
			if(open(err, O_RDWR) != 2)
			{
				fprintf(stderr, "spawn: could not open %s for stderr\n", err);
				exit(1);
			}
		}
		ioctl(1, 2, 0); 
		execv(cmd, a);
	}
	return 0;
}
