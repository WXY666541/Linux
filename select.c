/*************************************************************************
    > File Name: select.c
    > Author: wangchen
    > Mail: w_chen1024@qq.com 
    > Created Time: Sun 26 Apr 2020 08:25:03 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<assert.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <sys/select.h>
#include <sys/time.h>

#define SIZE 1024

// 用于记录所有的文件描述符
int FileFd[SIZE];

void InitFileFd()
{
	int i = 0;
	for(; i < SIZE; ++i)
	{
		FileFd[i] = -1;
	}
}

void AddFd(int fd)
{
	int i = 0;
	for(; i < SIZE; ++i)
	{
		if(FileFd[i] == -1)
		{
			FileFd[i] = fd;
			break;
		}
	}
}

void DeleteFd(int fd)
{
	int i = 0;
	for(; i < SIZE; ++i)
	{
		if(FileFd[i] == fd)
		{
			FileFd[i] = -1;
			break;
		}
	} 
}

int CreateSocket()
{
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if(listenfd == -1)
	{
		return -1;
	}

	struct sockaddr_in ser_addr;
	memset(&ser_addr, 0 , sizeof(ser_addr));
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(6000);
	ser_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	int res = bind(listenfd, (struct sockaddr*)&ser_addr, sizeof(ser_addr));
	if(res == -1)
	{
		return -1;
	}

	res = listen(listenfd, 5);
	if(res == -1)
	{
		return -1;
	}

	return listenfd;
}

//  将当前所有的文件描述符设置到readfds上, 并找到最大的文件描述符
int SetReadfds(fd_set *readfds)
{
	FD_ZERO(readfds);
	int maxfd = -1;
	int i = 0;
	for(; i < SIZE; ++i)
	{
		if(FileFd[i] != -1)
		{
			FD_SET(FileFd[i], readfds);
			if(FileFd[i] > maxfd)
			{
				maxfd = FileFd[i];
			}
		}
	}

	return maxfd;
}

void DealReadyEvent(fd_set *readfds, int listenfd)
{
	int i = 0;
	for(; i < SIZE; ++i)
	{
		if(FileFd[i] == -1)
		{
			continue;
		}

		if(FD_ISSET(FileFd[i], readfds))
		{
			if(FileFd[i] == listenfd) // 有新的客户端链接成功
			{
				struct sockaddr_in cli_addr;
				socklen_t len = sizeof(cli_addr);
				int c = accept(listenfd, (struct sockaddr*)&cli_addr, &len);
				if( c == -1)
				{
					printf("Get Client Link error\n");
					continue;
				}
				printf("One Client Link Success: %d\n", c);

				AddFd(c);
			}
			else // 一个客户端给服务器发送了数据
			{
				char buff[128] = {0};
				int n = recv(FileFd[i], buff, 127, 0);
				if(n <= 0)
				{
					printf("Client UNlink\n");
					DeleteFd(FileFd[i]);
					close(FileFd[i]);
					continue;
				}

				printf("%d: %s\n",FileFd[i], buff);

				send(FileFd[i], "OK", 2, 0);
			}
		}
	}
}

int main()
{
	InitFileFd();
	int listenfd = CreateSocket();
	assert(listenfd != -1);
	AddFd(listenfd);

	fd_set readfds;

	while(1)
	{
		int maxfd = SetReadfds(&readfds);
		struct timeval time = {5, 0};
		int n = select(maxfd+1, &readfds, NULL, NULL, NULL); // 最后一个参数设置为NULL
		assert(n != -1);
		if(n == 0)
		{
			printf("time out\n");
			continue;
		}

		DealReadyEvent(&readfds, listenfd);
	}

	close(listenfd);
	exit(0);
}
