#include <stdio.h>
#include <unistd.h>
int main(int argc, char **argv)
{
	execl("/bin/cat", "/bin/cat", "/proc/modules");
	return 1;
}
