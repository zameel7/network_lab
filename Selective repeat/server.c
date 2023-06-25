#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <ctype.h>
#include <arpa/inet.h>
#define W 5
#define P1 50
#define P2 10


char client_message[10];
char b[10];
void alpha9(int);
void alp(int);


int main(){

    struct sockaddr_in server_addr, client_addr;
    int socket_desc, client_sock, n, i, j, c = 1, f;
    unsigned int s1;

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(3000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    bind(socket_desc, (struct sockaddr *)&server_addr, sizeof(server_addr));

    listen(socket_desc, 1);

    n = sizeof(client_addr);
    client_sock = accept(socket_desc, (struct sockaddr *)&client_addr, &n);

    printf("\nTCP Connection Establised\n");

    s1 = (unsigned int)time(NULL);
    srand(s1);
    strcpy(b, "Time Out");
    
    recv(client_sock, client_message, sizeof(client_sock), 0);
    f = atoi(client_message);

    while(1){
        for(i = 0; i < W; i++){
            recv(client_sock, client_message, sizeof(client_message), 0);
            if(strcmp(client_message, b) == 0){
                break;
            }
        }
        i = 0;
        while(i < W){
            L:
                j = rand() % P1;
            if(j < P2){
                alp(c);
                send(client_sock, b, sizeof(b), 0);
                goto L;
            }
            else{
                alpha9(c);
                if (c <= f){
                    printf("\nFrame %s Received", client_message);
                    send(client_sock, client_message, sizeof(client_message), 0);
                }
                else{
                    break;
                }
                c++;
            }
            if (c > f){
                break;
            }
            i++;
        }
    }
    close(client_sock);
    close(socket_desc);
    return 0;
}

void alpha9(int z){
    int i = 0, j, k, g;
    k = z;
    while(k > 0){
        i++;
        k = k / 10;
    }
    g = i;
    i--;
    while(z > 0){
        k = z % 10;
        client_message[i] = k + 48;
        i--;
        z = z / 10;
    }
    client_message[g] = '\0';
}

void alp(int z){
    int k, i = 1, j, g;
    k = z;
    b[0] = 'N';
    while(k > 0){
        i++;
        k = k/10;
    }
    g= i;
    i --;
    while(z > 0){
        k = z % 10;
        b[i] = k + 48;
        i --;
        z=  z / 10;
    }
    b[g] = '\0';
}