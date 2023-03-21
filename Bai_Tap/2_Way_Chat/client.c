#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>     //  Chứa cấu trúc cần thiết cho socket. 
#include <netinet/in.h>     //  Thư viện chứa các hằng số, cấu trúc khi sử dụng địa chỉ trên internet
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>


int main(void)
{
    int clientfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(clientfd == -1)
    {
        perror("Socket creation failed\n");
        exit(0);
    }
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(5000);
    server.sin_addr.s_addr = INADDR_ANY;
    int c = connect(clientfd,(struct sockaddr*)&server,sizeof(server));
    if (c == -1)
    {
        perror("connection failed\n");
        exit(0);
    }
    printf("connected to server\n");
    int roll;
    printf("Enter roll to research mark: ");
    scanf("%d",&roll);
    send(clientfd,&roll,sizeof(int),0);
    printf("Roll sent to server\n");
    int mark;
    char msg[40];
    int k = recv(clientfd,&mark,sizeof(int),0);
    if(k == -1){
        recv(clientfd,msg,sizeof(msg),0);
        printf("%s\n",msg);
    }
    printf("Student mark is: %d\n",mark); 
    return 0;
}

