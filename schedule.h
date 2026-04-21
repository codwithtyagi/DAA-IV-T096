#ifndef SCHEDULE_H
#define SCHEDULE_H

#include "utils.h"

typedef struct {
    int id;
    int priority;
    int weight;
    int value;
    char name[MAX_NAME];
    int src;
    int dst;
    int deadline;
    char status[MAX_NAME];
} Order;

Order orders[MAX_ORDERS];
int ordCnt;

void insertOrder(int id, char *name, int pri, int wt, int val,
                  int src, int dst, int deadline, char *status);
void pqPush(Order o);
Order pqPop();
int pqSize();
void pqClear();
void scheduleDeliveries();

#endif
