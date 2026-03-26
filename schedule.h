#ifndef SCHEDULE_H
#define SCHEDULE_H

#include "utils.h"

// order struct
typedef struct {
    int id;
    int priority;   // lower = higher priority
    int weight;
    int value;
    char name[MAX_NAME];
    int src;
    int dst;
    int deadline;
    char status[MAX_NAME];
} Order;

// global orders
Order orders[MAX_ORDERS];
int ord_cnt;

// add order
void insert_order(int id, char *name, int pri, int wt, int val,
                  int src, int dst, int deadline, char *status);

// priority queue push
void pq_push(Order o);

// priority queue pop (min priority)
Order pq_pop();

// get pq size
int pq_size();

// clear pq
void pq_clear();

// schedule deliveries (greedy by priority), prints to console
void schedule_deliveries();

#endif
