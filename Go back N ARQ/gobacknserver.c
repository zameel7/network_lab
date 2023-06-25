// 1. Start

// 2. Import all the necessary commands
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// Macros
#define WINDOW_SIZE 5
#define PROBABILITY_P1 50
#define PROBABILITY_P2 10

// Strings to store and compare frameArray
char ack[10];
char frameArray[10];

// Function prototype
void intToAlpha(int);

int main() {
    struct sockaddr_in server, client;
    int socket_descriptor, client_size, accepted_connection, frameArray_code, frameArray_code_cmp = 1, i, random_val;

    // 3. Create a socket that returns a socket descriptor
    socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);

    // 4. Set port and IP of the receiver
    server.sin_port = 6500;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;

    // 5. Initialize socket structure and bind using bind(descriptor, pointer to socket address, length in byte of the address)
    bind(socket_descriptor, (struct sockaddr *)&server, sizeof(server));

    // 6. Listen for the connection with listen(descriptor, no. of ack that could be WINDOW_SIZEed)
    listen(socket_descriptor, 1);

    // 7. Accept the connection with the accept(descriptor, pointer to socket address, actual length of address)
    client_size = sizeof(client);
    accepted_connection = accept(socket_descriptor, (struct sockaddr *)&client, &client_size);
    printf("\nTCP connection established\n");

    // 8. Receive packet and send ack or if lost, sender will resend
    strcpy(ack, "Time Out");
    recv(accepted_connection, frameArray, sizeof(frameArray), 0);
    frameArray_code = atoi(frameArray);
    while (1) {
        for (i = 0; i < WINDOW_SIZE; i++) {
            recv(accepted_connection, frameArray, sizeof(frameArray), 0);
            if (strcmp(ack, frameArray) == 0) {
                break;
            }
        }
        i = 0;

        // 9. Silence of receiver causes the timer of unacknowledged frame to expire
        while (i < WINDOW_SIZE) {
            random_val = rand() % PROBABILITY_P1;
            if (random_val < PROBABILITY_P2) {
                send(accepted_connection, ack, sizeof(ack), 0);
                break;
            } else {
                intToAlpha(frameArray_code_cmp);
                if (frameArray_code_cmp <= frameArray_code) {
                    printf("\nFrame %s received", frameArray);
                    send(accepted_connection, frameArray, sizeof(frameArray), 0);
                } else {
                    break;
                }
                frameArray_code_cmp++;
            }
            if (frameArray_code_cmp > frameArray_code) {
                break;
            }
            i++;
        }

        if (frameArray_code_cmp > frameArray_code) {
            break;  // Break out of the outer while loop if frameArray_code_cmp exceeds frameArray_code
        }
    }

    // 10. Close the connections
    close(accepted_connection);
    close(socket_descriptor);
    return 0;
}

// Function to convert an integer to string, e.g., 12345 to "12345"
void intToAlpha(int frameArray_code_cmp) {
    snprintf(frameArray, sizeof(frameArray), "%d", frameArray_code_cmp);
}
