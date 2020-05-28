#define _GNU_SOURCE

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<assert.h>

#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <semaphore.h>
#include <fcntl.h>
#include <errno.h>

#define EVENTSIZE 128
#define PTHREADNUM 5  // 线程池中线程的数量
#define ARRAYSIZE 128

pthread_t id[PTHREADNUM];

pthread_mutex_t mutex;
//定义信号量
sem_t sem;
int Buff[ARRAYSIZE];

void InitBuff()
{
	int i = 0;
	for(; i < ARRAYSIZE; ++i)
	{
		Buff[i] = -1;
	}
}
void AddBuff(int fd)
{
	pthread_mutex_lock(&mutex);
	int i = 0;
	for(; i < ARRAYSIZE; ++i)
	{
		if(Buff[i] == -1)
		{
			Buff[i] = fd;
			break;
		}
	}

	if(i == ARRAYSIZE)
	{
		printf("Buff Full\n");
	}

	pthread_mutex_unlock(&mutex);
}

// 获取一个有效的文件描述符并将Buff中的位置置为-1
int GetBuff()
{
	pthread_mutex_lock(&mutex);
	int i = 0;
	int c = -1;
	for(; i < ARRAYSIZE; ++i)
	{
		if(Buff[i] != -1)
		{
			c = Buff[i];
			Buff[i] = -1;
			break;
		}
	}

	pthread_mutex_unlock(&mutex);
	return c;
}

// 所有的函数线程的主逻辑: 1、为一个客户端服务  2、处理一次就绪事件
void * work_pthread(void *arg)
{
	//UNDO
	int index = (int)arg;
	while(1)//循环的处理交付给其的事件
	{
		sem_wait(&sem); // 信号量的P操作
		int fd = GetBuff();
		if(fd == -1)
		{
			printf("GetBuff error\n");
			continue;
		}

		while(1)
		{
			char buff[128] = {0};
			int n = recv(fd, buff, 127, 0);
			if(n == -1 && errno == EAGAIN)
			{
				printf("later\n");
				break;
			}

			printf("%d::%d: %s\n",index, fd, buff);
		}

		send(fd, "OK", 2, 0);
	}
}

//创建线程
void CreatePthread()
{
	int i = 0;
	for(; i < PTHREADNUM; ++i)
	{
		int res = pthread_create(&id[i], NULL, work_pthread, (void*)i);
		assert(res == 0);
	}
}

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
	if(c == -1)
	{
		printf("Get new link error\n");
		return;
	}
	printf("Get new link success\n");

	int old_option = fcntl(c, F_GETFL);
	int new_option = old_option | O_NONBLOCK;
	fcntl(c, F_SETFL, new_option);

	struct epoll_event event;
	event.data.fd = c;
	event.events = EPOLLIN | EPOLLRDHUP | EPOLLET;//事件关注的类型

	int res = epoll_ctl(epfd, EPOLL_CTL_ADD, c, &event);
	assert(res != -1);
}

void CloseLink(int fd, int epfd)
{
	int res = epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
	assert(res != -1);
	close(fd);
}

void DealReadyEvent(int epfd, struct epoll_event *events, int n, int listenfd)
{
	int i = 0;
	for(; i < n; ++i)
	{
		int fd = events[i].data.fd;
		if(fd == listenfd)
		{
			GetNewLink(fd, epfd);
		}
		else//普通的文件描述符
		{
			if(events[i].events & EPOLLRDHUP)//断开连接的处理
			{
				CloseLink(fd, epfd);
			}
			else//有数据到达，把其交付给工作线程处理
			{
				AddBuff(fd);
				sem_post(&sem);//执行一次V操作，唤醒线程
			}
		}
	}
}

int main()
{
	pthread_mutex_init(&mutex, NULL);
	sem_init(&sem, 0, 0);
	InitBuff();
	CreatePthread();

	int listenfd = CreateSocket();
	assert(listenfd != -1);

	//创建一个epfd
	int epfd = epoll_create(5);
	assert(epfd != -1);

	struct epoll_event event;
	event.data.fd = listenfd;
	event.events = EPOLLIN;//事件关注的类型

	//将listenfd添加到内核事件表中去
	int res = epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &event);
	assert(res != -1);

	while(1)
	{
		struct epoll_event events[EVENTSIZE];
		int n = epoll_wait(epfd, events, EVENTSIZE, -1);//监听就绪事件
		if(n <=0 )
		{
			printf("epoll_wait error\n");
			break;
		}
		//处理就绪的事件,发现就绪事件扔给工作线程处理
		DealReadyEvent(epfd, events, n, listenfd);
	}

	exit(0);
}
