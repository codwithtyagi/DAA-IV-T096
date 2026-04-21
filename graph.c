#include <stdio.h>
#include "graph.h"

#include <string.h>


void initGraph()
{
    int i, j;
    nNodes = 0;
    for (i = 0; i < MAX_NODES; i++)
    {
        for (j = 0; j < MAX_NODES; j++)
        {
            adj[i][j] = (i == j) ? 0 : INF;
            costMat[i][j] = (i == j) ? 0 : INF;
        }
    }
    for (i = 0; i < MAX_NODES; i++)
        names[i][0] = '\0';
}

// add node, return index
int addNode(char *name)
{
    if (nNodes >= MAX_NODES)
    {
        printf("Max nodes reached!\n");
        return -1;
    }
    strncpy(names[nNodes], name, MAX_NAME - 1);
    names[nNodes][MAX_NAME - 1] = '\0';
    return nNodes++;
}

// add undirected edge
void addEdge(int src, int dst, int dist, int c)
{
    if (src < 0 || src >= nNodes || dst < 0 || dst >= nNodes)
    {
        printf("Invalid node index!\n");
        return;
    }
    adj[src][dst] = dist;
    adj[dst][src] = dist; // undirected
    costMat[src][dst] = c;
    costMat[dst][src] = c;
}

// find node
int findNode(char *name)
{
    int i;
    for (i = 0; i < nNodes; i++)
        if (strcmp(names[i], name) == 0)
            return i;
    return -1;
}

// print matrix
void printGraph()
{
    int i, j;
    if (nNodes == 0)
    {
        printf("Graph is empty.\n");
        return;
    }
    printf("\nAdjacency Matrix (Distances):\n");
    printf("%12s", "");
    for (i = 0; i < nNodes; i++)
        printf("%12s", names[i]);
    printf("\n");
    for (i = 0; i < nNodes; i++)
    {
        printf("%12s", names[i]);
        for (j = 0; j < nNodes; j++)
        {
            if (adj[i][j] == INF)
                printf("%12s", "INF");
            else
                printf("%12d", adj[i][j]);
        }
        printf("\n");
    }
}
