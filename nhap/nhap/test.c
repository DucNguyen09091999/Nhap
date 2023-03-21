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
//    read_numb = read(fd,buff,strlen(buff));
//    if(read_numb == -1)
//    {
//     printf("Read failed\n");
//    }
//    else
//    {
//     printf("Read successfully\n");
//    }
//     close(fd);

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
//     write_numb = write(fd1,buff1,strlen(buff1));


//     if(write_numb == -1)
//    {
//     printf("write failed\n");
//    }
//    else
//    {
//     printf("write successfully\n");
//    }
//     close(fd1);
 

    ssize_t n;
    while(n=read(fd,buff,50)){
        write(fd1,buff,n);
        printf("..writing..\n");
    }
    close(fd);
    close(fd1);

    return 0;
    



}