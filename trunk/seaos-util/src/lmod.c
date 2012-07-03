#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <errno.h>

int main(int argc, char **argv)
{
	if(argc < 2)
	{
		fprintf(stderr, "usage: lmod path\n");
		return 1;
	}
	int err = 0;
	int i;
	for(i=1;i<argc;i++) {
		int ret = load_module(argv[i], 0);
		if(ret < 0)
			fprintf(stderr, "lmod: failed to load module '%s': ", argv[i]), err=1;
		switch(ret)
		{
			case -EINVAL:
				fprintf(stderr, "invalid module\n");
				break;
			case -ENOEXEC:
				fprintf(stderr, "could not load dependencies, or invalid ELF file\n");
				break;
			case -ENOENT:
				fprintf(stderr, "file not found\n");
				break;
			case -EEXIST:
				fprintf(stderr, "module already loaded\n");
				break;
		}
		if(ret > 0) {
			fprintf(stderr, "lmod: %s: module initialization returned %d (%s)\n", argv[i], ret, strerror(ret));
		}
	}
	return err ? 1 : 0;
}
