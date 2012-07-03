#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
char *progname;
void usage()
{
	fprintf(stderr, "%s: usage: ioctl device command argument\n", progname);
}

int main(int argc, char **argv)
{
	progname = (char *)basename(argv[0]);
	int i;
	int cmd=0;
	int arg=0;
	FILE *f;
	if(argc != 4) {
		usage();
		return 1;
	}
	f = fopen(argv[1], "rw");
	cmd = atoi(argv[2]);
	/* -[arg] is interpreted as a string */
	if(argv[3][0] == '-')
		arg = (int)argv[3]+1;
	else
		arg = atoi(argv[3]);
	if(!f)
	{
		fprintf(stderr, "%s: %s: %s\n", progname, argv[1], strerror(errno));
		return 2;
	}
	return ioctl(fileno(f), cmd, arg);
}
