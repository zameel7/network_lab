#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MAXLINE 1024
#define PORT 4000

int main(void)
{
    int sockfd;
    struct sockaddr_in serveraddr;
    char buffer[MAXLINE], hello[20];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        printf("Socket cannot be created");
        return -1;
    }
    printf("Socket created succesfully\n");

    memset(&serveraddr, '\0', sizeof(serveraddr));

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    int len, n;
    len = sizeof(serveraddr);

    printf("Enter the message: ");
    gets(hello);
    
    sendto(sockfd, (const char *)hello, strlen(hello), MSG_CONFIRM, (const struct sockaddr *) &serveraddr, len);
    printf("Message sent\n");
    
    n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&serveraddr, &len);
    buffer[n] = 0;
    printf("Message received : %s\n", buffer);
    
    return 0;
}