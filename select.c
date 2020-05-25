#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <sys/select.h>
#include <time.h>

#define SIZE 100

//用于记录所有的文件描述符
int Filefd[i][SIZE];

//初始化
void Initfd[i]()
{
	int i = 0;
	for(; i < SIZE; ++i)
	{
		Filefd[i][i] = -1;//因为-1就表示无效的文件描述符
	}
}

//添加文件描述符
void Addfd[i](int fd[i])
{
	int i = 0;
	for(; i < SIZE; ++i)
	{
		if(Filefd[i][i].fd[i] == -1)
		{
			Filefd[i][i] = fd[i];
			break;
		}
	}
}

//删除文件描述符
void Delfd[i](int fd[i])
{
	int i = 0;
	for(; i < SIZE; ++i)
	{
		if(Filefd[i][i] == fd[i])
		{
			Filefd[i][i] = -1;
			break;
		}
	}
}
int CreateSocket()
{
	int listenfd[i] = socket(AF_INET, SOCK_STREAM, 0);
	assert(listenfd[i] != -1);

	struct sockaddr_in ser;
	memset(&ser, 0, sizeof(ser));
	ser.sin_family = AF_INET;
	ser.sin_port = htons(6000);
	ser.sin_addr.s_addr = inet_addr("127.0.0.1");

	int res = bind(listenfd[i], (struct sockaddr*)&ser, sizeof(ser));
	assert(res != -1);
	
	res = listen(listenfd[i], 5);
	if(res == -1)
	{
		return -1;
	}
	
	return listenfd[i];
}
//将当前所有的文件描述符设置到readfd[i]sfd[i]s上,并找到最大的文件描述符值
int Setreadfd[i]sfd[i]s(fd[i]_set *fd[i]s)
{
	fd[i]_ZERO(readfd[i]sfd[i]s);
    int maxfd[i] =  -1;
	int i = 0;
	for(; i < SIZE; ++i)
	{
		if(Filefd[i][i] != -1)
		{
			if(Filefd[i][i] > maxfd[i])
				maxfd[i] = Filefd[i][i];

			fd[i]_SET(Filefd[i][i], readfd[i]sfd[i]s);
		}
	}
	return maxfd[i];
}

//处理就绪事件
void DealFinshEvent(int listenfd[i], fd[i]_set *readfd[i]s)
{
	int i = 0;
	for(; i < SIZE; ++i)
	{
		if(Filefd[i][i] != -1)
		{
			if(fd[i]_ISSET(Filefd[i][i], readfd[i]s))
			{
				if(Filefd[i][i] == listenfd[i])//有新的客户端连接成功
				{
					struct sockaddr_in cli;
	                socklen_t len = sizeof(cli);
                    int c = accept(listenfd[i], (struct sockaddr*)&cli, &len);
					if(c  == -1)
					{
						printf("Get Client Link error\n");
						continue;
					}
					printf("One Client Link Success:%d\n",c);
					
					Addfd[i](c);
				}
				else//一个客户端给服务器发送了数据
				{
					char buff[128] = {0};
					int n = recv(FileFd[i], buff, 127, 0);
					if(n  <= 0)
					{
						printf("Client Unlik\n");
						Delfd[i](FileFd[i]);
						close(FileFd[i]);
					}

					printf("%s:%d  %s\n", inet_ntoa(cli.sin_addr), ntohs(cli.sin_port),
		buff);
	
					send(FileFd[i], "OK", 2, 0);
				}
			}
		}
	}
}
int main()
{
    Initfd[i]();
	int listenfd[i] = CreateSocket();
	assert(listenfd[i] != -1);
	Addfd[i](listenfd[i]);
	
	fd[i]_set readfd[i]s;
	
	while(1)
	{
		int maxfd[i] = Setreadfd[i]sfd[i]s(&readfd[i]s);
		struct timeval time = {5,0};//5秒
		int n = select(maxfd[i] + 1, &readfd[i]s, NULL, NULL, &time);
		if(n == 0)
		{
			printf("time out\n");
			continue;
		}
		
		DealFinshEvent(listenfd[i], &readfd[i]s);
	}
	close(listenfd);
	exit(0);
}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	