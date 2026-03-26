#ifndef ROUTING_H
#define ROUTING_H

#include "utils.h"

// dijkstra shortest path from src
void dijkstra(int src, int dist[], int prev[]);

// bfs traversal from src
void bfs(int src);

// dfs traversal from src
void dfs(int src);

// print path from src to dst using prev array
void print_path(int prev[], int src, int dst);

#endif
