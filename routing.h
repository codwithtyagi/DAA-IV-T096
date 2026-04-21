#ifndef ROUTING_H
#define ROUTING_H

#include "utils.h"

void dijkstra(int src, int dist[], int prev[]);
void bfs(int src);
void dfs(int src);
void printPath(int prev[], int src, int dst);

#endif
