#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
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
    int s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0) { printf("\e[1A\e[2KCreating Socket...\t\t[FAIL]\n"); return 1; }
    else printf("\e[1A\e[2KCreating Socket...\t\t[OK]\n");

    int port_no = 3000;
    printf("Enter the Port Number: ");
    scanf("%d", &port_no);
    server.sin_port = htons(port_no);
	
	char prompt[100];
	socklen_t slen = sizeof(server);
	sendto(s, "PROMPT", strlen("PROMPT")+1, 0, (struct sockaddr *) &server, sizeof(server));
	recvfrom(s, prompt, 100, 0, (struct sockaddr *) &server, &slen);

	while (1) {
		char write_buff[100], read_buff[100];
		
		printf("%s", ">> ");
		getstr(write_buff, stdin);
		
		if (strcmp("QUIT", write_buff) == 0) break;
		sendto(s, write_buff, strlen(write_buff)+1, 0, (struct sockaddr *) &server, sizeof(server));
		
		if (strcmp("SHUTDOWN", write_buff) == 0) break;
		recvfrom(s, read_buff, 100, 0, (struct sockaddr *) &server, &slen);
		
		printf("%s\n\n", read_buff);
	}
	
	close(s);
}

