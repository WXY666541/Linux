/*************************************************************************
    > File Name: tcpser.c
    > Author: wangchen
    > Mail: w_chen1024@qq.com 
    > Created Time: Sat 18 Apr 2020 11:02:10 AM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<assert.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);
	assert(listenfd != -1);

	struct sockaddr_in ser_addr;
	memset(&ser_addr, 0, sizeof(ser_addr));
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(6000);
	ser_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // "127.0.0.1"回环地址

	int res = bind(listenfd, (struct sockaddr*)&ser_addr, sizeof(ser_addr));
	assert(res != -1);

	res = listen(listenfd, 5);
	assert(res != -1);

	// 循环接收客户端连接并处理
	while(1)
	{
		struct sockaddr_in cli_addr;
		socklen_t  len = sizeof(cli_addr);

		int clientLinkFd = accept(listenfd, (struct sockaddr*)&cli_addr, &len);
		if(clientLinkFd == -1)
		{
			printf("one client link error\n");
			continue;
		}

		printf("one clinet success -- %s:%d\n",inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

		// 处理一个客户端与服务器交互的数据
		while(1)
		{
			char buff[128] = {0};
			int num = recv(clientLinkFd, buff, 127, 0);
			if(num == -1)
			{
				printf("recv error\n");
				break;
			}
			else if(num == 0)
			{
				printf("client over\n");
				break;
			}

			printf("recv data is : %s\n", buff);

			char *restr = "recv data success";
			num = send(clientLinkFd, restr, strlen(restr), 0);
			if(num == -1)
			{
				printf("send data error\n");
				break;
			}
		}

		close(clientLinkFd);
	}

	close(listenfd);
	exit(0);
}
