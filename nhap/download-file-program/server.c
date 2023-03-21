#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>     
#include <netinet/in.h>   
#include <arpa/inet.h>
#include <unistd.h>
#include <unistd.h>
#include <fcntl.h>

#define LISTEN_BACKLOG 1
#define BUFF_SIZE 4096
#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)



void download_func(int new_socket_fd , char *argv[])
{     

    int fd1;
    char buffer[BUFF_SIZE];
    ssize_t numb_read, numb_write;
    char sendbuff[BUFF_SIZE];
    char recvbuff[BUFF_SIZE];

        memset(sendbuff, '0', BUFF_SIZE);
        memset(recvbuff, '0', BUFF_SIZE);

        fd1 = open(argv[2],O_WRONLY|O_CREAT|O_EXCL,S_IRUSR);
        if(fd1 == -1)
        {
            printf("Open failed\n");
            exit(1);
        }
        else
        {
        printf("Open successfully\n");
        }

 ssize_t total_bytes_copied = 0;
        for (;;) {
        ssize_t bytes_read = read(new_socket_fd, buffer, BUFF_SIZE);
        if (bytes_read <= 0) {
            break;
        }

        ssize_t bytes_written = write(fd1, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            printf("bytes_written != bytes_read failed\n");
            printf("bytes_read: %zd\n", bytes_read);
            printf("bytes_written: %zd\n", bytes_written);
            close(fd1);
            exit(EXIT_FAILURE);
        }

        total_bytes_copied += bytes_written;
    }



    close(fd1);

    close(new_socket_fd);
}

int main(int argc, char *argv[])
{
    int port_no, len, opt;
    int server_fd, new_socket_fd;
    struct sockaddr_in serv_addr, client_addr;
    int fd;
    char buff[32] = "";

    /* Đọc portnumber trên command line */
    if (argc < 3) {
        printf("No port provided\ncommand: ./server <port number> <ten file>\n");
        exit(EXIT_FAILURE);
    } else
    {
        port_no = atoi(argv[1]);
        
        
    }
    memset(&serv_addr, 0, sizeof(struct sockaddr_in));
    memset(&client_addr, 0, sizeof(struct sockaddr_in));
    
    
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        handle_error("socket()");
        fprintf(stderr, "ERROR on socket() : %s\n", strerror(errno));
    }
    /* Ngăn lỗi : “address already in use” */
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
        handle_error("setsockopt()");  

    /* Khởi tạo địa chỉ cho server */
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port_no);
    serv_addr.sin_addr.s_addr =  INADDR_ANY; //inet_addr("192.168.5.128"); //INADDR_ANY

    /* Gắn socket với địa chỉ server */
    if (bind(server_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        handle_error("bind()");

    /* Nghe tối đa 5 kết nối trong hàng đợi */
    if (listen(server_fd, LISTEN_BACKLOG) == -1)
        handle_error("listen()");

    /* Dùng để lấy thông tin client */
	len = sizeof(client_addr);

    while (1) {
        printf("Server is listening at port : %d \n....\n",port_no);
		new_socket_fd  = accept(server_fd, (struct sockaddr*)&client_addr, (socklen_t *)&len); 
		if (new_socket_fd == -1)
            handle_error("accept()");

		system("clear");
		
		//char temp[BUFF_SIZE];
		//inet_ntop(client_addr.sin_family, (struct sockaddr*)&client_addr, temp, sizeof(temp));
		printf("Server : got connection \n");
		        download_func(new_socket_fd,argv);
    close(server_fd);
    return 0;
    }
    
    // return 0;
}