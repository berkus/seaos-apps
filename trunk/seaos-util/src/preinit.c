/* This program does the following:
 * runs '/bin/sh /preinit.sh'
 * fsck the root device
 * mounts the root device
 * chroots to the root device
 * calls /bin/init
 *
 *preinit.sh should load modules and prepare the system */
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>

void sigint_h(int g)
{
	
}

int main(int argc, char **argv)
{
	signal(SIGINT, sigint_h);
	setbuf(stdout, 0);
	if(argc < 3)
	{
		fprintf(stderr, "preinit called without root or init path!\n");
		exit(1);
	}
	int pid;
	if(!(pid=fork())) {
		execl("/sh", "sh", "/preinit.sh", 0);
		fprintf(stderr, "preinit: could not call /preinit.sh\n");
		exit(1);
	}
	waitpid(pid, 0, 0);
	if(strcmp(argv[1], "/")) {
		printf("%s: ", argv[1]);
		fflush(0);
		if(!(pid=fork())) {
			execl("/usr/sbin/fsck", "fsck", "-p", "-T", "-C", argv[1], 0);
			fprintf(stderr, "preinit: could not fsck %s\n", argv[1]);
			exit(1);
		}
		waitpid(pid, 0, 0);
		printf("Mounting Filesystems...\n    * %s -> %s: ", argv[1], "/");
		int ret = mount_filesystem(argv[1], "/mnt", 0);
		if(ret < 0)
		{
			printf("failed\n");
			fflush(0);
			fprintf(stderr, "preinit: could not mount root device '%s'!\n", argv[1]);
			exit(2);
		}
		if(chroot("/mnt") == -1)
		{
			printf("failed\n");
			fflush(0);
			fprintf(stderr, "preinit: could not chroot to mounted root device!\n");
			exit(3);
		}
		printf("ok\n    * devfs -> /dev: ");
		mount_filesystem(0, "/dev", "devfs");
		printf("ok\n    * procfs -> /proc: ");
		mount_filesystem(0, "/proc", "procfs");
		printf("ok\n");
	}
	printf("Starting init process '%s'\n", argv[2]);
	execl(argv[2], (char *)basename(argv[2]), 0);
	execl("/bin/sh", "sh", 0);
	execl("/sh", "sh", 0);
	fprintf(stderr, "preinit: could not start init process or fallbacks!\n");
	return 4;
}
