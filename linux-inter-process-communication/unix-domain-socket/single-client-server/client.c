#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>


#define SOCKET_NAME "/tmp/DemoSocket"
#define BUFF_SIZE 128



int main(int arc, char *argv[])
{
    struct  sockaddr_un addr;
    int i;
    int ret;
    int data_socket;
    char buffer[BUFF_SIZE];


    /*Create data socket*/
    data_socket = socket(AF_UNIX,SOCK_STREAM,0);
    if(data_socket == -1)
    {
        perror("Socket");
        exit(EXIT_FAILURE);
    }

    /*
     * For  portability clear the whole structure, since some implementations have additional (non standard) fields in the structure */
    memset(&addr,0,sizeof(struct sockaddr_un));

    /* Connect socket to socket address */

    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path,SOCKET_NAME,sizeof(addr.sun_path) - 1);
    ret = connect (data_socket,(const struct sockaddr*)&addr,sizeof(struct sockaddr_un));
    if (ret == -1)
    {
        fprintf(stderr,"the server is down\n");
        exit(EXIT_FAILURE);
    };


    /* Send arguments*/
    do
    {
        printf("Enter the number to send the server: \n");
        scanf("%d",&i);
        ret = write(data_socket,&i,sizeof(int));
        if(ret == -1)
        {
            perror("write");
            break;
        }
        printf("No of bytes send = %d, data sent = %d\n",ret,i);
    } while (i);

    /*Request result*/
    memset(buffer,0,BUFF_SIZE);
    ret = read(data_socket,buffer,BUFF_SIZE);
    if (ret == -1)
    {
        perror("read");
        exit(EXIT_FAILURE);
    }
    printf("Recvd from server: %s\n",buffer);

    /*Close socket*/
    close(data_socket);
    exit(EXIT_SUCCESS);
    
}