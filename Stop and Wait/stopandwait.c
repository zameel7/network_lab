#include <stdio.h>
#include <stdlib.h>

typedef struct frame {
    int info, seq;
}frame;

int ack; // to mark acknowledgement
int t=5, k; // t to represent timer and k
int disconnect=0; // to disconnect the connection
frame p;
char turn = 's'; // to check if send or receive
int errorframe=1, errorack=1;

void sender();
void receiver();

int main() {
    p.info = 0;
    p.seq = 0;
    while(!disconnect) {
        sender();
        for(k=1; k<=10000000; k++) {
            receiver();
        }
    }
}

void sender() {
    static int flag=0;
    if(turn=='s') {
        if (errorack=0)
        {
            printf("Sender: sent packet with seq no.: %d\n", p.seq);
            errorframe = rand()%4;
            printf("%s\n", (errorframe==0?"Error while sending packet": ""));
            turn = 'r';
        }
        else {
            if(flag==1) printf("Sender: Received ACK for packet %d\n", ack);
            if(p.seq==5) {
                disconnect = 1;
                return;
            }
            p.info += 1;
            p.seq += 1;
            printf("Sender: sent packet with seq no.: %d\n", p.seq);
            errorframe = rand()%4;
            printf("%s\n", (errorframe==0?"Error while sending packet":""));
            turn = 'r';
            flag = 1;
        }
    }
    else {
        t--;
        printf("Sender time reducing \n");
        if(t==0) {
            turn='s';
            errorack = 0;
            t = 5;
        }
    }
}

void receiver() {
    static int frexp = 1;
    if(turn=='r') {
        if(errorframe!=0) {
            if(p.seq==frexp) {
                printf("Receiver: received packet with seq no.: %d\n", p.seq);
                ack=p.seq;
                frexp+=1;
                turn='s';
                errorack=rand()%4;
                printf("%s\n", (errorack==0?"Error while sending ACK":""));
            }
        }
        else {
            printf("Receiver: Dupicated packet with seq %d\n", frexp-1);
            ack = frexp-1;
            turn = 's';
            errorack = rand()%4;
            printf("%s\n", (errorack==0?"Error while sending ACK":""));
        }
    }
}


