#include "routing.h"
#include "graph.h"
#include <stdio.h>

// dijkstra
void dijkstra(int src, int dist[], int prev[])
{
    int vis[MAX_NODES] = {0};
    int i, j, u, min;

    for (i = 0; i < nNodes; i++)
    {
        dist[i] = INF;
        prev[i] = -1;
    }
    dist[src] = 0;

    for (i = 0; i < nNodes; i++)
    {
        // find min unvisited
        min = INF;
        u = -1;
        for (j = 0; j < nNodes; j++)
        {
            if (!vis[j] && dist[j] < min)
            {
                min = dist[j];
                u = j;
            }
        }
        if (u == -1) break; // no more reachable
        vis[u] = 1;

        // relax
        for (j = 0; j < nNodes; j++)
        {
            if (!vis[j] && adj[u][j] != INF)
            {
                if (dist[u] + adj[u][j] < dist[j])
                {
                    dist[j] = dist[u] + adj[u][j];
                    prev[j] = u;
                }
            }
        }
    }
}

// print path
void printPath(int prev[], int src, int dst)
{
    int path[MAX_NODES], cnt = 0;
    int cur = dst;

    if (prev[dst] == -1 && src != dst)
    {
        printf("No path from %s to %s\n", names[src], names[dst]);
        return;
    }

    // traceback
    while (cur != -1)
    {
        path[cnt++] = cur;
        cur = prev[cur];
    }

    // print
    int i;
    for (i = cnt - 1; i >= 0; i--)
    {
        printf("%s%s", names[path[i]], (i > 0) ? " -> " : "");
    }
    printf("\n");
}

// bfs
void bfs(int src)
{
    int vis[MAX_NODES] = {0};
    int q[MAX_NODES], front = 0, rear = 0;
    int u, i;

    printf("BFS: ");

    vis[src] = 1;
    q[rear++] = src;

    while (front < rear)
    {
        u = q[front++];
        printf("%s ", names[u]);

        for (i = 0; i < nNodes; i++)
        {
            if (!vis[i] && adj[u][i] != INF && adj[u][i] != 0)
            {
                vis[i] = 1;
                q[rear++] = i;
            }
        }
    }
    printf("\n");
}

// dfs visit
void dfsVisit(int u, int vis[])
{
    vis[u] = 1;
    printf("%s ", names[u]);

    int i;
    for (i = 0; i < nNodes; i++)
    {
        if (!vis[i] && adj[u][i] != INF && adj[u][i] != 0)
            dfsVisit(i, vis);
    }
}

// dfs
void dfs(int src)
{
    int vis[MAX_NODES] = {0};
    printf("DFS: ");
    dfsVisit(src, vis);
    printf("\n");
}
