#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

int getstr(char *str, FILE *src) {
    int str_len = 0;
    char c;
    while (1) {
        c = fgetc(src);
        if (c == '\n') { if (str_len > 0) break; }
        else str[str_len++] = c;
    }
    str[str_len] = '\0';
    return str_len;
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

    int port_no = 3000;
    printf("Enter the Port Number: ");
    scanf("%d", &port_no);
    server.sin_port = htons(port_no);

    printf("Connecting to Server...\n");
    int c = connect(s, (struct sockaddr*) &server, sizeof(server));
    if (c < 0) { printf("\e[1A\e[2KConnecting to Server...\t\t[FAIL]\n"); return 1; }
    else printf("\e[1A\e[2KConnecting to Server...\t\t[OK]\n");

    char prompt[100];
    write(s, "PROMPT", strlen("PROMPT")+1);
    read(s, prompt, 100);

    while(1) {
        char write_buff[100], read_buff[100];
        printf("%s", prompt);
        getstr(write_buff, stdin);
        if (strcmp("QUIT", write_buff) == 0) { close(s); break; }

        int written_size = write(s, write_buff, strlen(write_buff)+1);
        if (strcmp("SHUTDOWN", write_buff) == 0) { close(s); break; }

        int read_loop = 1;
        while (read_loop) {
            int r = read(s, read_buff, 100);
            if (r == 0) break;
            if (read_buff[r-1] == '\0') read_loop = 0;
            else read_buff[r] = '\0';
            printf("%s", read_buff);
        }
        printf("\n");
        
    }

    return 0;
}