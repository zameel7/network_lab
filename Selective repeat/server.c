#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <unistd.h>

#define WINDOW_SIZE 5
#define TIMEOUT_PROBABILITY 50
#define FRAME_LOSS_PROBABILITY 10

char client_message[10];  // Buffer to store the client message
char acknowledgement[10]; // Buffer used for sending acknowledgements

void convertIntToString(int num) {
    sprintf(client_message, "%d", num);
}

void generateAcknowledgement(int num) {
    sprintf(acknowledgement, "N%d", num);
}

int main() {
    struct sockaddr_in server_addr, client_addr;
    int server_socket, client_socket, client_addr_length, i, j, frameCounter = 1, totalFrames;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(3000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));

    listen(server_socket, 1);

    client_addr_length = sizeof(client_addr);
    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_length);

    printf("\nTCP Connection Established\n");

    strcpy(acknowledgement, "Time Out");

    recv(client_socket, client_message, sizeof(client_socket), 0);
    totalFrames = atoi(client_message);  // Get the total number of frames from the client

    while (1) {
        // Receive 'WINDOW_SIZE' number of frames from the client
        for (i = 0; i < WINDOW_SIZE; i++) {
            recv(client_socket, client_message, sizeof(client_message), 0);
            if (strcmp(client_message, acknowledgement) == 0) {
                break;
            }
        }

        i = 0;
        while (i < WINDOW_SIZE) {
            L:
            j = rand() % TIMEOUT_PROBABILITY;
            if (j < FRAME_LOSS_PROBABILITY) {
                generateAcknowledgement(frameCounter);
                send(client_socket, acknowledgement, sizeof(acknowledgement), 0);
                goto L;
            } else {
                convertIntToString(frameCounter);
                if (frameCounter <= totalFrames) {
                    printf("\nFrame %s Received", client_message);
                    send(client_socket, client_message, sizeof(client_message), 0);
                } else {
                    break;
                }
                frameCounter++;
            }
            if (frameCounter > totalFrames) {
                break;
            }
            i++;
        }
    }

    close(client_socket);
    close(server_socket);
    return 0;
}
