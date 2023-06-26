// Distance Vector Routing Protocol
#include <stdio.h>

// node to keep track of distance from one node to all other nodes
struct node
{
    unsigned dist[20];
    unsigned from[20];
} rt[10];

int main()
{
    int costmat[20][20];
    int nodes, i, j, k, count = 0;

    // accept no. of nodes
    printf("\nEnter the number of nodes:");
    scanf("%d", &nodes);

    // accept the cost matrix
    printf("\nEnter the cost matrix:\n");
    for (i = 0; i < nodes; i++)
    {
        for (j = 0; j < nodes; j++)
        {
            // accept i->j cost in costmat[i][j]
            scanf("%d", &costmat[i][j]);
            // make diagonal costs 0 (no self loop)
            costmat[i][i] = 0;
            // in node, distance from i->j is stored in rt[i].dist[j] through j is stored in rt[i].from[j]
            rt[i].dist[j] = costmat[i][j];
            rt[i].from[j] = j;
        }
    }

    do
    {
        // count used to keep track of changes, and if no changes after complete for loop iteration: it stops
        count = 0;
        for (i = 0; i < nodes; i++)
            for (j = 0; j < nodes; j++)
                for (k = 0; k < nodes; k++)
                    // if i->j distance is greater than i->k->j, update the node values in rt node struct
                    if (rt[i].dist[j] > costmat[i][k] + rt[k].dist[j])
                    {
                        // calculate minimum distance
                        rt[i].dist[j] = rt[i].dist[k] + rt[k].dist[j];
                        rt[i].from[j] = k;
                        count++;
                    }
    } while (count != 0);

    // displaying distance from each node to the other via which node
    for (i = 0; i < nodes; i++)
    {
        printf("\n\n For router %d\n", i + 1);
        for (j = 0; j < nodes; j++)
        {
            printf("\t\n Distance to node: %d via node: %d = %d", j + 1, rt[i].from[j] + 1, rt[i].dist[j]);
        }
    }
    printf("\n\n");
}
