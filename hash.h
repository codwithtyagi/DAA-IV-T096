#ifndef HASH_H
#define HASH_H

#define HT_SIZE 41

typedef struct {
    int id;
    int idx;
    int used;
} HashNode;

HashNode ht[HT_SIZE];

int hashFn(int id);
void hashInsert(int id, int idx);
int hashFind(int id);
void hashClear();

#endif
