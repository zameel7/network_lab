#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MAXLINE 1024
#define PORT 4000

int main (void)
{
    int sockfd;
    struct sockaddr_in serveraddr , clientaddr;
    char buffer[MAXLINE], hello[20];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0)
    {
        printf("Socket cannot be created");
        return -1;
    }
    printf("Socket created succesfully\n");

    memset(&serveraddr, '\0', sizeof(serveraddr));
    memset(&clientaddr, '\0', sizeof(clientaddr));

    serveraddr.sin_family= AF_INET ;
    serveraddr.sin_port= htons(PORT);
    serveraddr.sin_addr.s_addr= INADDR_ANY ;

    if(bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr))<0)
    {
        printf("Cannot Bind");
        return -1;
    }
    printf("Binding Successful\n");

    int len, n;
    len = sizeof(clientaddr);

    n = recvfrom(sockfd, (char*) buffer,MAXLINE, MSG_WAITALL,(struct sockaddr*)&clientaddr, &len);
    buffer[n]='\0';
    printf("Message received : %s\n",buffer);
    
    printf("Enter the message: ");
    gets(hello);
    sendto(sockfd, (const char*) hello, strlen(hello), MSG_CONFIRM,(const struct sockaddr*)&clientaddr, len);
    printf("Message sent\n");

    return 0;
}