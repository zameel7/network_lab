// Server.c

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
int main()
{

	int socket_desc, client_socket, client_size;
	struct sockaddr_in server_addr, client_addr;
	char server_message[2000], client_message[2000];

	// clear the buffer
	memset(server_message, '\0', sizeof(server_message));
	memset(client_message, '\0', sizeof(client_message));

	// create a socket
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_desc < 0)
	{
		printf("Error while creating socket\n");
		return -1;
	}
	printf("Socket created successfully\n");

	// configure the family, port and address of server and bind
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(2000);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (bind(socket_desc, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		printf("Couldn't bind to the port\n");
		return -1;
	}
	printf("Done with binding\n");

	// listen to connections from the client
	if (listen(socket_desc, 0) < 0)
	{
		printf("Cannot listen");
		return -1;
	}

	// accept the clients connection
	client_size = sizeof(client_addr);
	client_socket = accept(socket_desc, (struct sockaddr *)&client_addr, &client_size);

	if (client_socket < 0)
	{
		printf("Can't accept\n");
		return -1;
	}

	printf("Client connected at IP:%s and port:%i\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
	
	// receive message from the client
	if (recv(client_socket, client_message, sizeof(client_message), 0) < 0)
	{
		printf("Couldn't receive\n");
		return -1;
	}
	// display the file name received from client in client_message
	printf("%s\n", client_message);

	// open the file if valid and send its contents or else display and send error message
	FILE *fileptr = fopen(client_message, "r");
	if (fileptr == NULL)
	{
		strcpy(server_message, "NOT A VALID FILE");
		if (send(client_socket, server_message, strlen(server_message), 0) < 0)
		{
			printf("Couldn't send the error message\n");
			return -1;
		}
		printf("File not send as it is not valid\n");
		return -1;
	}

	int i = 0;
	while (!feof(fileptr))
	{
		// store the contents of file in server_message array
		server_message[i++] = fgetc(fileptr);
	}

	server_message[i] = '\0';
	printf("\n%s", server_message);

	// send the file contents
	if (send(client_socket, server_message, strlen(server_message), 0) < 0)
	{
		printf("Couldn't send the file\n");
		return -1;
	}

	fclose(fileptr);
	close(client_socket);
	close(socket_desc);
	return 0;
}
