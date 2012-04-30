#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

void usage()
{
	fprintf(stderr, "usage: ioctl [-d] loop-device file\n");
}

int main(int argc, char **argv)
{
	char *progname = (char *)basename(argv[0]);
	int dis=0;
	int opt;
	while((opt = getopt(argc, argv, "d")) != -1) {
		switch(opt) {
			case 'd':
				dis=1;
				break;
		}
	}
	if((optind + 2) > argc)
	{
		usage();
		return 1;
	}
	FILE *f = fopen(argv[optind], "rw");
	if(!f)
	{
		fprintf(stderr, "%s: could not open loop device '%s': %s\n", progname, argv[optind], strerror(errno));
		return 2;
	}
	int ret;
	if(dis)
	{
		ret = ioctl(fileno(f), 1, 0);
	} else 
		ret = ioctl(fileno(f), 0, (int)argv[optind+1]);
	if(ret == -1)
	{
		switch(errno)
		{
			case EINVAL:
				fprintf(stderr, "%s: Invalid loop device '%s'\n", progname, argv[optind]);
				break;
			case EEXIST:
				fprintf(stderr, "%s: Loop device '%s' is busy\n", progname, argv[optind]);
				break;
			default:
				fprintf(stderr, "%s: %s: %s\n", progname, argv[optind+1], strerror(errno));
				break;
		}
	}
	return ret;
}
