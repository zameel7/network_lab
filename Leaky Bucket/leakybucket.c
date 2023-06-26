// Leaky Bucket

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NOF_PACKETS 10

int rnd(int a)
{
    // generate random no. less than "a"
    int rn = random() % a;
    // return 1 if rn==0 else return rn itself
    return rn == 0 ? 1 : rn;
}

int main()
{

    int packet_sz[NOF_PACKETS], i, clk, b_size, o_rate,

    p_sz_rm = 0, p_sz, p_time, op;

    // assign random no. of byte size to packets
    for (i = 0; i < NOF_PACKETS; ++i)
    {
        packet_sz[i] = rnd(6) * 10;
    }

    // display packets sizes
    for (i = 0; i < NOF_PACKETS; ++i)
        printf("\npacket[%d]:%d bytes\t", i, packet_sz[i]);

    // accept output rate and bucket size
    printf("\nEnter the Output rate:");
    scanf("%d", &o_rate);

    printf("Enter the Bucket Size:");
    scanf("%d", &b_size);

    for (i = 0; i < NOF_PACKETS; ++i)
    {
        // if packet_size + packet_size_remaining is greater than the bucket size
        if (packet_sz[i]+p_sz_rm > b_size)
        {
            if (packet_sz[i] > b_size) /*compare the packet siz with bucket size*/
                    printf("\n\nIncoming packet size (%d bytes) is Greater than bucket capacity (%d bytes)-PACKET REJECTED", packet_sz[i], b_size);
            else {
                printf("\n\nBucket capacity exceeded--Packet rejected\n");
            }
        // if packet size + remaining is less than the bucket size
        }
        else
        {
            p_sz_rm += packet_sz[i];
            printf("\n\nIncoming Packet size: %d", packet_sz[i]);

            printf("\nBytes remaining to Transmit: %d", p_sz_rm);

            // generate random process time < 4s
            p_time = rnd(4) * 10;
            printf("\nTime left for transmission: %d units", p_time);

            for (clk = 10; clk <= p_time; clk += 10)
            {
                sleep(1);

                if (p_sz_rm)
                {
                    // if rem. size is less than output rate, empty remaining and move all to output
                    if (p_sz_rm <= o_rate) /*packet size remaining comparing with output rate*/
                    {
                        op = p_sz_rm;
                        p_sz_rm = 0;
                    }
                    // else move size(output_rate) units to output from remaining
                    else
                    {
                        op = o_rate;
                        p_sz_rm -= o_rate;
                    }
                    printf("\nPacket of size %d Transmitted", op);

                    printf("----Bytes Remaining to Transmit:%d", p_sz_rm);
                }
                // if remaining is empty, show no packets to transmit and remaining time left
                else
                {
                    printf("\nTime left for transmission: %d units", p_time - clk);
                    printf("\nNo packets to transmit!!");
                    break;
                }
            }
        }
    }
}