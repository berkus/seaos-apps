#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
void display_help()
{
	printf("Mount - mount a device on to a directory\n");
	printf("Syntax: mount [-t type] device directory\n");
	printf("Switches:\n\t-t <type>: Specify filesystem type (e.g. 'ext2')\n");
	printf("Note: Not specifying -t will have the kernel try all types until one works\n");
}

int main(int argc, char **argv)
{
	/* Parse cmd line */
	int opt;
	char * type=0;
	if(argc == 1)
		execl("/bin/df", "df", "-h");
	while((opt = getopt(argc, argv, "ht:")) != -1) {
		switch(opt) {
			case 'h':
				display_help();
				return 0;
				break;
			case 't':
				type = optarg;
				break;
		}
	}
	if((optind + 2) > argc)
	{
		fprintf(stderr, "usage: mount [-t type] device directory\n");
		return 1;
	}
	int ret = sea_mount_filesystem(argv[optind], argv[optind+1], type, 0, 0);
	if(ret < 0)
		fprintf(stderr, "%s: %s -> %s: %s\n", (char *)basename(argv[0]), argv[optind], argv[optind+1], strerror(errno));
	
	return ret;
}
