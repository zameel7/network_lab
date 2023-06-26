#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>

#define WINDOW_SIZE 5
#define PROBABILITY_P1 50
#define PROBABILITY_P2 10

char frameArray[10];
char ack[10];

void convertIntToString(int);

int main() {
    int socketDescriptor, frameCount, windowLength, currentFrame = 1, i = 0, progress = 0, error = 0;
    struct sockaddr_in server;

    // Create a socket
    socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);

    // Set up server address
    server.sin_family = AF_INET;
    server.sin_port = 6500;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to the server
    if (connect(socketDescriptor, (struct sockaddr *)&server, sizeof(server)) == 0) {
        printf("\nTCP Connection Established.\n");
    } else {
        printf("Connection establishment failed\n");
        return 0;
    }

    // Get the number of frames from user
    printf("\nEnter the number of Frames: ");
    scanf("%d", &frameCount);

    // Convert frame count to string and send to server
    convertIntToString(frameCount);
    send(socketDescriptor, frameArray, sizeof(frameArray), 0);

    strcpy(ack, "Time Out");

    while (1) {
        // Send frames within the window size
        for (i = 0; i < WINDOW_SIZE; i++) {
            convertIntToString(currentFrame);
            send(socketDescriptor, frameArray, sizeof(frameArray), 0);

            if (currentFrame <= frameCount) {
                printf("\nFrame %d Sent", currentFrame);
                currentFrame++;
            }
        }

        i = 0;
        windowLength = WINDOW_SIZE;

        // Receive acknowledgments and handle timeouts
        while (i < WINDOW_SIZE) {
            recv(socketDescriptor, frameArray, sizeof(frameArray), 0);
            progress = atoi(frameArray);

            if (strcmp(frameArray, ack) == 0) {
                error = currentFrame - windowLength;

                if (error < frameCount) {
                    printf("\nTime Out, Resent Frame %d onwards", error);
                }
                break;
            } else {
                if (progress <= frameCount) {
                    printf("\nFrame %s Acknowledged", frameArray);
                    windowLength--;
                } else {
                    break;
                }
            }

            if (progress > frameCount) {
                break;
            }
            i++;
        }

        // Check if all frames are acknowledged and complete the transmission
        if (windowLength == 0 && currentFrame > frameCount) {
            send(socketDescriptor, ack, sizeof(ack), 0);
            break;
        } else {
            currentFrame = currentFrame - windowLength;
            windowLength = WINDOW_SIZE;
        }
    }

    // Close the socket
    close(socketDescriptor);
    return 0;
}

// Function to convert an integer to a string
void convertIntToString(int number) {
    snprintf(frameArray, sizeof(frameArray), "%d", number);
}