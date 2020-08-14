#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <stdlib.h>

#define SERV_PORT 8000

int main(void)
{
	struct sockaddr_in serv_addr, clit_addr;
	socklen_t clit_addr_len = sizeof(clit_addr);
	char buf[BUFSIZ];
	char str[INET_ADDRSTRLEN];
	int n, i;

	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_PORT);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	printf("Accepting connections...\n");

	while(1)
	{
		n = recvfrom(sockfd, buf, BUFSIZ, 0, (struct sockaddr*)&clit_addr, &clit_addr_len);
		if (n == -1)
		{
			perror("recvfrom error");
			exit(1);
		} 
		printf("Received from %s at port %d\n", inet_ntop(AF_INET, &clit_addr.sin_addr.s_addr, str, sizeof(str)), ntohs(clit_addr.sin_port));

		for (i = 0; i < n; i++)
		{
			buf[i] = toupper(buf[i]);
		}

		n = sendto(sockfd, buf, n, 0, (struct sockaddr*)&clit_addr, sizeof(clit_addr));
		if (n == -1)
		{
			perror("sendto error");
			exit(1);
		}
	}
	close(sockfd);
	return 0;

}


