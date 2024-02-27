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

    printf("Creating Socket...\n");
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) { printf("Creating Socket...\t\t[FAIL]\n"); return; }
    else printf("Creating Socket...\t\t[OK]\n");

    printf("Connecting to Server...\n");
    int c = connect(s, (struct sockaddr*) &server, sizeof(server));
    if (c < 0) { printf("Connecting to Server...\t\t[FAIL]\n"); return; }
    else printf("Connecting to Server...\t\t[OK]\n");

    char write_buff[] = "Hello\n";
    int written_size = write(s, write_buff, strlen(write_buff));
    printf("%d\n", written_size);
}