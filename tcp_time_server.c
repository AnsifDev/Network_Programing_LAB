#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

void get_time(char *str) {
    time_t t = time(NULL);
    // sprintf(str, "%s", ctime(&t));
    struct tm *st = localtime(&t);
    sprintf(str, "%d/%d/%d %d:%d:%d", st->tm_mday, st->tm_mon, st->tm_year+1900, st->tm_hour, st->tm_min, st->tm_sec);
}

int main() {
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(5000);
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

            if (strcmp("TIME", read_buff) == 0) get_time(write_buff);
            else sprintf(write_buff, "Unknown command");

            write(ns, write_buff, strlen(write_buff)+1);
            printf("  %s\n", write_buff);
        }

        printf("Client Disconnected\n");
    }
    printf("Server Shutdown\n");

    return 0;
}