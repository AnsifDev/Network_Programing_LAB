#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
	struct sockaddr_in server, client;
	server.sin_family = AF_INET;
	server.sin_port = htons(3000);
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	
	int s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s >= 0) printf("Socket created sucessfully\n");
	else { printf("Socket creation failed\n"); return 1; }
	
	while (1) {
		char write_buff[100], read_buff[100];
		
		printf(">> ");
		scanf("%s", write_buff);
		
		if (strcmp("QUIT", write_buff) == 0) break;
		sendto(s, write_buff, strlen(write_buff)+1, 0, (struct sockaddr *) &server, sizeof(server));
		
		if (strcmp("SHUTDOWN", write_buff) == 0) break;
		socklen_t slen = sizeof(server);
		recvfrom(s, read_buff, 100, 0, (struct sockaddr *) &server, &slen);
		
		printf("%s\n\n", read_buff);
	}
	
	close(s);
}

