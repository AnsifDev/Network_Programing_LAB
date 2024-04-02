#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

void make_response(char *str) {
    printf("  >> ");
    scanf("%s", str);
}

int main() {
    struct sockaddr_in server, client;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    printf("Creating Socket...\n");
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) { printf("\e[1A\e[2KCreating Socket...\t\t[FAIL]\n"); return 1; }
    else printf("\e[1A\e[2KCreating Socket...\t\t\t[OK]\n");

    for (int port_no = 3000; port_no < 3100; port_no++) {
        server.sin_port = htons(port_no);
        printf("Binding Socket on Port %d...\n", port_no);
        int b = bind(s, (struct sockaddr*) &server, sizeof(server));
        if (b < 0) { printf("\e[1A\e[2KBinding Socket on Port %d...\t\t[FAIL]\n", port_no); }
        else { printf("\e[1A\e[2KBinding Socket on Port %d...\t\t[OK]\n", port_no); break; }
    }
    
    int l = listen(s, 5);
    if (l < 0) { printf("ERR: Server Listening Failed\n"); return 1; }

    int run = 1;
    while (run) {
        printf("Waiting for a connection...\n");
        socklen_t socketlen = sizeof(server);
        int ns = accept(s, (struct sockaddr*) &client, &socketlen);
        if (ns < 0) { printf("Connection to client Failed\n"); return 1; }
        else printf("New Client Connected\n");

        int packet_number = 0;

        while (1) {
            char read_buff[100], write_buff[100];

            int r = read(ns, read_buff, 100);
            if (r == 0) break;
            if (strcmp("SHUTDOWN", read_buff) == 0) { run = 0; break; }

            if (strcmp("PROMPT", read_buff) == 0) {
                sprintf(write_buff, "Enter the Frame Number: ");
                write(ns, write_buff, strlen(write_buff)+1);
                continue;
            }
            

            int packet_received = 0;
            sscanf(read_buff, "%d", &packet_received);

            if (packet_received != packet_number) {
                sprintf(write_buff, "Resend Frame %d", packet_number);
                printf("ERR: Frame %d not received\n", packet_number);
            }
            else {
                printf("Frame %d Received\n", packet_number);
                sprintf(write_buff, "ACK %d Received", packet_number++);
            }

            write(ns, write_buff, strlen(write_buff)+1);
        }

        printf("Client Disconnected\n");
    }
    printf("Server Shutdown\n");

    return 0;
}