#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
int main(int argc, char*argv[])
{
    int fd,fd1;
    char *buff = (char*) calloc(100,sizeof(char));
    char *buff1 = (char*) calloc(100,sizeof(char));
    ssize_t read_numb;
    ssize_t write_numb;
    if(argc <2)
    {
        printf("usage: ./file <name-file>\n");
    }



   fd = open(argv[1],O_RDONLY);
   if(fd == -1)
   {
    printf("Open failed\n");
    return -1;
   }
   else
   {
    printf("Open successfully\n");
   }


    fd1 = open("hello.txt",O_WRONLY|O_CREAT|O_EXCL,S_IRUSR);
        if(fd == -1)
        {
        printf("Open failed\n");
        return -1;
        }
        else
        {
        printf("Open successfully\n");
        }

 

    ssize_t n;
    while(n=read(fd,buff,50)){
        write(fd1,buff,n);
        printf("..writing..\n");

        if(fd != EOF)
        { printf("\n There was a problem"); }
        else
        {  
        printf("\n Program has reached End-of-file");}
    }
    close(fd);
    close(fd1);

    return 0;
    



}