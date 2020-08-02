# define _GNU_SOURCE
# include<stdio.h>
# include<unistd.h>
# include<string.h>
# include<assert.h>

# include<sys/types.h>
# include<sys/socket.h>
# include<sys/epoll.h>
# include<arpa/inet.h>
# include<netinet/in.h>
# include<fcntl.h>
# include<errno.h>
# define MAXEVENTS 100

int InitSocket()
{
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd==-1) return -1;
    struct sockaddr_in ser;
    memset(&ser,0,sizeof(ser));
    ser.sin_family=AF_INET;
    ser.sin_port=htons(6000);
    ser.sin_addr.s_addr=inet_addr("127.0.0.1");

    int res=bind(sockfd,(struct sockaddr*)&ser,sizeof(ser));
    if(res==-1) return -1;

    res=listen(sockfd,5);
    if(res==-1) return -1;
    return sockfd;
}
//获取新的客户端链接，并且将其添加到epfd指定的内核事件表中
void GetNewClientLink(int sockfd,int epfd)
{
    struct sockaddr_in cli;
    socklen_t len=sizeof(cli);
    int c=accept(sockfd,(struct sockaddr*)&cli,&len);
    if(c<0) return;
    printf("one clink\n");
    //用户使用的
    struct epoll_event event;
    //事件类型
    event.events=EPOLLIN|EPOLLRDHUP|EPOLLET;
    event.data.fd=c;
    //将事件插入到内核事件表中
    int res=epoll_ctl(epfd,EPOLL_CTL_ADD,c,&event);
    assert(res!=-1);
    //将C设置为非阻塞方式
    int oldoption= fcntl(c,F_GETFL);
    int newoption=oldoption| O_NONBLOCK;//新的标志位设为非阻塞状态。
    fcntl(c,F_SETFL,newoption);//设置新的状态
}
void DealClientData(int fd)
{
    while(1)
    {
        char buff[128]={0};
        int n=recv(fd,buff,1,0);//如果fd设置为非阻塞方式，则recv不会阻塞，如果没有阻塞，则recv返回-1，并且会设置全局的errno
        if(n==0)
        {
            printf("%d recv error\n",fd);
            break;
        }
        else if(n==-1)
        {
            if(errno==EAGAIN||errno==EWOULDBLOCK)//满足则表示没有数据可以读或者数据已经读取完毕，则epoll就能再次触发sockfd上的EPOLLIN事件，以驱动下一次读操作。
            {
                printf("read later\n");
                send(fd,"ok",2,0);
                break;
            }
            else 
            {
                printf("%d recv error\n",fd);
                break;
            }
        }
        else
        {
            printf("%d:%s\n",fd,buff);
           
        }
    }
}
//第一个为就绪事件数组，n表示就绪事件个数，sockfd用来判断是监听链接还是数据链接，epfd是有新链接来时，添加到内核事件表中
void DealFinishEvents(struct epoll_event *events,int n,int sockfd,int epfd)
{
    int i=0;
    for(;i<n;i++)
    {
        int fd=events[i].data.fd;
        if(fd==sockfd)//监听链接
        {
            GetNewClientLink(sockfd,epfd);
        }
        else
        {
            if(events[i].events&EPOLLRDHUP)//客户端断开链接
            {
                close(fd);
                epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);//删除内核事件表的事件
                printf("one over\n");
            }
            else
            {
                DealClientData(fd);//处理数据
            }

        }
    }
}
int main()
{
    int sockfd=InitSocket();
    assert(sockfd!=-1);
    //创建内核事件表，现在为空
    int epfd=epoll_create(5);
    assert(epfd!=-1);
    //存放用户事件类型和文件描述符的结构体
    struct epoll_event event;
    event.events=EPOLLIN;//事件类型为可读事件
    event.data.fd=sockfd;//文件描述符为sockfd
    //把文件描述符添加进内核事件表中
    int res=epoll_ctl(epfd,EPOLL_CTL_ADD,sockfd,&event);
    assert(res!=-1);
    int count=0;
    while(1)
    {
        //存储内核填写的就绪事件描述符和类型的数组
        struct epoll_event events[MAXEVENTS];
        //监听内核事件表
        int n=epoll_wait(epfd,events,MAXEVENTS,-1);//监听
        printf("epoll_wait return %d\n",count++);//记录次数
        if(n<=0)
        {
            printf("epoll_wait error\n");
            continue;
        }
        DealFinishEvents(events,n,sockfd,epfd);
    }
}


