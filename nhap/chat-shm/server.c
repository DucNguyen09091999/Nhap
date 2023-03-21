//server
#include <stdio.h>
#include<stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <signal.h>
#define SHARED_MEM_SIZE	100 /*size of shared memory object in bytes)*/
#define FILE_NAME	"duc-map"
char *data;
int shm_fd;
void exit_handler(int num)
{
	printf("Release all data\n");
	munmap(data,SHARED_MEM_SIZE);
	close(shm_fd);
	exit(EXIT_SUCCESS);
}


int main()
{
	signal(SIGINT, exit_handler);
    char chat[64];
	char *mypid = (char*)malloc(sizeof(int));
	int pid;

	shm_fd = shm_open(FILE_NAME,O_CREAT | O_RDWR , 0666);
	if(shm_fd < 0)
	{
		printf("shm_open() is failed, error: %s.\n",strerror(errno));
		return -1;

	}

	/*set size*/
	ftruncate(shm_fd,SHARED_MEM_SIZE);

	/* Map/Unmap */

	data = (char*)mmap(0,SHARED_MEM_SIZE, PROT_READ | PROT_WRITE,MAP_SHARED,shm_fd,0);

	printf("Data is: %s\n",data);
	
	while(1)
	{
		long check_pid_received = atol(data);
		if (check_pid_received == 0)
		{
			printf("Please wait for client to start\n");
			printf("check value: %ld\n",check_pid_received);
			sleep(1);
		}
	else
	{
		printf("check value: %ld\n",check_pid_received);
		strcpy(mypid,data);
		printf("my pid = %s\n",data);
		pid = atoi(mypid);
		printf("client pid = %d\n",pid);
		break;
	}
	}


	

    while(1)
    {
    printf("Please enter your data: ");
    fgets(chat,sizeof(chat),stdin);
	strcpy(data,chat);
	printf("%s: Write data	: %s \n",__FILE__,data);
    kill(pid, SIGUSR1);
    }

	return 0;
}