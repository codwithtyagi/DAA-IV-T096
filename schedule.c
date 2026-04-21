#include "schedule.h"
#include <stdio.h>
#include <string.h>

// min-heap pq
Order pq[MAX_ORDERS];
int pqCnt = 0;

// swap
void swap(Order *a, Order *b)
{
    Order t = *a;
    *a = *b;
    *b = t;
}

// heap up
void heapUp(int i)
{
    while (i > 0)
    {
        int p = (i - 1) / 2;
        if (pq[i].priority < pq[p].priority)
        {
            swap(&pq[i], &pq[p]);
            i = p;
        }
        else break;
    }
}

// heap down
void heapDown(int i)
{
    int l, r, smallest;
    while (1)
    {
        l = 2 * i + 1;
        r = 2 * i + 2;
        smallest = i;
        if (l < pqCnt && pq[l].priority < pq[smallest].priority)
            smallest = l;
        if (r < pqCnt && pq[r].priority < pq[smallest].priority)
            smallest = r;
        if (smallest != i)
        {
            swap(&pq[i], &pq[smallest]);
            i = smallest;
        }
        else break;
    }
}

// push
void pqPush(Order o)
{
    if (pqCnt >= MAX_ORDERS) return;
    pq[pqCnt] = o;
    heapUp(pqCnt);
    pqCnt++;
}

// pop
Order pqPop()
{
    Order top = pq[0];
    pqCnt--;
    pq[0] = pq[pqCnt];
    heapDown(0);
    return top;
}

// size
int pqSize()
{
    return pqCnt;
}

// clear
void pqClear()
{
    pqCnt = 0;
}

// insert
void insertOrder(int id, char *name, int pri, int wt, int val,
                  int src, int dst, int deadline, char *status)
{
    if (ordCnt >= MAX_ORDERS) return;
    Order *o = &orders[ordCnt];
    o->id = id;
    strncpy(o->name, name, MAX_NAME - 1);
    o->name[MAX_NAME - 1] = '\0';
    o->priority = pri;
    o->weight = wt;
    o->value = val;
    o->src = src;
    o->dst = dst;
    o->deadline = deadline;
    strncpy(o->status, status, MAX_NAME - 1);
    o->status[MAX_NAME - 1] = '\0';
    ordCnt++;
}

// schedule by priority
void scheduleDeliveries()
{
    int i;
    Order o;

    if (ordCnt == 0)
    {
        printf("No orders to schedule.\n");
        return;
    }

    // push all to pq
    pqClear();
    for (i = 0; i < ordCnt; i++)
        pqPush(orders[i]);

    printf("Scheduled Deliveries (by priority):\n");
    printf("%-6s %-15s %-8s %-8s %-10s %-10s\n",
             "ID", "Name", "Pri", "Wt", "Deadline", "Status");

    while (pqSize() > 0)
    {
        o = pqPop();
        printf("%-6d %-15s %-8d %-8d %-10d %-10s\n",
                 o.id, o.name, o.priority, o.weight, o.deadline, o.status);
    }
}
