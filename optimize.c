#include "optimize.h"
#include "graph.h"
#include <stdio.h>

// 0/1 knapsack dp
int knapsack(int cap, int wt[], int val[], int n)
{
    int dp[MAX_ORDERS + 1][1001]; // max cap 1000
    int i, w;

    if (cap > 1000) cap = 1000;

    // fill dp table
    for (i = 0; i <= n; i++)
    {
        for (w = 0; w <= cap; w++)
        {
            if (i == 0 || w == 0)
                dp[i][w] = 0;
            else if (wt[i - 1] <= w)
            {
                int inc = dp[i - 1][w - wt[i - 1]] + val[i - 1];
                int exc = dp[i - 1][w];
                dp[i][w] = (inc > exc) ? inc : exc;
            }
            else
                dp[i][w] = dp[i - 1][w];
        }
    }

    printf("Knapsack Result (Capacity=%d):\n", cap);
    printf("Max Value: %d\n", dp[n][cap]);

    // trace selected items
    printf("Selected items (index): ");
    w = cap;
    for (i = n; i > 0; i--)
    {
        if (dp[i][w] != dp[i - 1][w])
        {
            printf("%d ", i - 1);
            w -= wt[i - 1];
        }
    }
    printf("\n");

    return dp[n][cap];
}

// swap ints
void swap_int(int *a, int *b)
{
    int t = *a; *a = *b; *b = t;
}

// generate next permutation, returns 0 if last
int next_perm(int arr[], int n)
{
    int i = n - 2, j;
    while (i >= 0 && arr[i] >= arr[i + 1]) i--;
    if (i < 0) return 0;
    j = n - 1;
    while (arr[j] <= arr[i]) j--;
    swap_int(&arr[i], &arr[j]);
    // reverse from i+1 to end
    int l = i + 1, r = n - 1;
    while (l < r)
    {
        swap_int(&arr[l], &arr[r]);
        l++;
        r--;
    }
    return 1;
}

// tsp brute force
void tsp(int start)
{
    int perm[MAX_NODES], best_path[MAX_NODES];
    int i, cnt = 0, dist, best = INF;

    if (n_nodes < 2)
    {
        printf("Need at least 2 nodes for TSP.\n");
        return;
    }

    // build perm of non-start nodes
    for (i = 0; i < n_nodes; i++)
    {
        if (i != start)
            perm[cnt++] = i;
    }

    // try all permutations
    do
    {
        dist = 0;
        int prev = start;
        int valid = 1;
        for (i = 0; i < cnt; i++)
        {
            if (adj[prev][perm[i]] == INF)
            {
                valid = 0;
                break;
            }
            dist += adj[prev][perm[i]];
            prev = perm[i];
        }
        // return to start
        if (valid && adj[prev][start] != INF)
        {
            dist += adj[prev][start];
            if (dist < best)
            {
                best = dist;
                for (i = 0; i < cnt; i++)
                    best_path[i] = perm[i];
            }
        }
    } while (next_perm(perm, cnt));

    if (best == INF)
    {
        printf("No valid TSP route found.\n");
        return;
    }

    printf("TSP Best Route (from %s):\n", names[start]);
    printf("%s -> ", names[start]);
    for (i = 0; i < cnt; i++)
    {
        printf("%s -> ", names[best_path[i]]);
    }
    printf("%s\n", names[start]);
    printf("Total Distance: %d\n", best);
}
