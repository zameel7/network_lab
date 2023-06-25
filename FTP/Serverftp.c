//Server.c

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
int main()
{

	int socketfd,client_socket,client_size;
	struct sockaddr_in server_addr,client_addr;
	char server_message[2000],client_message[2000];

	memset(server_message,'\0',sizeof(server_message));
	memset(client_message,'\0',sizeof(client_message));

	socketfd=socket(AF_INET,SOCK_STREAM,0);

		if(socketfd<0)
		{
			printf("Error while creating socket\n");
			return -1;
		}

printf("Socket created successfully\n");

server_addr.sin_family=AF_INET;
server_addr.sin_port=htons(2000);
server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

	if(bind(socketfd,(struct sockaddr*)&server_addr,sizeof(server_addr))<0)
	{
		printf("Couldn't bind to the port\n");
		return -1;
	}
printf("Done with binding\n");

		if(listen(socketfd,0)<0)
		{
		printf("Cannot listen");
		return -1;
		}


//printf("\nWaiting for incoming connections\n");
client_size=sizeof(client_addr);
client_socket=accept(socketfd,(struct sockaddr*)&client_addr,&client_size);

if(client_socket<0)
{
printf("Can't accept\n");
return -1;
}


printf("Client connected at IP:%s and port:%i\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));

if(recv(client_socket,client_message,sizeof(client_message),0)<0)
{
printf("Couldn't receive\n");
return -1;
}


printf("%s",client_message);
FILE* fileptr=fopen(client_message,"r");
		if(fileptr==NULL)
		{
			strcpy(server_message,"NOT A VALID FILE");
			if(send(client_socket,server_message,strlen(server_message),0)<0)
			{
				printf("Cant send\n");
				return -1;
			}
			return -1;
		}

int i=0;
while(!feof(fileptr))
{
server_message[i++]=fgetc(fileptr);
}

server_message[i]='\0';
printf("%s",server_message);

		if(send(client_socket,server_message,strlen(server_message),0)<0)
		{
			printf("Cant send\n");
			return -1;
		}


fclose(fileptr);
close(client_socket);
close(socketfd);
return 0;
}






