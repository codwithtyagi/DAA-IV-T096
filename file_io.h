#ifndef FILE_IO_H
#define FILE_IO_H

#include "graph.h"
#include "schedule.h"
#include "hash.h"

#define GRAPH_FILE "graph_data.txt"
#define ORDERS_FILE "orders.txt"

void saveGraphTXT(const char *filename);
void loadGraphTXT(const char *filename);
void exportOrdersTXT(const char *filename);
void importOrdersTXT(const char *filename);

#endif
