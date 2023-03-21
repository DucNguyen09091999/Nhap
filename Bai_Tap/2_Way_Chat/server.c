#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>     
#include <netinet/in.h>   
#include <arpa/inet.h>
#include <unistd.h>

struct Stud {
    int roll;
    int mark;
    char name[20];

}s1[3];
int main()
{
    int serverfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(serverfd == -1)
    {
        perror("Socket creation failed\n");
        exit(0);
    }
    struct sockaddr_in server,client;
    server.sin_family = AF_INET;
    server.sin_port = htons(5000);
    server.sin_addr.s_addr = INADDR_ANY;
    int b = bind(serverfd,(struct sockaddr*)&server,sizeof(server));
    if (b == -1)
    {
        perror("bind failed\n");
        exit(0);
    }
    int l = listen(serverfd,5);
    if(l==0)
    {
        printf("Port is already to listen connection from client\n");
    }
    int size = sizeof(server);
    
    int clientfd = accept(serverfd,(struct sockaddr*)&client,&size);
    if(clientfd == -1)
    {
        perror("Accept failed\n");
        exit(0);
    }
    printf("Incoming request come from %s client\n",inet_ntoa(client.sin_addr));

    int i;

    for(i = 0; i < 3;i++)
    {
        printf("Enter record no %d\n",i+1);
        printf("Enter student roll: ");
        scanf("%d",&s1[i].roll);
        printf("Enter student mark: ");
        scanf("%d",&s1[i].mark);
    }
    int flag = 0; 
    int recv_roll;
    recv(clientfd,&recv_roll,sizeof(int),0);
    for(int i = 0 ; i < 3;i++){
        if(recv_roll = s1[i].roll){
            send(clientfd,&s1[i].mark,sizeof(int),0);
            flag = 1;
            break;  
        } 
        char msg[50] = "Roll not found in server";
        if (flag ==0)
        {
            send(clientfd,msg,strlen(msg),0);
        }
    }
}