#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>


#define SOCKET_NAME "/tmp/DemoSocket"
#define BUFFER_SIZE 128
int main(int argc, char *argv[] )
{
    struct sockaddr_un name;

#if 0
    struct sockaddr_un {
        sa_family_t sun_family;  /*AF_UNIX*/
        char        sun_path[108]
    } 
#endif
    int ret;
    int connection_socket;
    int data_socket;
    int result;
    int data;
    char buffer[BUFFER_SIZE];

    /*In case of the programm existed inadvertently on the last run,
    remove the socket*/
    unlink(SOCKET_NAME);

    /*create master socket*/
    /*SOCK_DIAGRAM for datagram based communication*/
    connection_socket = socket(AF_UNIX,SOCK_STREAM,0);
    if(connection_socket == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    printf("Master socket created\n");

    /*Initialize*/
    memset(&name,0,sizeof(struct sockaddr_un));
    /*specify the socket cridentials*/
    name.sun_family = AF_UNIX;
    strncpy(name.sun_path,SOCKET_NAME,sizeof(name.sun_path)-1);

    /*Blind socket to socket name*/
    /*purpose of blind() system call is that application() dictate underlying
     * operating systems the criteria of receiving tha data. here, blind() system call
     * is telling the OS that if sender process sends the data destined to socket "/tmp/DemoSocket",
     * Then such data needs to be delivered to this process me (the server process)*/
    ret = bind(connection_socket,(const struct sockaddr*)&name,sizeof(struct sockaddr_un));
    if (ret == -1)
    {
        perror("blind");
        exit(EXIT_FAILURE);
    }
    printf("blind() call success\n");

    /*Prepare for accepting connections. The backlog size is set 
    to 20. So while one request is being processed other requests 
    can be waiting*/

    ret = listen(connection_socket,20);
    if (ret == -1)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    /*This is the main loop for handling connections*/
    /*All server process usually run 24x7. Good servers should always up
     * and running and should never go down. Have you ever seen facebook or google page failed to load?
     */

    for(;;)
    {
        /*Wait for incoming connection*/
        printf("Waiting on accept() system\n");
        data_socket = accept(connection_socket,NULL,NULL);
        if(data_socket == -1)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        printf("Connection accepted from the client\n");
        result = 0;
            for(;;)
            {
                /*Prepare the buffer to recv the data*/
                memset(buffer,0,BUFFER_SIZE);
                /*Wait for next data package*/
                /*Server is block here waiting for the data to arrive from client*/
                printf("Waiting data from the client\n");
                ret = read(data_socket,buffer,BUFFER_SIZE);

                if(ret == -1)
                {
                    perror("read");
                    exit(EXIT_FAILURE);
                }


                /* Add received summand*/
                memcpy(&data,buffer,sizeof(int));
                if (data == 0)
                {
                    break;
                }
                else
                {
                    result += data;
                }
            }

            /*Send result*/
            memset(buffer,0,BUFFER_SIZE);
            sprintf(buffer,"result = %d",result);
            printf("Sending final result back to client\n");
            ret = write(data_socket,buffer,BUFFER_SIZE);
            if (ret == -1)
            {
                perror("write");
                exit(EXIT_FAILURE);
            }
            /*Close socket*/
            close(data_socket);

    }

    /*Close the master socket*/
    close(connection_socket);
    printf("Connection closed\n");

    /*server should release resource before getting terminated*/
    /*Unlink the socket*/

    unlink(SOCKET_NAME);
    exit(EXIT_SUCCESS);
}