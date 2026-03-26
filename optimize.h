#ifndef OPTIMIZE_H
#define OPTIMIZE_H

#include "utils.h"

// 0/1 knapsack, prints result to console
int knapsack(int cap, int wt[], int val[], int n);

// tsp brute force on current graph, prints result to console
void tsp(int start);

#endif
