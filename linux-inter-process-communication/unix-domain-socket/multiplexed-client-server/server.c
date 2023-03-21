#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/select.h>

#define SOCKET_NAME "/tmp/DemoSocket"
#define BUFFER_SIZE 128
#define MAX_CLIENT_SUPPORTED 32


/*
 * An array of file descriptors which the server process
 * is maintaining in order to talk with the connected clients.
 * Master socket FD is also a member of this array
 */
int monitored_fd_set[MAX_CLIENT_SUPPORTED];
/*
 * Each connected client's immediate result is 
 * maintained in this client array
 */
int client_result[MAX_CLIENT_SUPPORTED] = {0};

/*Remove all the file FDs, if any, from the array*/
static void initiaze_monitor_fd_set()
{
    
    for(int i = 0;i < MAX_CLIENT_SUPPORTED;i++)
    {
        monitored_fd_set[i] = -1;
    }
}

/**
 * @brief Add a new FD to the monitored_fd_set array
 */

static void add_to_monitored_fd_set(int socket_fd)
{
    for(int i = 0; i < MAX_CLIENT_SUPPORTED;i++)
    {
        if(monitored_fd_set[i] != -1)
        {
            continue;
        }
        else
        {
            monitored_fd_set[i] = socket_fd;
            break;
        }
    }
}

/**
 * @brief Remove the FD from monitored_fd_set array
 */

static void remove_from_monitored_fd_set(int socket_fd)
{
    for(int i = 0; i < MAX_CLIENT_SUPPORTED;i++)
    {
        if(monitored_fd_set[i] != socket_fd)
        {
            continue;
        }
        else
        {
            monitored_fd_set[i] = -1;
            break;
        }
    }
}


/**
 * @brief Clone all the FDs in monitored_fd_set array into
 * fd set data structure
 */

static void refresh_fd_set(fd_set *fd_set_ptr)
{
    FD_ZERO(fd_set_ptr);
    for(int i = 0; i < MAX_CLIENT_SUPPORTED;i++)
    {
        if(monitored_fd_set[i] != -1)
        {
            FD_SET(monitored_fd_set[i],fd_set_ptr);
        }
    }
}

/**
 * @brief get the numerical max value among all FDs which server
 * is monitoring
 */
static int get_max_fd()
{
    int max = -1;
    for(int i = 0; i < MAX_CLIENT_SUPPORTED;i++)
    {
        if(monitored_fd_set[i] > max)
        {
            max = monitored_fd_set[i];
        }

    }
     return max;
}
int main(void)
{
    struct sockaddr_un name;

    
  #if 0 
    struct sockaddr_un {
        sa_family_t sun_family; /*AF_UNIX*/
        char sun_path[108]; /*Path Name*/
    }
  
  #endif
    int ret;
    int connection_socket;
    int data_socket;
    int result;
    int data;
    char buffer[BUFFER_SIZE];
    fd_set readfds;
    int comm_socket_fd,i;
    initiaze_monitor_fd_set();

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

    /*Bind socket to socket name*/
    /*purpose of blind() system call is that application() dictate underlying
     * operating systems the criteria of receiving tha data. here, blind() system call
     * is telling the OS that if sender process sends the data destined to socket "/tmp/DemoSocket",
     * Then such data needs to be delivered to this process me (the server process)*/
    ret = bind(connection_socket,(const struct sockaddr*)&name,sizeof(struct sockaddr_un));
    if (ret == -1)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    printf("bind() call success\n");

    /*Prepare for accepting connections. The backlog size is set 
    to 20. So while one request is being processed other requests 
    can be waiting*/

    ret = listen(connection_socket,20);
    if (ret == -1)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    /**
     * @brief Add master socket to monitored set of FDs
     */

    add_to_monitored_fd_set(connection_socket);

    /*This is the main loop for handling connections*/
    /*All server process usually run 24x7. Good servers should always up
     * and running and should never go down. Have you ever seen facebook or google page failed to load?
     */


    for(;;)
    {
        refresh_fd_set(&readfds); /*Copy the entire monitored FDs to reads*/
        /**
         * @brief Wait for incoming connection
         * 
         */
        printf("Waiting on select() system call\n");

        /**
         * @brief Call the select() system call, server process blocked here.
         * Linux OS keeps this process blocked until the connection initiation request or
         * data requests arrives on any of the file descriptor in the 'readfds' set. 
         * 
         */
        select(get_max_fd() + 1,&readfds,NULL,NULL,NULL);

        if(FD_ISSET(connection_socket,&readfds))
        {
            /**
             * @brief Data arrives on master socket only when new client connects with the server (that is 'connect' call is invoked on client side)
             * 
             */
            printf("new connection received recvd,accept the connection\n");
            data_socket = accept(connection_socket,NULL,NULL);
            if (data_socket == -1)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }
            printf("Connection accepted from the client\n");
            add_to_monitored_fd_set(data_socket);
        }
        else /* Data arrives on some other client FD*/
        {
            /**
             * @brief Find the client which has send us the data request
             * 
             */
            comm_socket_fd = -1;
            for(int i =0;i < MAX_CLIENT_SUPPORTED;i++)
            {
                if(FD_ISSET(monitored_fd_set[i],&readfds))
                {
                    comm_socket_fd = monitored_fd_set[i];
                    /* Prepare the buffer to recv the data*/
                    memset(buffer,0,BUFFER_SIZE);
                    /**
                     * @brief Wait for next data package.
                     * Server is blocked here. Waiting for the data to arrive from the client
                     * 
                     * 
                     */
                    printf("Waiting for data from the client\n");
                    ret = read(comm_socket_fd,buffer,BUFFER_SIZE);
                    if(ret == -1)
                    {
                        perror("read");
                        exit(EXIT_FAILURE);
                    }
                    /* add received summand*/
                    memcpy(&data,buffer,sizeof(int));
                    if(data == 0)
                    {
                        /* Send result*/
                        memset(buffer,0,BUFFER_SIZE);
                        sprintf(buffer,"Result = %d\n",client_result[i]);
                        printf("sending final result back to client\n");
                        ret = write(comm_socket_fd,buffer,BUFFER_SIZE);
                        if(ret == -1)
                        {
                            perror("write");
                            exit(EXIT_FAILURE);
                        }

                        /* Close socket*/
                        close(comm_socket_fd);
                        client_result[i] = 0;
                        remove_from_monitored_fd_set(comm_socket_fd);
                        continue; /* Go to select() and block*/
                    }
                        client_result[i] += data;
                }
            }
        }

    } /* Go to the select() and block*/
    /* Close the master socket*/
    close(comm_socket_fd);
    remove_from_monitored_fd_set(connection_socket);
    printf("Connection closed..\n");

    /* Server should release resources before getting terminated.
     * unlink the socket
    */
   unlink(SOCKET_NAME);
   exit(EXIT_SUCCESS);
}