#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <string.h>

#define SERV_PORT 8000

int main(int argc, char * argv[])
{
	struct sockaddr_in serv_addr;
	int sockfd, n;

	char buf[BUFSIZ];

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

	bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	while(fgets(buf, BUFSIZ, stdin)!= NULL)
	{
		n = sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
		if(n == -1)
		{
			perror("sendto error");
			exit(1);
		}
		n = recvfrom(sockfd, buf, BUFSIZ, 0, NULL, 0);
		if(n == -1)
		{
			perror("recvfrom error");
			exit(1);
		}
		write(STDOUT_FILENO, buf, n);
	}
	close(sockfd);

	return 0;
}



