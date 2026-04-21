#include "hash.h"
#include <stdio.h>

// hash fn
int hashFn(int id)
{
    return id % HT_SIZE;
}

// clear
void hashClear()
{
    int i;
    for (i = 0; i < HT_SIZE; i++)
        ht[i].used = 0;
}

// insert
void hashInsert(int id, int idx)
{
    int h = hashFn(id);
    int i;
    for (i = 0; i < HT_SIZE; i++)
    {
        int pos = (h + i) % HT_SIZE;
        if (!ht[pos].used)
        {
            ht[pos].id = id;
            ht[pos].idx = idx;
            ht[pos].used = 1;
            return;
        }
    }
    printf("Hash table full!\n");
}

// find by id
int hashFind(int id)
{
    int h = hashFn(id);
    int i;
    for (i = 0; i < HT_SIZE; i++)
    {
        int pos = (h + i) % HT_SIZE;
        if (!ht[pos].used) return -1;
        if (ht[pos].id == id) return ht[pos].idx;
    }
    return -1;
}
