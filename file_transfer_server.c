#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

int read_file(char *str, FILE *src, int limit) {
    int str_len = 0;
    char c;
    for (int i = 0; i < limit; i++) {
        c = fgetc(src);
        if (c == EOF) break;
        str[str_len++] = c;
    }
    if (c == EOF) str[str_len] = '\0';
    // printf("Found: %s\n", str);
    return str_len;
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
        socklen_t socketlen = sizeof(client);
        int ns = accept(s, (struct sockaddr*) &client, &socketlen);
        if (ns < 0) { printf("Connection to client Failed\n"); return 1; }
        else printf("New Client Connected\n");

        while (1) {
            char read_buff[100], write_buff[100];

            int r = read(ns, read_buff, 100);
            if (r == 0) break;
            if (strcmp("SHUTDOWN", read_buff) == 0) { run = 0; break; }

            if (strcmp("PROMPT", read_buff) == 0) {
                sprintf(write_buff, "Enter the filename: ");
                write(ns, write_buff, strlen(write_buff)+1);
                continue;
            }

            FILE *file = fopen(read_buff, "r");
            int str_len = 0;
            while ((str_len = read_file(write_buff, file, 99)) != 0) {
                write(ns, write_buff, str_len);

                write_buff[str_len] = '\0';
                printf("%s", write_buff);
            }
            printf("\n");
            write(ns, "\0", 1);
        }

        printf("Client Disconnected\n");
    }
    printf("Server Shutdown\n");

    return 0;
}