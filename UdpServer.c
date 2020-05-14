#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main()
{
	int sockfd = socket(AF_INET,  SOCK_DGRAM, 0);
	assert(-1 != sockfd);

	struct sockaddr_in ser;
	memset(&ser, 0, sizeof(ser));
	ser.sin_family = AF_INET;
	ser.sin_port = htons(6500);
	ser.sin_addr.s_addr = inet_addr("127.0.0.1");

	int res = bind(sockfd,  (struct sockaddr*)&ser, sizeof(ser));
	assert(res != -1);
	
	while(1)
	{
		char buff[128] = {0};
		struct sockaddr_in cli;
		socklen_t len = sizeof(cli);

		int n = recvfrom(sockfd, buff, 127, 0, (struct sockaddr*)&cli, &len);
		if(n <= 0)
		{
			printf("recvfrom error\n");
			continue;
		}

		printf("%s:%d --> %s\n", inet_ntoa(cli.sin_addr), ntohs(cli.sin_port),buff);
		
		int res = sendto(sockfd, "OK", 2, 0, (struct sockaddr*)&cli, len);
		if(res <= 0)
		{
			printf("sendto error\n");
			continue;
		}
		close(sockfd);
		exit(0);
	}
}