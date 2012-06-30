#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

void usage()
{
	fprintf(stderr, "usage: losetup [-r] [-c | -x | -d] [-o offset] [-s size] loop_device [file]\n");
}

int main(int argc, char **argv)
{
	char *progname = (char *)basename(argv[0]);
	int dis=0, ro=0, create=0, remove=0;
	int opt;
	char *file=0;
	unsigned size=0, off=0;
	while((opt = getopt(argc, argv, "cxrdo:s:")) != -1) {
		switch(opt) {
			case 'd':
				dis=1;
				break;
			case 'r':
				ro=1;
				break;
			case 'o':
				off = atoi(optarg);
				break;
			case 's':
				size = atoi(optarg);
				break;
			case 'c':
				create=1;
				break;
			case 'x':
				remove=1;
				break;
		}
	}
	char *loop = argv[optind];
	if(!loop) {
		usage();
		return 1;
	}
	if(!dis && !create && !remove) {
		file = argv[optind+1];
		if(!file) {
			usage();
			return 1;
		}
	}
	FILE *f = fopen(loop, "rw");
	if(!f)
	{
		fprintf(stderr, "%s: could not open loop device '%s': %s\n", progname, argv[optind], strerror(errno));
		return 2;
	}
	int ret;

	if(create) {
		ret = ioctl(fileno(f), 4, 0);
	} else if(remove) {
		ret = ioctl(fileno(f), 5, 0);
	} else if(dis) {
		ret = ioctl(fileno(f), 1, 0);
	} else {
		ret = ioctl(fileno(f), 0, (int)file);
		if(ret) goto errors;
		if(ro) {
			if(ioctl(fileno(f), 6, 1)) {
				fprintf(stderr, "%s: could not set loop device '%s' as read only: %s\n", progname, loop, strerror(errno));
				ret=4;
			}
		}
		if(size) {
			if(ioctl(fileno(f), 3, size)) {
				fprintf(stderr, "%s: could not set size on loop device '%s': %s\n", progname, loop, strerror(errno));
				ret=5;
			}
		}
		if(off) {
			if(ioctl(fileno(f), 2, off)) {
				fprintf(stderr, "%s: could not set offset on loop device '%s': %s\n", progname, loop, strerror(errno));
				ret=6;
			}
		}
	}
	errors:
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
	return ret < 0 ? 3 : ret;
}
