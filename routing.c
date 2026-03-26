#include "routing.h"
#include "graph.h"
#include <stdio.h>

// dijkstra - find shortest path from src
void dijkstra(int src, int dist[], int prev[])
{
    int vis[MAX_NODES] = {0};
    int i, j, u, min;

    for (i = 0; i < n_nodes; i++)
    {
        dist[i] = INF;
        prev[i] = -1;
    }
    dist[src] = 0;

    for (i = 0; i < n_nodes; i++)
    {
        // find min dist unvisited
        min = INF;
        u = -1;
        for (j = 0; j < n_nodes; j++)
        {
            if (!vis[j] && dist[j] < min)
            {
                min = dist[j];
                u = j;
            }
        }
        if (u == -1) break; // no more reachable
        vis[u] = 1;

        // relax neighbors
        for (j = 0; j < n_nodes; j++)
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

// print path using prev array
void print_path(int prev[], int src, int dst)
{
    int path[MAX_NODES], cnt = 0;
    int cur = dst;

    if (prev[dst] == -1 && src != dst)
    {
        printf("No path from %s to %s\n", names[src], names[dst]);
        return;
    }

    // trace back
    while (cur != -1)
    {
        path[cnt++] = cur;
        cur = prev[cur];
    }

    // print forward
    int i;
    for (i = cnt - 1; i >= 0; i--)
    {
        printf("%s%s", names[path[i]], (i > 0) ? " -> " : "");
    }
    printf("\n");
}

// bfs traversal
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

        for (i = 0; i < n_nodes; i++)
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

// dfs helper
void dfs_visit(int u, int vis[])
{
    vis[u] = 1;
    printf("%s ", names[u]);

    int i;
    for (i = 0; i < n_nodes; i++)
    {
        if (!vis[i] && adj[u][i] != INF && adj[u][i] != 0)
            dfs_visit(i, vis);
    }
}

// dfs traversal
void dfs(int src)
{
    int vis[MAX_NODES] = {0};
    printf("DFS: ");
    dfs_visit(src, vis);
    printf("\n");
}
