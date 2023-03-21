#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>     //  Chứa cấu trúc cần thiết cho socket. 
#include <netinet/in.h>     //  Thư viện chứa các hằng số, cấu trúc khi sử dụng địa chỉ trên internet
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#define BUFF_SIZE 4096
#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)
/* Chức năng chat */
void send_data_to_server_func(int server_fd, char* argv[])
{
    char buffer[BUFF_SIZE];
    int fd1;

    memset(buffer, '0', BUFF_SIZE);
    fd1 = open(argv[3],O_RDONLY);
    if(fd1 == -1)
    {
        printf("can't open source file\n");
        exit(1);
    }

        ssize_t total_bytes_passed = 0;
        for (;;) {
        ssize_t bytes_read = read(fd1, buffer, BUFF_SIZE);
        if (bytes_read <= 0) {
            break;
        }

        ssize_t bytes_written = write(server_fd, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            printf("bytes_written != bytes_read failed\n");
            printf("bytes_read: %zd\n", bytes_read);
            printf("bytes_written: %zd\n", bytes_written);
            close(fd1);
            exit(EXIT_FAILURE);
        }

        total_bytes_copied += bytes_written;
    }

     printf("File Copy Successful, %zd bytes copied\n", total_bytes_copied);
    close(fd1);

    close(server_fd); 
}

int main(int argc, char *argv[])
{
    int portno;
    int server_fd;
    struct sockaddr_in serv_addr;
 
    char buff[20];
	memset(&serv_addr, '0',sizeof(serv_addr));
	
    /* Đọc portnumber từ command line */
    if (argc < 3) {
        printf("command : ./client <server address> <port number> <file-name>\n");
        exit(1);
    }

    portno = atoi(argv[2]);
    
	
    /* Khởi tạo địa chỉ server */
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port   = htons(portno);
    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) == -1) 
        handle_error("inet_pton()");
	
    /* Tạo socket */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
        handle_error("socket()");
	
    /* Kết nối tới server*/
    if (connect(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        handle_error("connect()");
	
    send_data_to_server_func(server_fd,argv);

    return 0;
}
