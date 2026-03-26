#include "schedule.h"
#include <stdio.h>
#include <string.h>

// min-heap priority queue
Order pq[MAX_ORDERS];
int pq_cnt = 0;

// swap orders
void swap(Order *a, Order *b)
{
    Order t = *a;
    *a = *b;
    *b = t;
}

// heapify up
void heap_up(int i)
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

// heapify down
void heap_down(int i)
{
    int l, r, smallest;
    while (1)
    {
        l = 2 * i + 1;
        r = 2 * i + 2;
        smallest = i;
        if (l < pq_cnt && pq[l].priority < pq[smallest].priority)
            smallest = l;
        if (r < pq_cnt && pq[r].priority < pq[smallest].priority)
            smallest = r;
        if (smallest != i)
        {
            swap(&pq[i], &pq[smallest]);
            i = smallest;
        }
        else break;
    }
}

// push to pq
void pq_push(Order o)
{
    if (pq_cnt >= MAX_ORDERS) return;
    pq[pq_cnt] = o;
    heap_up(pq_cnt);
    pq_cnt++;
}

// pop from pq
Order pq_pop()
{
    Order top = pq[0];
    pq_cnt--;
    pq[0] = pq[pq_cnt];
    heap_down(0);
    return top;
}

// pq size
int pq_size()
{
    return pq_cnt;
}

// clear pq
void pq_clear()
{
    pq_cnt = 0;
}

// add order
void insert_order(int id, char *name, int pri, int wt, int val,
                  int src, int dst, int deadline, char *status)
{
    if (ord_cnt >= MAX_ORDERS) return;
    Order *o = &orders[ord_cnt];
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
    ord_cnt++;
}

// schedule deliveries by priority
void schedule_deliveries()
{
    int i;
    Order o;

    if (ord_cnt == 0)
    {
        printf("No orders to schedule.\n");
        return;
    }

    // push all orders into pq
    pq_clear();
    for (i = 0; i < ord_cnt; i++)
        pq_push(orders[i]);

    printf("Scheduled Deliveries (by priority):\n");
    printf("%-6s %-15s %-8s %-8s %-10s %-10s\n",
             "ID", "Name", "Pri", "Wt", "Deadline", "Status");

    while (pq_size() > 0)
    {
        o = pq_pop();
        printf("%-6d %-15s %-8d %-8d %-10d %-10s\n",
                 o.id, o.name, o.priority, o.weight, o.deadline, o.status);
    }
}
