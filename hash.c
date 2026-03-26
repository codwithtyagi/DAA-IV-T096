#include "hash.h"
#include <stdio.h>

// hash function
int hash_fn(int id)
{
    return id % HT_SIZE;
}

// clear table
void hash_clear()
{
    int i;
    for (i = 0; i < HT_SIZE; i++)
        ht[i].used = 0;
}

// insert with linear probing
void hash_insert(int id, int idx)
{
    int h = hash_fn(id);
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

// find order index by id
int hash_find(int id)
{
    int h = hash_fn(id);
    int i;
    for (i = 0; i < HT_SIZE; i++)
    {
        int pos = (h + i) % HT_SIZE;
        if (!ht[pos].used) return -1;
        if (ht[pos].id == id) return ht[pos].idx;
    }
    return -1;
}
