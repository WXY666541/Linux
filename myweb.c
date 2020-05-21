#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<unistd.h>

#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>

#include<fcntl.h>
#include<sys/stat.h>

int CreateSocket();

void DealClientLink(int fd);

void SendData(int fd,char *file,int flag);//回馈数据

int main()
{
	int listenfd = CreateSocket();
	assert(listenfd != -1);

	while(1)
	{
		struct sockaddr_in cli_addr;
		socklen_t len = sizeof(cli_addr);
		int c = accept(listenfd,(struct sockaddr*)&cli_addr,&len);
		if(c == -1)
		{
			continue;
		}
		DealClientLink(c);
		close(c);
	}
	close(listenfd);
	exit(0);

}

int CreateSocket()
{
	int listenfd = socket(AF_INET,SOCK_STREAM,0);
	if(listenfd == -1)
	{
		return -1;
	}

	struct sockaddr_in ser_addr;
	memset(&ser_addr,0,sizeof(ser_addr));
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(80);
	ser_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	int res = bind(listenfd,(struct sockaddr*)&ser_addr,sizeof(ser_addr));
	if(res == -1)
	{
		return -1;
	}

	res = listen(listenfd,5);
	if(res == -1)
	{
		return -1;
	}
	return listenfd;
}

void DealClientLink(int fd)
{
	//短链接
	char recvbuff[1024] = {0};
	int n = recv(fd,recvbuff,1023,0);
	if(n <= 0)
	{
		return;
	}
	char *filename = strtok(recvbuff," ");//这个切割GET
	filename = strtok(NULL," ");//这个切割“/index.html"
	char path[128] = "/var/www/html";//在这个路径下寻找index.html
	strcat(path,filename);
	int flag = 0;
	if(0 == access(path,F_OK))//如果文件存在
	{
		filename = path;
		flag = 1;
	}
	else//不存在
	{
		filename = "/var/www/html/404.html";
	}

	SendData(fd,filename,flag);
}

void SendData(int fd,char *file,int flag)
{
	//发送报头
	char sendbuff[128] = "HTTP/1.1 ";
	if(flag)
	{
		strcat(sendbuff,"200 OK\r\n");
	}
	else
	{
		strcat(sendbuff,"404 Not Found\r\n");
	}
	strcat(sendbuff,"Server: MyWeb1.0\r\n");//服务器名称
	strcat(sendbuff,"Content-Length:");//文件长度
	struct stat st;
	stat(file,&st);//文件大小 st.st_size
	sprintf(sendbuff+strlen(sendbuff),"%d",st.st_size);
	strcat(sendbuff,"\r\n");
	strcat(sendbuff,"Content-Type:text/html:charset = utf-8\r\n");
	strcat(sendbuff,"\r\n");

	send(fd,sendbuff,strlen(sendbuff),0);


	//发送页面数据
	int file_fd = open(file,O_RDONLY);
	assert(file_fd != -1);
	while(1)
	{
		char buff[128] = {0};
		int n = read(file_fd,buff,127);
		if(n <= 0)
		{
			break;
		}

		if(send(fd,buff,n,0) <= 0)
		{
			break;
		}
	}
	close(file_fd);
}



