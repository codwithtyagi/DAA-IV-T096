#ifndef GRAPH_H
#define GRAPH_H

#include "utils.h"

// adjacency matrix for distances
int adj[MAX_NODES][MAX_NODES];
// cost matrix
int cost_mat[MAX_NODES][MAX_NODES];
// node names
char names[MAX_NODES][MAX_NAME];
// node count
int n_nodes;

// init graph to empty
void init_graph();

// add a node, returns index
int add_node(char *name);

// add edge with dist and cost
void add_edge(int src, int dst, int dist, int c);

// print adj matrix
void print_graph();

// get node index by name, -1 if not found
int find_node(char *name);

#endif
