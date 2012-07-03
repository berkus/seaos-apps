#include <stdio.h>
#include <errno.h>
int main(int argc, char **argv)
{
	if(argc < 2)
	{
		fprintf(stderr, "usage: dmod module-name\n");
		return 1;
	}
	int ret = sea_unload_module(argv[1], 0);
	if(ret)
		fprintf(stderr, "dmod: could not unload module '%s': ", argv[1]);
	switch(ret)
	{
		case -EINVAL:
			fprintf(stderr, "dependancy error\n");
			break;
		case -ENOENT:
			fprintf(stderr, "module not found\n");
			break;
	}
	return ret ? 1 : 0;
}
