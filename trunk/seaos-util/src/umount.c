#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char **argv)
{
	int force=0;
	if(argc < 2) {
		fprintf(stderr, "usage: umount [-f] directory\n");
		return 1;
	}
	if(!strcmp(argv[1], "-f"))
		force=1;
	int ret = umount_filesystem((char *)(argv[1]), force);
	if(ret < 0)
		fprintf(stderr, "%s: %s: %s\n", (char *)basename(argv[0]), argv[1], strerror(errno));
	return ret ? 1 : 0;
}
