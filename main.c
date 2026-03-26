/*
 * main.c - Smart Delivery & Routing Optimization System
 * Console-only version using printf/scanf
 */
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

void print_menu()
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
    printf("11. Exit\n");
    printf("===========================================\n");
    printf("Enter choice: ");
}

int main()
{
    int choice;
    char name[MAX_NAME], name2[MAX_NAME], status[MAX_NAME];
    int id, pri, wt, val, src, dst, dl, dist_val, cost;
    int i;

    // init data structures
    init_graph();
    hash_clear();

    while (1)
    {
        print_menu();
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
        {
            // Add Warehouse/City
            printf("Enter node name: ");
            scanf("%s", name);
            int idx = add_node(name);
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
            scanf("%d", &dist_val);
            printf("Enter cost: ");
            scanf("%d", &cost);
            int s = find_node(name);
            int d = find_node(name2);
            if (s < 0 || d < 0)
            {
                printf("Node not found!\n");
            }
            else
            {
                add_edge(s, d, dist_val, cost);
                printf("Edge added: %s <-> %s (dist=%d, cost=%d)\n",
                       name, name2, dist_val, cost);
            }
            break;
        }
        case 3:
        {
            // Display Graph
            print_graph();
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
            int s = find_node(name);
            int d = find_node(name2);
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
                    print_path(prev, s, d);
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
            int s = find_node(name);
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
            src = find_node(name2);
            printf("Enter Dest Node: ");
            scanf("%s", name2);
            dst = find_node(name2);
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
                insert_order(id, name, pri, wt, val, src, dst, dl, status);
                hash_insert(id, ord_cnt - 1);
                printf("Order added: ID=%d, Name=%s\n", id, name);
            }
            break;
        }
        case 7:
        {
            // Schedule Deliveries
            schedule_deliveries();
            break;
        }
        case 8:
        {
            // Knapsack (Vehicle Loading)
            int cap;
            int w[MAX_ORDERS], v[MAX_ORDERS];
            printf("Enter vehicle capacity: ");
            scanf("%d", &cap);
            for (i = 0; i < ord_cnt; i++)
            {
                w[i] = orders[i].weight;
                v[i] = orders[i].value;
            }
            knapsack(cap, w, v, ord_cnt);

            // show order details
            printf("\nOrder Details:\n");
            printf("%-6s %-15s %-8s %-8s\n",
                   "ID", "Name", "Weight", "Value");
            for (i = 0; i < ord_cnt; i++)
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
            int s = find_node(name);
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
                printf("Method (0=Hash O(1), 1=Interpolation, 2=Jump, 3=Binary): ");
                scanf("%d", &method);

                if (method == 0)
                {
                    idx = hash_find(id);
                    algo = "Hash Table O(1)";
                }
                else
                {
                    Order sorted[MAX_ORDERS];
                    for (i = 0; i < ord_cnt; i++) sorted[i] = orders[i];
                    merge_sort(sorted, 0, ord_cnt - 1, 3);

                    if (method == 1)
                    {
                        idx = interp_search(sorted, ord_cnt, id);
                        algo = "Interpolation Search O(log log n)";
                    }
                    else if (method == 2)
                    {
                        idx = jump_search(sorted, ord_cnt, id);
                        algo = "Jump Search O(sqrt(n))";
                    }
                    else
                    {
                        int lo = 0, hi = ord_cnt - 1, mid;
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
                        for (i = 0; i < ord_cnt; i++)
                            if (orders[i].id == found_id)
                            {
                                idx = i;
                                break;
                            }
                    }
                }

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
            else if (sub == 2)
            {
                // Search Order by Name
                Order sorted[MAX_ORDERS];
                for (i = 0; i < ord_cnt; i++) sorted[i] = orders[i];
                merge_sort(sorted, 0, ord_cnt - 1, 4);

                printf("Enter Order Name: ");
                scanf("%s", name);
                int idx = bin_search(sorted, ord_cnt, name);
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
                int key, method;
                printf("Sort by (0=Value, 1=Weight, 2=Priority, 3=ID, 4=Name): ");
                scanf("%d", &key);
                printf("Method (0=MergeSort, 1=QuickSort): ");
                scanf("%d", &method);

                if (method == 0)
                    merge_sort(orders, 0, ord_cnt - 1, key);
                else
                    quick_sort(orders, 0, ord_cnt - 1, key);

                // rebuild hash table
                hash_clear();
                for (i = 0; i < ord_cnt; i++)
                    hash_insert(orders[i].id, i);

                const char *knames[] = {"Value", "Weight", "Priority", "ID", "Name"};
                const char *mnames[] = {"Merge Sort", "Quick Sort"};
                printf("Orders sorted by %s using %s:\n",
                       knames[key % 5], mnames[method % 2]);

                printf("%-6s %-15s %-8s %-8s %-8s %-10s %-10s\n",
                       "ID", "Name", "Pri", "Wt", "Val", "Deadline", "Status");
                for (i = 0; i < ord_cnt; i++)
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
