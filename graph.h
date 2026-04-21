#ifndef GRAPH_H
#define GRAPH_H

#include "utils.h"

int adj[MAX_NODES][MAX_NODES];
int costMat[MAX_NODES][MAX_NODES];
char names[MAX_NODES][MAX_NAME];
int nNodes;

void initGraph();
int addNode(char *name);
void addEdge(int src, int dst, int dist, int c);
void printGraph();
int findNode(char *name);

#endif
