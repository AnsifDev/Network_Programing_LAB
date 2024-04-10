#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct VectorTableRow {
    int dist, next, neigbhour;
};
typedef struct VectorTableRow VectorTableRow;

struct SparseTableRow {
    int node, dest, dist, next, dest_next;
};
typedef struct SparseTableRow SparseTableRow;

#define DISP_CAPACITY 6

void display_store(int node_count, VectorTableRow vector_store[node_count][node_count]) {
    for (int i = 0; i < (float) node_count/DISP_CAPACITY; i++) {
        printf("Node%d:\t\t\t", i*DISP_CAPACITY);
        for (int n = i*DISP_CAPACITY+1; n < i*DISP_CAPACITY+DISP_CAPACITY; n++) if (n < node_count) printf("Node%d\t\t\t", n);

        printf("\nDest\tDist\tNext");
        for (int n = i*DISP_CAPACITY+1; n < i*DISP_CAPACITY+DISP_CAPACITY; n++) if (n < node_count) printf("\tDest\tDist\tNext");
        
        printf("\n");
        for (int dest = 0; dest < node_count; dest++) {
            printf("N%d%s\t", dest, i*DISP_CAPACITY == dest? "**": vector_store[i*DISP_CAPACITY][dest].neigbhour? "*": "");
            if (vector_store[i*DISP_CAPACITY][dest].dist == -1) printf("-\t-\t");
            else printf("%d\tN%d\t", vector_store[i*DISP_CAPACITY][dest].dist, vector_store[i*DISP_CAPACITY][dest].next);

            for (int n = i*DISP_CAPACITY+1; n < i*DISP_CAPACITY+DISP_CAPACITY; n++) if (n < node_count) {
                printf("N%d%s\t", dest, n == dest? "**": vector_store[n][dest].neigbhour? "*": "");
                if (vector_store[n][dest].dist == -1) printf("-\t-\t");
                else printf("%d\tN%d\t", vector_store[n][dest].dist, vector_store[n][dest].next);
            }

            printf("\n");
        }
        printf("\n");
    }
}

void exchange(int node_count, VectorTableRow vector_store[node_count][node_count]) {
    SparseTableRow updates[node_count*node_count];
    int updates_len = 0;

    printf("Calculating Changes:\n");
    for (int node = 0; node < node_count; node++) { // For Each Node
        for (int dest = node+1; dest < node_count; dest++) { // For Each Row
            if (node == dest) continue; // Omit Self Record

            int min = vector_store[node][dest].dist;
            for (int neigbhour = 0; neigbhour < node_count; neigbhour++) {
                if (vector_store[node][neigbhour].neigbhour == 0) continue; // Omit Non Neigbhours
                if (dest == neigbhour) continue; // Omit Self Neigbhours

                int cost_to_neigbhour = vector_store[node][neigbhour].dist;
                int cost_to_dest = vector_store[neigbhour][dest].dist;
                int new_dist = cost_to_neigbhour+cost_to_dest;

                if (cost_to_dest == -1) continue; // Omit if Neigbhour don't know
                if (min <= new_dist && min != -1) continue; // Omit if way is lengthy

                // Find dest side neigbhour
                int neigbhour_dest = -1;
                for (int neigbhour = 0; neigbhour < node_count; neigbhour++) {
                    if (vector_store[dest][neigbhour].neigbhour == 0) continue; // Omit Non Neigbhours
                    if (node == neigbhour) continue; // Omit Self Neigbhours

                    int cost_to_dest_neigbhour = vector_store[dest][neigbhour].dist;
                    int cost_to_node = vector_store[neigbhour][node].dist;
                    int dest_dist = cost_to_dest_neigbhour+cost_to_node;

                    if (cost_to_node == -1) continue;
                    if (dest_dist != new_dist) continue; // Omit if dest nieghbour doesn't have same distance
                    if (dest_dist < new_dist) break; // Abort entire operation if a better path is found
                    neigbhour_dest = neigbhour;
                    break;
                }
                
                if (neigbhour_dest == -1) continue;
                min = new_dist;
                updates[updates_len].node = node;
                updates[updates_len].dest = dest;
                updates[updates_len].dist = new_dist;
                updates[updates_len].next = vector_store[node][neigbhour].next;
                updates[updates_len].dest_next = vector_store[dest][neigbhour_dest].next;
                printf("* [N%d - N%d]: (%d, N%d, N%d)\n", node, dest, updates[updates_len].dist, updates[updates_len].next, updates[updates_len].dest_next);
            }

            if (min != vector_store[node][dest].dist) updates_len++;
        }
    }

    printf("Applying Changes: %d\n", updates_len);
    for (int i = 0; i < updates_len; i++) {
        int node = updates[i].node;
        int dest = updates[i].dest;
        printf("* [N%d - N%d]: (%d, N%d, N%d)\n", node, dest, updates[i].dist, updates[i].next, updates[i].dest_next);
        vector_store[node][dest].dist = vector_store[dest][node].dist = updates[i].dist;
        vector_store[node][dest].next = updates[i].next;
        vector_store[dest][node].next = updates[i].dest_next;
    }
}

int main() {
    int node_count = 0;
    printf(">> Enter number of Nodes: ");
    scanf("%d", &node_count);

    VectorTableRow table_store[node_count][node_count];
    for (int node = 0; node < node_count; node++) {
        table_store[node][node].dist = node;
        table_store[node][node].next = node;
        table_store[node][node].neigbhour = 0;

        for (int dest = node+1; dest < node_count; dest++) {
            int dist = 0;
            printf (">> Enter distance between N%d & N%d (-1 if not neigbhours): ", node, dest);
            scanf ("%d", &dist);

            table_store[node][dest].dist = table_store[dest][node].dist = dist;
            table_store[node][dest].next = (dist == -1)? -1: dest;
            table_store[dest][node].next = (dist == -1)? -1: node;
            table_store[node][dest].neigbhour = table_store[dest][node].neigbhour = dist > 0;
        }
    }

    printf("\nBefore Iterations:\n");
    display_store(node_count, table_store);

    for (int i = 0; i < node_count-1; i++) {
        exchange(node_count, table_store);

        printf("After Iteration %d\n", i+1);
        display_store(node_count, table_store);
    }
}

// 6 2 -1 4 3 -1 1 -1 -1 -1 2 4 -1 1 5 2