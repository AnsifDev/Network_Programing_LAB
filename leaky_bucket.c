#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main () {
    int bucket_size = 0;
    printf(">> Enter Bucket Size: ");
    scanf("%d", &bucket_size);

    int output_rate = 0;
    printf(">> Enter Output Rate: ");
    scanf("%d", &output_rate);
    
    int packets_waiting = 0, readable = 1;
    for (int i = 0; readable || packets_waiting > 0; i++) {
        printf("\nAt Time %d:\n", i);
        if (packets_waiting > 0) 
            if (packets_waiting > output_rate) printf("Sending %d Packets\n", output_rate);
            else printf("Sending %d Packets\n", packets_waiting);
        
        packets_waiting -= packets_waiting > output_rate ? output_rate: packets_waiting;

        if (packets_waiting > 0)
            if (packets_waiting > output_rate) printf("Packets left in Bucket: %d out of %d\n", packets_waiting, bucket_size);
            else printf("Packets left in Bucket: %d out of %d\n", packets_waiting, bucket_size);
        else printf("Bucket is empty\n");

        int packets = 0;
        if (readable) {
            printf(">> Enter the Number of Packets Arrived: ");
            scanf("%d", &packets);
            if (packets == -1) { packets = 0; readable = 0; }
        }

        packets_waiting += packets;
        if (packets_waiting > bucket_size) { printf("[!] %d packets dropped\n", packets_waiting - bucket_size); packets_waiting = bucket_size; }

        if (readable)
            if (packets_waiting > output_rate) printf("Packets left in Bucket: %d out of %d\n", packets_waiting, bucket_size);
            else printf("Packets left in Bucket: %d out of %d\n", packets_waiting, bucket_size);
    }
    
    // while (packets_waiting > 0) {
    //     if (packets_waiting > output_rate) printf("Sending %d Packets\n", output_rate);
    //     else printf("Sending %d Packets\n", packets_waiting);

    //     packets_waiting -= packets_waiting > output_rate ? output_rate: packets_waiting;

    //     if (packets_waiting > output_rate) printf("Packets left in Bucket: %d out of %d\n", packets_waiting, bucket_size);
    //     else printf("Packets left in Bucket: %d out of %d\n", packets_waiting, bucket_size);
    // }
}