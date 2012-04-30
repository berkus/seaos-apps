#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
int usage()
{
	printf("swap - Add or remove devices from the swap devices list\n");
	printf("Usage: swap [options] device-name\n");
	printf("Options:\n\tswap with no options adds a device\n\t-r: Remove device\n\t-f: Force operation\n\t-l: List swap devices\n");
	return 0;
}

int main(int argc, char **argv)
{
	int list=0, off=1, force=0;
	int i;
	int pid=0;
	for(i=1;i<argc;i++)
	{
		if(argv[i][0] == '-')
		{
			if(argv[i][1] == 'l')
				list=1;
			if(argv[i][1] == 'r')
				off=2;
			if(argv[i][1] == 'f')
				force=1;
			if(argv[i][1] == 'h')
				return usage();
			if(argv[i][1] == 'p')
			{
				pid = atoi(argv[++i]);
			}
		}
		else
			break;
	}
	char *dev = argv[i];
	
	if(list)
	{
		execl("/bin/cat", "/bin/cat", "/proc/swap");
		exit(0);
	}
	
	if(!dev && !pid) {
		if(!list)
			usage();
		return 0;
	}
	if(dev) {
		int ret;
		printf("%sing device %s to swap devices...\n", off == 2 ? "Remov" : "Add", dev);
		if(off)
			ret = syscall(61+off, (int)dev, (off == 2) ? force : 0, 0, 0, 0);
		printf("%s\n", ret == 0 ? "Success" : "Failed. Check kernel log for details.");
	}
	if(pid)
	{
		char err[5][64] = {
			"",
			"Not called as root",
			"Process does not exist",
			"Cannot swap a kernel task",
			"No swap devices"
		};
		printf("Preparing to swap task %d...", pid);fflush(0);
		int ret = syscall(96, pid, 0, 0, 0, 0);
		if(ret < 0)
			printf("Failed: %s\n", err[-ret]);
		else
			printf("Task will be swapped\n");
	}
	return 0;
}
