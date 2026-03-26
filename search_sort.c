#include "search_sort.h"
#include <string.h>
#include <math.h>

// get key value from order
int get_key(Order *o, int key)
{
    if (key == 0) return o->value;
    if (key == 1) return o->weight;
    if (key == 3) return o->id;
    return o->priority; // key==2 default
}

// compare for name-based sort (key==4)
int cmp_order(Order *a, Order *b, int key)
{
    if (key == 4) return strcmp(a->name, b->name);
    return get_key(a, key) - get_key(b, key);
}

// linear search by id - O(n)
int lin_search(Order arr[], int n, int id)
{
    int i;
    for (i = 0; i < n; i++)
        if (arr[i].id == id)
            return i;
    return -1;
}

// binary search by name (sorted) - O(log n)
int bin_search(Order arr[], int n, char *name)
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

// jump search by id (sorted by id) - O(sqrt(n))
int jump_search(Order arr[], int n, int id)
{
    int step = (int)sqrt((double)n);
    int prev = 0, i;

    // jump ahead in blocks
    while (prev < n && arr[(prev + step - 1 < n) ? prev + step - 1 : n - 1].id < id)
        prev += step;

    // linear search in block
    for (i = prev; i < n && i < prev + step; i++)
    {
        if (arr[i].id == id)
            return i;
    }
    return -1;
}

// interpolation search by id (sorted by id) - O(log log n) best case
int interp_search(Order arr[], int n, int id)
{
    int lo = 0, hi = n - 1, pos;

    while (lo <= hi && id >= arr[lo].id && id <= arr[hi].id)
    {
        if (lo == hi)
        {
            if (arr[lo].id == id) return lo;
            return -1;
        }

        // estimate position using interpolation formula
        pos = lo + ((double)(hi - lo) / (arr[hi].id - arr[lo].id))
                   * (id - arr[lo].id);

        if (pos < lo || pos > hi) break;

        if (arr[pos].id == id)
            return pos;
        else if (arr[pos].id < id)
            lo = pos + 1;
        else
            hi = pos - 1;
    }
    return -1;
}

// merge sort
void merge(Order arr[], int l, int mid, int r, int key)
{
    Order tmp[MAX_ORDERS];
    int i = l, j = mid + 1, k = 0;

    while (i <= mid && j <= r)
    {
        if (cmp_order(&arr[i], &arr[j], key) <= 0)
            tmp[k++] = arr[i++];
        else
            tmp[k++] = arr[j++];
    }
    while (i <= mid) tmp[k++] = arr[i++];
    while (j <= r) tmp[k++] = arr[j++];

    for (i = 0; i < k; i++)
        arr[l + i] = tmp[i];
}

void merge_sort(Order arr[], int l, int r, int key)
{
    if (l < r)
    {
        int mid = (l + r) / 2;
        merge_sort(arr, l, mid, key);
        merge_sort(arr, mid + 1, r, key);
        merge(arr, l, mid, r, key);
    }
}

// quick sort
int partition(Order arr[], int l, int r, int key)
{
    int i = l - 1, j;
    Order tmp;
    for (j = l; j < r; j++)
    {
        if (cmp_order(&arr[j], &arr[r], key) <= 0)
        {
            i++;
            tmp = arr[i]; arr[i] = arr[j]; arr[j] = tmp;
        }
    }
    tmp = arr[i + 1]; arr[i + 1] = arr[r]; arr[r] = tmp;
    return i + 1;
}

void quick_sort(Order arr[], int l, int r, int key)
{
    if (l < r)
    {
        int p = partition(arr, l, r, key);
        quick_sort(arr, l, p - 1, key);
        quick_sort(arr, p + 1, r, key);
    }
}
