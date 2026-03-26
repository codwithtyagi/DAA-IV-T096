#include <stdio.h>
#include "graph.h"

#include <string.h>

// init graph
void init_graph()
{
    int i, j;
    n_nodes = 0;
    for (i = 0; i < MAX_NODES; i++)
    {
        for (j = 0; j < MAX_NODES; j++)
        {
            adj[i][j] = (i == j) ? 0 : INF;
            cost_mat[i][j] = (i == j) ? 0 : INF;
        }
    }
    for (i = 0; i < MAX_NODES; i++)
        names[i][0] = '\0';
}

// add node
int add_node(char *name)
{
    if (n_nodes >= MAX_NODES)
    {
        printf("Max nodes reached!\n");
        return -1;
    }
    strncpy(names[n_nodes], name, MAX_NAME - 1);
    names[n_nodes][MAX_NAME - 1] = '\0';
    return n_nodes++;
}

// add edge
void add_edge(int src, int dst, int dist, int c)
{
    if (src < 0 || src >= n_nodes || dst < 0 || dst >= n_nodes)
    {
        printf("Invalid node index!\n");
        return;
    }
    adj[src][dst] = dist;
    adj[dst][src] = dist; // undirected
    cost_mat[src][dst] = c;
    cost_mat[dst][src] = c;
}

// find node by name
int find_node(char *name)
{
    int i;
    for (i = 0; i < n_nodes; i++)
        if (strcmp(names[i], name) == 0)
            return i;
    return -1;
}

// print adj matrix
void print_graph()
{
    int i, j;
    if (n_nodes == 0)
    {
        printf("Graph is empty.\n");
        return;
    }
    printf("\nAdjacency Matrix (Distances):\n");
    printf("%12s", "");
    for (i = 0; i < n_nodes; i++)
        printf("%12s", names[i]);
    printf("\n");
    for (i = 0; i < n_nodes; i++)
    {
        printf("%12s", names[i]);
        for (j = 0; j < n_nodes; j++)
        {
            if (adj[i][j] == INF)
                printf("%12s", "INF");
            else
                printf("%12d", adj[i][j]);
        }
        printf("\n");
    }
}
