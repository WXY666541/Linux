#define _GNU_SOURCE
/*************************************************************************
    > File Name: poll.c
    > Author: wangchen
    > Mail: w_chen1024@qq.com 
    > Created Time: Tue 28 Apr 2020 07:18:40 PM CST
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
#include <sys/epoll.h>

#define SIZE 100

int CreateSocket()
{
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if(listenfd == -1) return -1;

	struct sockaddr_in ser_addr;
	memset(&ser_addr, 0, sizeof(ser_addr));
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(6000);
	ser_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	int res = bind(listenfd, (struct sockaddr*)&ser_addr, sizeof(ser_addr));
	if(res == -1) return -1;

	res = listen(listenfd, 5);
	if(res == -1) return -1;

	return listenfd;
}

void GetNewLink(int fd, int epfd)
{
	struct sockaddr_in cli_addr;
	socklen_t len = sizeof(cli_addr);

	int c = accept(fd, (struct sockaddr*)&cli_addr, &len);
	assert(c != -1);

	struct epoll_event event;
	event.data.fd = c;
	event.events = EPOLLIN | EPOLLRDHUP;

	int res = epoll_ctl(epfd, EPOLL_CTL_ADD, c, &event);
	assert(res != -1);

	printf("%d link success\n", c);
}

void CloseClientLink(int fd, int epfd)
{
	int res = epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
	assert(res != -1);
	close(fd);
}

void DealOneClientData(int fd, int epfd)
{
	char buff[128] = {0};
	int n = recv(fd, buff, 127, 0);
	if(n <= 0)
	{
		CloseClientLink(fd, epfd);
		return;
	}

	printf("%d: %s\n", fd, buff);
	send(fd, "OK", 2, 0);
}

void DealReadyEvent(struct epoll_event *events, int n, int listenfd, int epfd)
{
	int i = 0;
	for(; i < n; ++i)
	{
		int fd = events[i].data.fd;  //

		if(fd == listenfd)
		{
			GetNewLink(fd, epfd);
		}
		else
		{
			if(events[i].events & EPOLLRDHUP)
			{
				CloseClientLink(fd, epfd);
				printf("one client unlink\n");
			}
			else if(events[i].events & EPOLLIN)
			{
				DealOneClientData(fd, epfd);
			}
			else
			{
				printf("error\n");
			}
		}
	}
}

int main()
{
	int listenfd = CreateSocket();
	assert(listenfd != -1);

	int epfd = epoll_create(5);
	assert(epfd != -1);

	struct epoll_event event;
	event.events = EPOLLIN;
	event.data.fd = listenfd;
	int res = epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &event);
	assert(res != -1);

	while(1)
	{
		struct epoll_event events[SIZE]; //  数组不用填充什么值，因为其作用又内核填充所有就绪的事件
		int n = epoll_wait(epfd, events, SIZE, -1);
		assert(n != -1);

		if(n == 0)
		{
			printf("time out\n");
			continue;
		}

		DealReadyEvent(events, n, listenfd, epfd);
	}

	close(listenfd);
	exit(0);
}

