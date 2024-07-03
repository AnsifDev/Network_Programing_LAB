#include <stdio.h>
#include <stdlib.h>

struct VectorMatrixCell {
    int dist, next, neigbhour;
};
typedef struct VectorMatrixCell VectorMatrixCell;

struct UpdateEntry {
    int node, dest, dist, next, rev_next;
};
typedef struct UpdateEntry UpdateEntry;

int display_capacity = 3;

void print_matrix(int nodes, VectorMatrixCell vector_matrix[nodes][nodes]) {
    int reps = nodes/display_capacity + (nodes%display_capacity > 0? 1: 0);
    for (int i = 0; i < reps; i++) {
        for (int j = 0; j < display_capacity; j++) {
            int node = i*display_capacity+j;
            if (node >= nodes) break;

            printf("Node%d\t\t\t\t", node);
        } printf("\n");

        for (int j = 0; j < display_capacity; j++) {
            if (i*display_capacity+j >= nodes) break;
            printf("Dest\tDist\tNext\t\t");
        }
        printf("\n");

        for (int dest = 0; dest < nodes-1; dest++) {
            for (int j = 0; j < display_capacity; j++) {
                int node = i*display_capacity+j;
                if (node >= nodes) break;

                int actual_dest = dest >= node? dest+1: dest;
                int dist = vector_matrix[node][actual_dest].dist;

                printf("N%d\t", actual_dest);
                if (dist < 0) printf("-\t-\t\t");
                else printf("%d\tN%d\t\t", vector_matrix[node][actual_dest].dist, vector_matrix[node][actual_dest].next);
            } printf("\n");
        }

        printf("\n");
    }
}

int make_update(int length, UpdateEntry updations[length], int size, int node, int dest, int dist, int next) {
    for (int i = 0; i < size; i++) {
        int found = updations[i].node == node && updations[i].dest == dest;
        int rev = updations[i].node == dest && updations[i].dest == node;
        if (rev) found = 1;

        if (!found) continue;
        updations[i].dist = dist;
        if (rev) updations[i].rev_next = next;
        else updations[i].next = next;

        return 0;
    }

    updations[size].node = node;
    updations[size].dest = dest;
    updations[size].dist = dist;
    updations[size].next = next;
    
    return 1;
}

void exchange(int nodes, VectorMatrixCell vector_matrix[nodes][nodes]) {
    int updations_length = (nodes*nodes-1)/2, updations_size = 0;
    UpdateEntry updations[updations_length];

    for (int node = 0; node < nodes; node++) for (int dest = 0; dest < nodes; dest++) {
        if (node == dest) continue; // Self

        int min_cost = vector_matrix[node][dest].dist;
        for (int neigbhour = 0; neigbhour < nodes; neigbhour++) {
            if (!vector_matrix[node][neigbhour].neigbhour) continue; // Not a Neighbour

            if (neigbhour == dest) continue; // Neighbour is destination
            if (vector_matrix[node][neigbhour].next != neigbhour) continue; // Not a good Neigbhour

            int neigbhours_cost = vector_matrix[neigbhour][dest].dist;
            if (neigbhours_cost < 0) continue; // Neigbhour don't know destination distance

            int total_cost = neigbhours_cost + vector_matrix[node][neigbhour].dist;
            if (min_cost != -1 && min_cost <= total_cost) continue; // Not a better cost

            min_cost = total_cost;
            if (make_update(updations_length, updations, updations_size, node, dest, min_cost, neigbhour)) updations_size++;
        }
    }

    for (int i = 0; i < updations_size; i++) {
        int node = updations[i].node;
        int dest = updations[i].dest;
        vector_matrix[node][dest].dist = vector_matrix[dest][node].dist = updations[i].dist;
        vector_matrix[node][dest].next = updations[i].next;
        vector_matrix[dest][node].next = updations[i].rev_next;
    }
}

int main() {
    char choice;
    printf("Allow auto loading (Y/N): ");
    scanf("%c", &choice);
    int auto_load = choice == 'y' || choice == 'Y';
    
    FILE *input_stream = auto_load? fopen("dvr_input.txt", "r"): stdin;

    int nodes;
    printf("Enter the number of nodes: ");
    fscanf(input_stream, "%d", &nodes);
    if (auto_load) printf("%d\n", nodes);


    VectorMatrixCell vector_matrix[nodes][nodes];
    for (int node = 0; node < nodes; node++) {
        vector_matrix[node][node].next = node;
        vector_matrix[node][node].dist = 0;
        vector_matrix[node][node].neigbhour = 0;
        
        for (int dest = node+1; dest < nodes; dest++) {
            int dist = -1;
            printf("Distance between N%d - N%d: ", node, dest);
            fscanf(input_stream, "%d", &dist);
            if (auto_load) printf("%d\n", dist);

            if (dist == 0) { printf("ERR: Invalid Length 0\n"); return 1; }

            vector_matrix[node][dest].next = dest;
            vector_matrix[dest][node].next = node;
            vector_matrix[node][dest].dist = vector_matrix[dest][node].dist = dist;
            vector_matrix[node][dest].neigbhour = vector_matrix[dest][node].neigbhour = dist > 0;
        }
    }

    printf("Display capacity: ");
    scanf("%d", &display_capacity);
    if (display_capacity < 1) { printf("ERR: Invalid display capacity\n"); return 2; }

    printf("Before Iteration:\n");
    print_matrix(nodes, vector_matrix);

    for (int i = 0; i < nodes-2; i++) {
        exchange(nodes, vector_matrix);
        printf("After Iteration %d:\n", i);
        print_matrix(nodes, vector_matrix);
    }

    return 0;
}