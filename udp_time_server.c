#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>


int fetch_time(char *str) {
	time_t t = time(NULL);
	struct tm *st = localtime(&t);
	sprintf(str, "%d/%d/%d %d:%d:%d", st->tm_mday, st->tm_mon, st->tm_year+1900, st->tm_hour, st->tm_min, st->tm_sec);	
}

int main() {
	struct sockaddr_in server, client;
	server.sin_family = AF_INET;
	server.sin_port = htons(3000);
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	
	int s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s >= 0) printf("Socket created sucessfully\n");
	else { printf("Socket creation failed\n"); return 1; }
	
	for (int port_no = 3000; port_no < 3100; port_no++) {
        server.sin_port = htons(port_no);
        printf("Binding Socket on Port %d...\n", port_no);
        int b = bind(s, (struct sockaddr*) &server, sizeof(server));
        if (b < 0) { printf("\e[1A\e[2KBinding Socket on Port %d...\t\t[FAIL]\n", port_no); }
        else { printf("\e[1A\e[2KBinding Socket on Port %d...\t\t[OK]\n", port_no); break; }
    }
	
	while (1) {
		char read_buff[100], write_buff[100];
		
		socklen_t clen = sizeof(client);
		recvfrom(s, read_buff, 100, 0, (struct sockaddr *) &client, &clen);
		
		if (strcmp("SHUTDOWN", read_buff) == 0) break;

		if (strcmp("PROMPT", read_buff) == 0) {
			sprintf(write_buff, ">> ");
			sendto(s, write_buff, strlen(write_buff)+1, 0, (struct sockaddr *) &client, sizeof(client));
			continue;
		}

		printf("%s\n", read_buff);
		
		if (strcmp("TIME", read_buff) == 0) fetch_time(write_buff);
		else sprintf(write_buff, "Invalid cmd");
		
		sendto(s, write_buff, strlen(write_buff)+1, 0, (struct sockaddr *) &client, sizeof(client));
	}
	
	
	close(s);
}

