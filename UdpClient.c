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
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	assert(-1 != sockfd);

	struct sockaddr_in ser;
	memset(&ser, 0, sizeof(ser));
	ser.sin_family = AF_INET;
	ser.sin_port = htons(6000);
	ser.sin_addr.s_addr = inet_addr("127.0.0.1");

	while(1)
	{
		printf("please input: ");
		char data[128] = {0};
		fgets(data, 128, stdin);

		if(strncmp(data, "bye", 3) == 0)
		{
			break;
		}

		int res = sendto(sockfd, data, strlen(data) - 1, 0, 
			(struct sockaddr*)&ser, sizeof(ser));
		assert(res != -1);
		
		memset(buff,0,128);
		int n = recvfrom(sockfd, buff, 127, 0, NULL, NULL);
		assert(n != -1);
		printf("recvfrom data:%s\n", buff);
	}
	close(sockfd);
	exit(0);
}