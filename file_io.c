#include <stdio.h>
#include <string.h>
#include "file_io.h"

// Save graph (nodes + edges) to TXT
void saveGraphTXT(const char *filename)
{
    int i, j;
    FILE *fp = fopen(filename, "w");
    if (!fp)
    {
        printf("Error: Cannot open file '%s' for writing.\n", filename);
        return;
    }

    // Write node count and names
    fprintf(fp, "%d\n", nNodes);
    for (i = 0; i < nNodes; i++)
        fprintf(fp, "%s\n", names[i]);

    // Count edges (undirected, so count each pair once)
    int edgeCount = 0;
    for (i = 0; i < nNodes; i++)
        for (j = i + 1; j < nNodes; j++)
            if (adj[i][j] != INF && adj[i][j] != 0)
                edgeCount++;

    fprintf(fp, "%d\n", edgeCount);
    for (i = 0; i < nNodes; i++)
        for (j = i + 1; j < nNodes; j++)
            if (adj[i][j] != INF && adj[i][j] != 0)
                fprintf(fp, "%s %s %d %d\n", names[i], names[j],
                        adj[i][j], costMat[i][j]);

    fclose(fp);
    printf("Graph saved to '%s'. (%d nodes, %d edges)\n",
           filename, nNodes, edgeCount);
}

// Load graph (nodes + edges) from TXT
void loadGraphTXT(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    char line[256];
    if (!fp)
    {
        printf("Error: Cannot open file '%s' for reading.\n", filename);
        return;
    }

    // Reset graph
    initGraph();

    // Read node count
    int n = 0;
    if (fgets(line, sizeof(line), fp) == NULL) { fclose(fp); return; }
    sscanf(line, "%d", &n);

    // Read node names
    int i;
    for (i = 0; i < n; i++)
    {
        char name[MAX_NAME];
        if (fgets(line, sizeof(line), fp) == NULL) break;
        sscanf(line, "%s", name);
        addNode(name);
    }

    // Read edge count
    int e = 0;
    if (fgets(line, sizeof(line), fp) == NULL) { fclose(fp); return; }
    sscanf(line, "%d", &e);

    // Read edges
    for (i = 0; i < e; i++)
    {
        char src[MAX_NAME], dst[MAX_NAME];
        int dist, cost;
        if (fgets(line, sizeof(line), fp) == NULL) break;
        if (sscanf(line, "%s %s %d %d", src, dst, &dist, &cost) != 4) continue;

        int s = findNode(src);
        int d = findNode(dst);
        if (s >= 0 && d >= 0)
            addEdge(s, d, dist, cost);
    }

    fclose(fp);
    printf("Graph loaded from '%s'. (%d nodes, %d edges)\n",
           filename, nNodes, e);
}

// Export orders to TXT
void exportOrdersTXT(const char *filename)
{
    int i;
    FILE *fp = fopen(filename, "w");
    if (!fp)
    {
        printf("Error: Cannot open file '%s' for writing.\n", filename);
        return;
    }

    fprintf(fp, "==========================================\n");
    fprintf(fp, "       Delivery Orders Report\n");
    fprintf(fp, "==========================================\n");
    fprintf(fp, "Total Orders: %d\n\n", ordCnt);

    fprintf(fp, "%-6s %-15s %-8s %-8s %-8s %-12s %-12s %-10s %-10s\n",
            "ID", "Name", "Pri", "Weight", "Value", "Src", "Dst", "Deadline", "Status");
    fprintf(fp, "----------------------------------------------------------------------"
            "----------------------------\n");

    for (i = 0; i < ordCnt; i++)
    {
        Order *o = &orders[i];
        fprintf(fp, "%-6d %-15s %-8d %-8d %-8d %-12s %-12s %-10d %-10s\n",
                o->id, o->name, o->priority, o->weight, o->value,
                names[o->src], names[o->dst], o->deadline, o->status);
    }

    fprintf(fp, "==========================================\n");

    fclose(fp);
    printf("Orders exported to '%s'. (%d orders)\n", filename, ordCnt);
}

// Import orders from TXT
void importOrdersTXT(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    char line[256];
    int count = 0;
    char srcName[MAX_NAME], dstName[MAX_NAME];

    if (!fp)
    {
        printf("Error: Cannot open file '%s' for reading.\n", filename);
        return;
    }

    // Skip header lines (everything until the dashed separator line)
    int headerDone = 0;
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (strncmp(line, "------", 6) == 0)
        {
            headerDone = 1;
            break;
        }
    }

    if (!headerDone)
    {
        printf("Error: Invalid file format.\n");
        fclose(fp);
        return;
    }

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        int id, pri, wt, val, dl;
        char name[MAX_NAME], status[MAX_NAME];

        // Stop at footer separator
        if (strncmp(line, "====", 4) == 0) break;

        int parsed = sscanf(line, "%d %s %d %d %d %s %s %d %s",
                            &id, name, &pri, &wt, &val,
                            srcName, dstName, &dl, status);
        if (parsed != 9) continue;

        int src = findNode(srcName);
        int dst = findNode(dstName);

        if (src < 0 || dst < 0)
        {
            printf("Warning: Skipping order ID=%d (unknown node '%s' or '%s')\n",
                   id, srcName, dstName);
            continue;
        }

        insertOrder(id, name, pri, wt, val, src, dst, dl, status);
        hashInsert(id, ordCnt - 1);
        count++;
    }

    fclose(fp);
    printf("Imported %d orders from '%s'.\n", count, filename);
}
