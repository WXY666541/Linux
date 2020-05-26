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
#include <poll.h>

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

void InitFds(struct pollfd *fds)
{
	int i = 0;
	for(; i < SIZE; ++i)
	{
		fds[i].fd = -1;
	}
}

void AddFd(struct pollfd *fds,  int fd)
{
	int i = 0;
	for(; i < SIZE; ++i)
	{
		if(fds[i].fd == -1)
		{
			fds[i].fd = fd;
			fds[i].events = POLLIN | POLLRDHUP;
			break;
		}
	}
}
void DelFd(struct pollfd  *fds, int fd)
{
	int i = 0;
	for(; i < SIZE; ++i)
	{
		if(fds[i].fd == fd)
		{
			fds[i].fd = -1;
			break;
		}
	}
}

void GetNewLink(struct pollfd *fds, int listenfd)
{
	struct sockaddr_in cli_addr;
	socklen_t len = sizeof(cli_addr);
	int c = accept(listenfd, (struct sockaddr*)&cli_addr, &len);
	if(c == -1)  return;

	printf("%d link success\n", c);
	AddFd(fds, c);
}

void DealOneClientData(struct pollfd *fds, int fd)
{
	char buff[128] = {0};
	int n = recv(fd, buff, 127, 0);
	if(n <= 0)
	{
		close(fd);
		DelFd(fds, fd);
		return;
	}

	printf("%d: %s\n", fd, buff);

	send(fd, "OK", 2, 0);
}

void DealReadyEvent(struct pollfd *fds, int listenfd)
{
	int i = 0;
	for(; i < SIZE; ++i)
	{
		if(fds[i].fd == -1) continue;
		
		if(fds[i].fd == listenfd && fds[i].revents & POLLIN)
		{
			GetNewLink(fds, listenfd);
		}
		else if(fds[i].revents & POLLRDHUP)
		{
			close(fds[i].fd);
			fds[i].fd = -1;
			printf("one client unlink\n");
		}
		else if(fds[i].revents & POLLIN)
		{
			DealOneClientData(fds, fds[i].fd);
		}
	}
}

int main()
{
	int listenfd = CreateSocket();
	assert(listenfd != -1);

	struct pollfd fds[SIZE];
	InitFds(fds);
	fds[0].fd = listenfd;
	fds[0].events = POLLIN;

	while(1)
	{
		int n = poll(fds, SIZE, -1);
		assert(n != -1);
		if(n == 0)
		{
			printf("time out\n");
			continue;
		}

		DealReadyEvent(fds, listenfd);
	}

	close(listenfd);
	exit(0);
}
