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
//��ȡ�µĿͻ������ӣ����ҽ�����ӵ�epfdָ�����ں��¼�����
void GetNewClientLink(int sockfd,int epfd)
{
    struct sockaddr_in cli;
    socklen_t len=sizeof(cli);
    int c=accept(sockfd,(struct sockaddr*)&cli,&len);
    if(c<0) return;
    printf("one clink\n");
    //�û�ʹ�õ�
    struct epoll_event event;
    //�¼�����
    event.events=EPOLLIN|EPOLLRDHUP|EPOLLET;
    event.data.fd=c;
    //���¼����뵽�ں��¼�����
    int res=epoll_ctl(epfd,EPOLL_CTL_ADD,c,&event);
    assert(res!=-1);
    //��C����Ϊ��������ʽ
    int oldoption= fcntl(c,F_GETFL);
    int newoption=oldoption| O_NONBLOCK;//�µı�־λ��Ϊ������״̬��
    fcntl(c,F_SETFL,newoption);//�����µ�״̬
}
void DealClientData(int fd)
{
    while(1)
    {
        char buff[128]={0};
        int n=recv(fd,buff,1,0);//���fd����Ϊ��������ʽ����recv�������������û����������recv����-1�����һ�����ȫ�ֵ�errno
        if(n==0)
        {
            printf("%d recv error\n",fd);
            break;
        }
        else if(n==-1)
        {
            if(errno==EAGAIN||errno==EWOULDBLOCK)//�������ʾû�����ݿ��Զ����������Ѿ���ȡ��ϣ���epoll�����ٴδ���sockfd�ϵ�EPOLLIN�¼�����������һ�ζ�������
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
//��һ��Ϊ�����¼����飬n��ʾ�����¼�������sockfd�����ж��Ǽ������ӻ����������ӣ�epfd������������ʱ����ӵ��ں��¼�����
void DealFinishEvents(struct epoll_event *events,int n,int sockfd,int epfd)
{
    int i=0;
    for(;i<n;i++)
    {
        int fd=events[i].data.fd;
        if(fd==sockfd)//��������
        {
            GetNewClientLink(sockfd,epfd);
        }
        else
        {
            if(events[i].events&EPOLLRDHUP)//�ͻ��˶Ͽ�����
            {
                close(fd);
                epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);//ɾ���ں��¼�����¼�
                printf("one over\n");
            }
            else
            {
                DealClientData(fd);//��������
            }

        }
    }
}
int main()
{
    int sockfd=InitSocket();
    assert(sockfd!=-1);
    //�����ں��¼�������Ϊ��
    int epfd=epoll_create(5);
    assert(epfd!=-1);
    //����û��¼����ͺ��ļ��������Ľṹ��
    struct epoll_event event;
    event.events=EPOLLIN;//�¼�����Ϊ�ɶ��¼�
    event.data.fd=sockfd;//�ļ�������Ϊsockfd
    //���ļ���������ӽ��ں��¼�����
    int res=epoll_ctl(epfd,EPOLL_CTL_ADD,sockfd,&event);
    assert(res!=-1);
    int count=0;
    while(1)
    {
        //�洢�ں���д�ľ����¼������������͵�����
        struct epoll_event events[MAXEVENTS];
        //�����ں��¼���
        int n=epoll_wait(epfd,events,MAXEVENTS,-1);//����
        printf("epoll_wait return %d\n",count++);//��¼����
        if(n<=0)
        {
            printf("epoll_wait error\n");
            continue;
        }
        DealFinishEvents(events,n,sockfd,epfd);
    }
}


