#include  <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>    //  IP地址转换函数
#include <netinet/in.h>   //  字节序转换函数


int main()
{
	int sockfd = socket(AF_INET, SOCK_STREAM,  0);
	assert(-1 != sockfd);

	struct sockaddr_in ser;
	memset(&ser, 0, sizeof(ser));
	ser.sin_family = AF_INET;
	ser.sin_port = htons(6000);
	ser.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	int res = connect(sockfd,  (struct sockaddr*)&ser, sizeof(ser));
	assert(-1 != res);

	while(1)
	{
		printf("please input: ");
		char data[128] = {0};
		fgets(data, 127, stdin);
		if(strncmp(data, "bye", 3) == 0)
		{
			break;
		}

		int num = send(sockfd, data, strlen(data) - 1, 0);
		assert(num != -1）；
		if(num == 0)
		{
			printf("the length that send is zero\n");
			break;
		}
		
		char buff[128] = {0};
		int n = recv(sockfd, buff, 127, 0);
		assert(n != -1);
		if(n == 0)
		{
			printf("error\n");
			break;
		}

		printf("recv data); is %s\n",buff);
	}
	close(sockfd);
	exit(0);
}