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
    int sock_desc, server_size, n;
    char client_msg[MAXLINE], server_msg[MAXLINE];
    struct sockaddr_in server_addr;

    server_size = sizeof(server_addr);

    memset(& server_addr, '\0', sizeof(server_addr));

    sock_desc = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_desc<0) {
        printf("Unable to create a socket\n");
        return -1;
    }
    printf("Socket created succesfully\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    printf("Enter message to send: ");
    gets(client_msg);
    
    sendto(sock_desc, (const char*)client_msg, strlen(client_msg), 0, (const struct sockaddr *)&server_addr, server_size);
    printf("Message sent\n");
    
    n = recvfrom(sock_desc, (char *)server_msg, 1024, MSG_WAITALL, (struct sockaddr *)&server_addr, &server_size);
    server_msg[n] = 0;
    
    printf("Message received: %s\n", server_msg);

    return 0;
}