#ifndef HASH_H
#define HASH_H

#define HT_SIZE 41

// hash node
typedef struct {
    int id;    // order id
    int idx;   // index in orders array
    int used;  // 0=empty, 1=used
} HashNode;

// global hash table
HashNode ht[HT_SIZE];

// hash function
int hash_fn(int id);

// insert into hash table
void hash_insert(int id, int idx);

// find order index by id, returns -1 if not found
int hash_find(int id);

// clear hash table
void hash_clear();

#endif
