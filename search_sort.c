#include "search_sort.h"
#include <string.h>
#include <math.h>

int getKey(Order *o, int key)
{
    if (key == 0) return o->value;
    if (key == 1) return o->weight;
    if (key == 3) return o->id;
    return o->priority; 
}


int cmpOrder(Order *a, Order *b, int key)
{
    if (key == 4) return strcmp(a->name, b->name);
    return getKey(a, key) - getKey(b, key);
}


int linSearch(Order arr[], int n, int id)
{
    int i;
    for (i = 0; i < n; i++)
        if (arr[i].id == id)
            return i;
    return -1;
}


int binSearch(Order arr[], int n, char *name)
{
    int lo = 0, hi = n - 1, mid, cmp;
    while (lo <= hi)
    {
        mid = (lo + hi) / 2;
        cmp = strcmp(arr[mid].name, name);
        if (cmp == 0) return mid;
        else if (cmp < 0) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}


int jumpSearch(Order arr[], int n, int id)
{
    int step = (int)sqrt((double)n);
    int prev = 0, i;
    while (prev < n && arr[(prev + step - 1 < n) ? prev + step - 1 : n - 1].id < id)
        prev += step;
    for (i = prev; i < n && i < prev + step; i++)
    {
        if (arr[i].id == id)
            return i;
    }
    return -1;
}

// merge
void merge(Order arr[], int l, int mid, int r, int key)
{
    Order tmp[MAX_ORDERS];
    int i = l, j = mid + 1, k = 0;

    while (i <= mid && j <= r)
    {
        if (cmpOrder(&arr[i], &arr[j], key) <= 0)
            tmp[k++] = arr[i++];
        else
            tmp[k++] = arr[j++];
    }
    while (i <= mid) tmp[k++] = arr[i++];
    while (j <= r) tmp[k++] = arr[j++];

    for (i = 0; i < k; i++)
        arr[l + i] = tmp[i];
}

void mergeSort(Order arr[], int l, int r, int key)
{
    if (l < r)
    {
        int mid = (l + r) / 2;
        mergeSort(arr, l, mid, key);
        mergeSort(arr, mid + 1, r, key);
        merge(arr, l, mid, r, key);
    }
}
