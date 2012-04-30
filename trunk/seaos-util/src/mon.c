#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
void clearline()
{
	printf("                                                                               \n");
}

int main(int argc, char **argv)
{
	int i;
	int iter=0, iter1=0;
	int knowne=0;
	struct mem_stat ss, *s;s = &ss;
	ioctl(0, 3, 0);
	ioctl(0, 4, 0);
	ioctl(0, 26, 1);
	top:
	mem_stat(s);
	ioctl(0, 3, 0);
	ioctl(0, 4, 0);
	unsigned kml = s->km_end - s->km_loc;
	printf("Phys Memory: Used %d / %d KB (%d KB Free). Usage is %3.2f%% \n", (s->used)/1024, s->total/1024, s->free/1024, s->perc);
	printf("Kernel Malloc Memory: %x -> %x\nNumber of index pages: %d. Used list nodes: %d / %d (%3.2f%%). \n", s->km_loc, s->km_end, s->km_numindex, s->km_usednodes, s->km_maxnodes, (float)100 * (float)s->km_usednodes/(float)s->km_maxnodes);
	printf("Number of slabs: %d. Used pages: %d / %d (%3.2f%%) \n", s->km_numslab, s->km_pagesused, kml/0x1000, (float)100 * (float)s->km_pagesused/(float)(kml/0x1000));
	printf("Used slab caches: %d / %d \n", s->km_usedscache, s->km_maxscache);
	if(!(iter1 % 1000))
	{
		ioctl(0, 3, 0);
		ioctl(0, 4, 0);
		clearline();
		clearline();
		clearline();
		clearline();
	}
	
	if(!(iter/50 % 4))
		printf("|\n");
	if((iter/50 % 4) == 1)
		printf("/\n");
	if((iter/50 % 4) == 2)
		printf("-\n");
	if((iter/50 % 4) == 3)
		printf("\\\n");
	
	iter++;
	iter1++;
	syscall(0,0,0,0,0,0);
	goto top;
}
