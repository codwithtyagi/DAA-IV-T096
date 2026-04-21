#ifndef SEARCH_SORT_H
#define SEARCH_SORT_H

#include "schedule.h"
#include <math.h>

int linSearch(Order arr[], int n, int id);
int binSearch(Order arr[], int n, char *name);
int jumpSearch(Order arr[], int n, int id);
void mergeSort(Order arr[], int l, int r, int key);

#endif
