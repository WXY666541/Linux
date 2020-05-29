#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <event.h>

#define MAX_CLIENT 100
#define DATALENGTH 1024

struct event_base *base = NULL;

typedef struct ClientData
{
	int fd;
	struct event *ev;
}ClientData;

void InitClients(ClientData clients[])
{
	int i = 0;
	for (; i < MAX_CLIENT; ++i)
	{
		clients[i].fd = -1;
		clients[i].ev = NULL;
	}
}

void InsertToClients(ClientData clients[], int fd, struct event *ev)
{
	int i = 0;
	for (; i < MAX_CLIENT; ++i)
	{
		if (clients[i].fd == -1)
		{
			clients[i].fd = fd;
			clients[i].ev = ev;
			break;
		}
	}
}

struct event * DeleteOfClients(ClientData clients[], int fd)
{
	int i = 0;
	for (; i < MAX_CLIENT; ++i)
	{
		if (clients[i].fd == fd)
		{
			clients[i].fd = -1;
			return clients[i].ev;
		}
	}

	return NULL;
}

//³õÊ¼»¯socket
int InitSocket()
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) return -1;

	struct sockaddr_in saddr;
	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(6000);
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	int res = bind(sockfd, (struct sockaddr*)&saddr, sizeof(saddr));
	if (res == -1) return -1;

	res = listen(sockfd, 5);
	if (res == -1) return -1;

	return sockfd;
}

void client_fun(int fd, short event, void *arg)
{
	ClientData *clients = (ClientData*)arg;

	char buff[DATALENGTH] = { 0 };
	int n = recv(fd, buff, DATALENGTH - 1, 0);
	if (n <= 0)
	{
		struct event *ev = DeleteOfClients(clients, fd);
		event_free(ev);
		printf("A Client Disconnect\n");
		return;
	}

	printf("%d:%s\n", fd, buff);
	send(fd, "OK", 2, 0);
}

void sockfd_fun(int fd, short event, void *arg)
{
	ClientData *clients = (ClientData*)arg;
	struct sockaddr_in caddr;
	socklen_t len = sizeof(caddr);
	int c = accept(fd, (struct sockaddr*)&caddr, &len);
	if (c < 0)
	{
		return;
	}

	struct event *ev = event_new(base, c, EV_READ | EV_PERSIST, client_fun, arg);
	InsertToClients(clients, c, ev);
	event_add(ev, NULL);
	printf("A client Link\n");
}


int main()
{
	int sockfd = InitSocket();
	assert(sockfd != -1);

	ClientData clients[MAX_CLIENT];
	InitClients(clients);

	base = event_init();
	
	struct event *ev = event_new(base, sockfd, EV_READ | EV_PERSIST, sockfd_fun, (void*)clients);
	event_add(ev, NULL);

	event_base_dispatch(base);
	event_free(ev);
	event_base_free(base);

	exit(0);
}