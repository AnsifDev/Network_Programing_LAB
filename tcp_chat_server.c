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
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(3000);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    printf("Creating Socket...\n");
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) { printf("\e[1A\e[2KCreating Socket...\t\t[FAIL]\n"); return 1; }
    else printf("\e[1A\e[2KCreating Socket...\t\t[OK]\n");

    printf("Binding Socket...\n");
    int b = bind(s, (struct sockaddr*) &server, sizeof(server));
    if (b < 0) { printf("\e[1A\e[2KBinding Socket...\t\t[FAIL]\n"); return 1; }
    else printf("\e[1A\e[2KBinding Socket...\t\t[OK]\n");
    
    int l = listen(s, 5);
    if (l < 0) { printf("ERR: Server Listening Failed\n"); return 1; }

    int run = 1;
    while (run) {
        printf("Waiting for a connection...\n");
        socklen_t socketlen = sizeof(server);
        int ns = accept(s, (struct sockaddr*) &server, &socketlen);
        if (ns < 0) { printf("Connection to client Failed\n"); return 1; }
        else printf("New Client Connected\n");

        while (1) {
            char read_buff[100], write_buff[100];

            int r = read(ns, read_buff, 100);
            if (r == 0) break;
            if (strcmp("SHUTDOWN", read_buff) == 0) { run = 0; break; }

            printf("  %s\n", read_buff);
            make_response(write_buff);
            write(ns, write_buff, strlen(write_buff)+1);
        }

        printf("Client Disconnected\n");
    }
    printf("Server Shutdown\n");

    return 0;
}