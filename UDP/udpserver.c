#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define MAXLINE 1024
#define PORT 4000

int main(void) {
    int sock_desc, client_size, n;
    char client_msg[MAXLINE], server_msg[MAXLINE];
    struct sockaddr_in client_addr, server_addr;

    client_size = sizeof(client_addr);

    memset(&client_addr, '\0', sizeof(client_addr));
    memset(& server_addr, '\0', sizeof(server_addr));

    sock_desc = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_desc<0) {
        printf("Unable to create a socket\n");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(sock_desc, (struct sockaddr *)&server_addr, sizeof(server_addr))<0)
    {
        printf("Couldn't socket and server\n");
        return -1;
    }
    printf("Binding Successful\n");
    
    n = recvfrom(sock_desc, (char *)client_msg, 1024, MSG_WAITALL, (struct sockaddr *)&client_addr, &client_size);
    client_msg[n] = '\0';
    
    printf("Message received: %s\n", client_msg);

    printf("Enter message to send: ");
    gets(server_msg);

    sendto(sock_desc, (const char*)server_msg, strlen(server_msg), 0, (const struct sockaddr *)&client_addr, client_size);
    printf("Message sent\n");

    return 0;
}