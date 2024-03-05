#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

int main() {
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(5000);
    server.sin_addr.s_addr = htonl(0x0ab7d4cb);

    printf("Creating Socket...\n");
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) { printf("\e[1A\e[2KCreating Socket...\t\t[FAIL]\n"); return 1; }
    else printf("\e[1A\e[2KCreating Socket...\t\t[OK]\n");

    printf("Connecting to Server...\n");
    int c = connect(s, (struct sockaddr*) &server, sizeof(server));
    if (c < 0) { printf("\e[1A\e[2KConnecting to Server...\t\t[FAIL]\n"); return 1; }
    else printf("\e[1A\e[2KConnecting to Server...\t\t[OK]\n");

    while(1) {
        char input[31], read_buff[100];
        printf(">> ");
        scanf("%s", input);
        if (strcmp("QUIT", input) == 0) { close(s); break; }

        int written_size = write(s, input, strlen(input)+1);
        if (strcmp("SHUTDOWN", input) == 0) { close(s); break; }

        int r = read(s, read_buff, 100);
        if (r == 0) break;
        printf("%s\n", read_buff);
    }

    return 0;
}