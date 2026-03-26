#ifndef SEARCH_SORT_H
#define SEARCH_SORT_H

#include "schedule.h"
#include <math.h>

// linear search by order id, returns index or -1
int lin_search(Order arr[], int n, int id);

// binary search by name (array must be sorted by name), returns index or -1
int bin_search(Order arr[], int n, char *name);

// jump search by id (array must be sorted by id) - O(sqrt(n))
int jump_search(Order arr[], int n, int id);

// interpolation search by id (array must be sorted by id) - O(log log n) best
int interp_search(Order arr[], int n, int id);

// merge sort orders by key: 0=value, 1=weight, 2=priority, 3=id, 4=name
void merge_sort(Order arr[], int l, int r, int key);

// quick sort orders by key: 0=value, 1=weight, 2=priority, 3=id, 4=name
void quick_sort(Order arr[], int l, int r, int key);

#endif
