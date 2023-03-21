//client
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <signal.h>
#define SHARED_MEM_SIZE 100
#define FILE_NAME "duc-map"
#define msg_size 20
char *data;
int fd;
void data_handler(int sign_num)
{
    printf("%s: Read data: %s\n",__FILE__,data);
}

void exit_handler(int sign_num)
{
	printf("Release\n");
    munmap(data,SHARED_MEM_SIZE);
	close(fd);
	shm_unlink(FILE_NAME);
	exit(EXIT_SUCCESS);
}


int main()
{
    int pid = getpid();
	char *mypid = (char*)malloc(sizeof(int));
    signal(SIGUSR1, data_handler);
	signal(SIGINT, exit_handler);
	/*create shared mem size*/
	fd = shm_open(FILE_NAME,O_CREAT | O_RDWR , 0666);

	if(fd < 0)
	{	
		printf("shm_open() is failed, %s.\n",strerror(errno));
		return -1;
	}
	/*declare size for smz*/
	ftruncate(fd,SHARED_MEM_SIZE);
	/*map/unmap*/
	data = (char*) mmap(0,SHARED_MEM_SIZE,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
    sprintf(data, "%d", pid);   
	printf("My pid is %d\n",getpid());
	printf("My pid is %s\n",data); 
	while(1);


	return 0;
}