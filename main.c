#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "graph.h"
#include "routing.h"
#include "schedule.h"
#include "optimize.h"
#include "search_sort.h"
#include "hash.h"
#include "file_io.h"

void printMenu()
{
    printf("\n===========================================\n");
    printf("   Smart Delivery & Routing System v2.0\n");
    printf("===========================================\n");
    printf(" 1. Add Warehouse/City\n");
    printf(" 2. Add Road (Edge)\n");
    printf(" 3. Display Graph\n");
    printf(" 4. Shortest Path (Dijkstra)\n");
    printf(" 5. BFS/DFS Traversal\n");
    printf(" 6. Add Delivery Order\n");
    printf(" 7. Schedule Deliveries\n");
    printf(" 8. Knapsack (Vehicle Loading)\n");
    printf(" 9. TSP Best Route\n");
    printf("10. Search & Sort Orders\n");
    printf("11. Save Graph (TXT)\n");
    printf("12. Load Graph (TXT)\n");
    printf("13. Export Orders (TXT)\n");
    printf("14. Import Orders (TXT)\n");
    printf("15. Exit\n");
    printf("===========================================\n");
    printf("Enter choice: ");
}

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    int choice;
    char name[MAX_NAME], name2[MAX_NAME], status[MAX_NAME];
    int id, pri, wt, val, src, dst, dl, distVal, cost;
    int i;

    // init data structures
    initGraph();
    hashClear();

    while (1)
    {
        printMenu();
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
        {
            // Add Warehouse/City
            printf("Enter node name: ");
            scanf("%s", name);
            int idx = addNode(name);
            if (idx >= 0)
                printf("Added node [%d]: %s\n", idx, name);
            else
                printf("Failed to add node (max reached).\n");
            break;
        }
        case 2:
        {
            // Add Road (Edge)
            printf("Enter source node name: ");
            scanf("%s", name);
            printf("Enter dest node name: ");
            scanf("%s", name2);
            printf("Enter distance: ");
            scanf("%d", &distVal);
            printf("Enter cost: ");
            scanf("%d", &cost);
            int s = findNode(name);
            int d = findNode(name2);
            if (s < 0 || d < 0)
            {
                printf("Node not found!\n");
            }
            else
            {
                addEdge(s, d, distVal, cost);
                printf("Edge added: %s <-> %s (dist=%d, cost=%d)\n",
                       name, name2, distVal, cost);
            }
            break;
        }
        case 3:
        {
            // Display Graph
            printGraph();
            break;
        }
        case 4:
        {
            // Dijkstra Shortest Path
            int dist[MAX_NODES], prev[MAX_NODES];
            printf("Enter source node: ");
            scanf("%s", name);
            printf("Enter dest node: ");
            scanf("%s", name2);
            int s = findNode(name);
            int d = findNode(name2);
            if (s < 0 || d < 0)
            {
                printf("Node not found!\n");
            }
            else
            {
                dijkstra(s, dist, prev);
                printf("Shortest distance from %s to %s: %d\n",
                       name, name2,
                       (dist[d] == INF) ? -1 : dist[d]);
                if (dist[d] != INF)
                {
                    printf("Path: ");
                    printPath(prev, s, d);
                }
            }
            break;
        }
        case 5:
        {
            // BFS/DFS Traversal
            int trav;
            printf("Enter start node: ");
            scanf("%s", name);
            int s = findNode(name);
            if (s < 0)
            {
                printf("Node not found!\n");
            }
            else
            {
                printf("Traversal method (0=BFS, 1=DFS): ");
                scanf("%d", &trav);
                if (trav == 0) bfs(s);
                else if (trav == 1) dfs(s);
                else printf("Invalid method.\n");
            }
            break;
        }
        case 6:
        {
            // Add Delivery Order
            printf("Enter Order ID: ");
            scanf("%d", &id);
            printf("Enter Order Name: ");
            scanf("%s", name);
            printf("Enter Priority (lower=higher): ");
            scanf("%d", &pri);
            printf("Enter Weight: ");
            scanf("%d", &wt);
            printf("Enter Value: ");
            scanf("%d", &val);
            printf("Enter Source Node: ");
            scanf("%s", name2);
            src = findNode(name2);
            printf("Enter Dest Node: ");
            scanf("%s", name2);
            dst = findNode(name2);
            printf("Enter Deadline: ");
            scanf("%d", &dl);
            printf("Enter Status: ");
            scanf("%s", status);
            if (src < 0 || dst < 0)
            {
                printf("Source or dest node not found!\n");
            }
            else
            {
                insertOrder(id, name, pri, wt, val, src, dst, dl, status);
                hashInsert(id, ordCnt - 1);
                printf("Order added: ID=%d, Name=%s\n", id, name);
            }
            break;
        }
        case 7:
        {
            // Schedule Deliveries
            scheduleDeliveries();
            break;
        }
        case 8:
        {
            // Knapsack (Vehicle Loading)
            int cap;
            int w[MAX_ORDERS], v[MAX_ORDERS];
            printf("Enter vehicle capacity: ");
            scanf("%d", &cap);
            for (i = 0; i < ordCnt; i++)
            {
                w[i] = orders[i].weight;
                v[i] = orders[i].value;
            }
            knapsack(cap, w, v, ordCnt);

            // show order details
            printf("\nOrder Details:\n");
            printf("%-6s %-15s %-8s %-8s\n",
                   "ID", "Name", "Weight", "Value");
            for (i = 0; i < ordCnt; i++)
            {
                printf("%-6d %-15s %-8d %-8d\n",
                       orders[i].id, orders[i].name,
                       orders[i].weight, orders[i].value);
            }
            break;
        }
        case 9:
        {
            // TSP Best Route
            printf("Enter start node: ");
            scanf("%s", name);
            int s = findNode(name);
            if (s < 0)
            {
                printf("Node not found!\n");
            }
            else tsp(s);
            break;
        }
        case 10:
        {
            // Search & Sort Orders
            int sub;
            printf("\n--- Search & Sort Orders ---\n");
            printf("1. Search by ID\n");
            printf("2. Search by Name\n");
            printf("3. Sort Orders\n");
            printf("Enter sub-choice: ");
            scanf("%d", &sub);

            if (sub == 1)
            {
                // Search Order by ID
                int method, idx = -1;
                const char *algo = "";
                printf("Enter Order ID: ");
                scanf("%d", &id);
                printf("Method (0=Hash O(1), 1=Jump, 2=Binary): ");
                scanf("%d", &method);

                if (method == 0)
                {
                    idx = hashFind(id);
                    algo = "Hash Table O(1)";
                }
                else if (method == 1 || method == 2)
                {
                    Order sorted[MAX_ORDERS];
                    for (i = 0; i < ordCnt; i++) sorted[i] = orders[i];
                    mergeSort(sorted, 0, ordCnt - 1, 3);

                    if (method == 1)
                    {
                        idx = jumpSearch(sorted, ordCnt, id);
                        algo = "Jump Search O(sqrt(n))";
                    }
                    else
                    {
                        int lo = 0, hi = ordCnt - 1, mid;
                        while (lo <= hi)
                        {
                            mid = (lo + hi) / 2;
                            if (sorted[mid].id == id)
                            {
                                idx = mid;
                                break;
                            }
                            else if (sorted[mid].id < id) lo = mid + 1;
                            else hi = mid - 1;
                        }
                        algo = "Binary Search O(log n)";
                    }

                    if (idx >= 0)
                    {
                        int found_id = sorted[idx].id;
                        idx = -1;
                        for (i = 0; i < ordCnt; i++)
                            if (orders[i].id == found_id)
                            {
                                idx = i;
                                break;
                            }
                    }
                }
                else
                {
                    printf("Invalid method.\n");
                    algo = "";
                }

                if (method == 0 || method == 1 || method == 2)
                {
                    printf("Algorithm used: %s\n\n", algo);
                    if (idx >= 0)
                    {
                        Order *o = &orders[idx];
                        printf("Found Order:\n");
                        printf("ID: %d\n", o->id);
                        printf("Name: %s\n", o->name);
                        printf("Priority: %d\n", o->priority);
                        printf("Weight: %d\n", o->weight);
                        printf("Value: %d\n", o->value);
                        printf("Src: %s\n", names[o->src]);
                        printf("Dst: %s\n", names[o->dst]);
                        printf("Deadline: %d\n", o->deadline);
                        printf("Status: %s\n", o->status);
                    }
                    else
                    {
                        printf("Order not found.\n");
                    }
                }
            }
            else if (sub == 2)
            {
                // Search Order by Name
                Order sorted[MAX_ORDERS];
                for (i = 0; i < ordCnt; i++) sorted[i] = orders[i];
                mergeSort(sorted, 0, ordCnt - 1, 4);

                printf("Enter Order Name: ");
                scanf("%s", name);
                int idx = binSearch(sorted, ordCnt, name);
                printf("Algorithm used: Binary Search O(log n) on name-sorted array\n\n");
                if (idx >= 0)
                {
                    Order *o = &sorted[idx];
                    printf("Found Order:\n");
                    printf("ID: %d\n", o->id);
                    printf("Name: %s\n", o->name);
                    printf("Priority: %d\n", o->priority);
                    printf("Weight: %d\n", o->weight);
                    printf("Value: %d\n", o->value);
                    printf("Deadline: %d\n", o->deadline);
                    printf("Status: %s\n", o->status);
                }
                else
                {
                    printf("Order not found.\n");
                }
            }
            else if (sub == 3)
            {
                // Sort Orders
                int key;
                printf("Sort by (0=Value, 1=Weight, 2=Priority, 3=ID, 4=Name): ");
                scanf("%d", &key);

                mergeSort(orders, 0, ordCnt - 1, key);

                // rebuild hash table
                hashClear();
                for (i = 0; i < ordCnt; i++)
                    hashInsert(orders[i].id, i);

                const char *knames[] = {"Value", "Weight", "Priority", "ID", "Name"};
                printf("Orders sorted by %s using Merge Sort:\n", knames[key % 5]);

                printf("%-6s %-15s %-8s %-8s %-8s %-10s %-10s\n",
                       "ID", "Name", "Pri", "Wt", "Val", "Deadline", "Status");
                for (i = 0; i < ordCnt; i++)
                {
                    printf("%-6d %-15s %-8d %-8d %-8d %-10d %-10s\n",
                           orders[i].id, orders[i].name, orders[i].priority,
                           orders[i].weight, orders[i].value,
                           orders[i].deadline, orders[i].status);
                }
            }
            else
            {
                printf("Invalid sub-choice.\n");
            }
            break;
        }
        case 11:
        {
            // Save Graph Data
            char savefile[MAX_NAME];
            printf("Enter filename (or press Enter for '%s'): ", GRAPH_FILE);
            getchar();
            if (fgets(savefile, sizeof(savefile), stdin) != NULL)
            {
                savefile[strcspn(savefile, "\n")] = '\0';
                if (strlen(savefile) == 0)
                    strncpy(savefile, GRAPH_FILE, MAX_NAME);
            }
            saveGraphTXT(savefile);
            break;
        }
        case 12:
        {
            // Load Graph Data
            char loadfile[MAX_NAME];
            printf("Enter filename (or press Enter for '%s'): ", GRAPH_FILE);
            getchar();
            if (fgets(loadfile, sizeof(loadfile), stdin) != NULL)
            {
                loadfile[strcspn(loadfile, "\n")] = '\0';
                if (strlen(loadfile) == 0)
                    strncpy(loadfile, GRAPH_FILE, MAX_NAME);
            }
            loadGraphTXT(loadfile);
            break;
        }
        case 13:
        {
            // Export Orders to TXT
            char txtfile[MAX_NAME];
            printf("Enter filename (or press Enter for '%s'): ", ORDERS_FILE);
            getchar();
            if (fgets(txtfile, sizeof(txtfile), stdin) != NULL)
            {
                txtfile[strcspn(txtfile, "\n")] = '\0';
                if (strlen(txtfile) == 0)
                    strncpy(txtfile, ORDERS_FILE, MAX_NAME);
            }
            exportOrdersTXT(txtfile);
            break;
        }
        case 14:
        {
            // Import Orders from TXT
            char txtfile2[MAX_NAME];
            printf("Enter filename (or press Enter for '%s'): ", ORDERS_FILE);
            getchar();
            if (fgets(txtfile2, sizeof(txtfile2), stdin) != NULL)
            {
                txtfile2[strcspn(txtfile2, "\n")] = '\0';
                if (strlen(txtfile2) == 0)
                    strncpy(txtfile2, ORDERS_FILE, MAX_NAME);
            }
            importOrdersTXT(txtfile2);
            break;
        }
        case 15:
        {
            // Exit
            printf("Exiting...\n");
            return 0;
        }
        default:
            printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}
