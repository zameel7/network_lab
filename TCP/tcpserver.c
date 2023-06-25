// header file
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(void)
{
    // initialize variables
    int socket_desc, client_sock, client_size;
    struct sockaddr_in client_addr, server_addr;
    char client_msg[2000], server_msg[2000], temp;

    // clean buffer that had been initialized
    memset(client_msg, '\0', sizeof(client_msg));
    memset(server_msg, '\0', sizeof(server_msg));

    // create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_desc < 0)
    {
        printf("Unable to create socket\n");
        return -1;
    }
    printf("Socket created successfully\n");

    // set port and ip
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(3000);

    int bvalue = 1020;
    bvalue = bind(socket_desc, (struct sockaddr *)&server_addr, sizeof(server_addr));
    // bind to set port and ip
    if (bvalue < 0)
    {
        printf("Binding is not possible\n");
        return -1;
    }

    // listen for client
    if (listen(socket_desc, 1) < 0)
    {
        printf("Error\n");
        return -1;
    }
    
    while (1)
    {
        // accept an incoming connection
        client_size = sizeof(client_addr);
        client_sock = accept(socket_desc, (struct sockaddr *)&client_addr, &client_size);
        if (client_sock < 0)
        {
            printf("Can't accept");
            return -1;
        }
    
        // receive and send message (client <-> server)
        if (recv(client_sock, client_msg, sizeof(client_msg), 0) < 0)
        {
            printf("Couldn't receive\n");
            return -1;
        }
        printf("Message from client: %s\n", client_msg);

        // send message to server
        printf("Enter message: ");
        gets(server_msg);

        if(send(client_sock, server_msg, strlen(server_msg), 0)<0) {
            printf("Can't send\n");
            return -1;
        }
        
        memset(client_msg, '\0', sizeof(client_msg));
    }
    // close the connection
    close(client_sock);
    close(socket_desc);
    return 0;
}