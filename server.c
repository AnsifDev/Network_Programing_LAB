#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

void main() {
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(3000);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    printf("Creating Socket...");
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) { printf("\rCreating Socket...\t\t[FAIL]\n"); return; }
    else printf("\rCreating Socket...\t\t[OK]\n");

    printf("Binding...\n");
    int b = bind(s, (struct sockaddr*) &server, sizeof(server));
    if (b < 0) { printf("Binding...\t\t\t[FAIL]\n"); return; }
    else printf("Binding...\t\t\t[OK]\n");
    
    printf("Listening...\n");
    int l = listen(s, 5);
    if (l < 0) { printf("\t\t\t[FAIL]\n"); return; }

    while (1) {
        printf("Waiting for a connection...\n");
        socklen_t socketlen = sizeof(server);
        int ns = accept(s, (struct sockaddr*) &server, &socketlen);
        if (ns < 0) { printf("Connection to client Failed\n"); return; }
        else printf("New Client Connected\n");

        char read_buff[100];
        int r = read(ns, read_buff, 100);
        printf("%s", read_buff);
    }
}