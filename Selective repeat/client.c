#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <ctype.h>
#include <arpa/inet.h>

#define WINDOW_SIZE 5
#define TIMEOUT_PROBABILITY 50
#define NAK_DELAY 10

char client_message[10]; // Buffer to store the client message
char acknowledgement[10]; // Buffer to store acknowledgements

void convertIntToString(int);

int main() {
    struct sockaddr_in server_addr;
    int socket_desc, n, i, j, frameCounter = 1, totalFrames, errorFrame, nakDelay = 0, windowLength;

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(3000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(socket_desc, (struct sockaddr *)&server_addr, sizeof(server_addr));

    printf("\nTCP Connection Established\n");

    printf("Enter the number of frames: ");
    scanf("%d", &totalFrames);

    convertIntToString(totalFrames);
    send(socket_desc, client_message, sizeof(client_message), 0);
    strcpy(acknowledgement, "Time Out");

    while (1) {
        for (i = 0; i < WINDOW_SIZE; i++) {
            convertIntToString(frameCounter);
            send(socket_desc, client_message, sizeof(client_message), 0);
            if (frameCounter <= totalFrames) {
                printf("\nFrame %d Sent", frameCounter);
                frameCounter++;
            }
        }

        i = 0;
        windowLength = WINDOW_SIZE;
        while (i < WINDOW_SIZE) {
            recv(socket_desc, client_message, sizeof(client_message), 0);
            errorFrame = atoi(client_message + 1);
            if (client_message[0] == 'N') {
                nakDelay = NAK_DELAY;
                if (errorFrame < totalFrames) {
                    printf("\nNAK %d", errorFrame);
                    printf("\nFrame %d sent", errorFrame);
                    i--;
                }
            } else {
                if (errorFrame <= totalFrames) {
                    printf("\nFrame %s Acknowledged", client_message);
                    windowLength--;
                } else {
                    break;
                }
            }
            if (errorFrame > totalFrames) {
                break;
            }
            i++;
        }

        if (windowLength == 0 && frameCounter > totalFrames) {
            send(socket_desc, acknowledgement, sizeof(acknowledgement), 0);
            break;
        } else {
            frameCounter = frameCounter - windowLength;
            windowLength = WINDOW_SIZE;
        }
    }

    close(socket_desc);
    return 0;
}

// Function to convert an integer to a string and store it in 'client_message'
void convertIntToString(int num) {
    sprintf(client_message, "%d", num);
}