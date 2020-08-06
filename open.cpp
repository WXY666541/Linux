# include<stdio.h>
# include<stdlib.h>
# include<assert.h>
# include<string.h>
# include<unistd.h>
# include<fcntl.h>
int main()
{
    int fd=open("./sys.txt",O_RDWR | O_CREAT,0664 );
    assert(fd!=-1);
    while(1)//循环读取数据
    {
        printf("input:");
        char buff[128]={0};
        fgets(buff,128,stdin);//标准输入
        if(strncmp(buff,"end",3)==0)
        {
            break;
        }
        int n=write(fd,buff,strlen(buff));
        if(n<=0)
        {
            printf("write error");
            exit(0);
        }
    }
    printf("------write over----------\n");
    
    int n=lseek(fd,0,SEEK_SET);//重定位
    if(n==-1)
    {
        printf("lseek error");
        exit(0);
    }
    while(1)
    {
        char buff[128]={0};
        int n=read(fd,buff,127);
        if(n==0)
        {
            printf("------read end----------\n");
            break;
        }
        else if(n<0)
        {
            printf("read error\n");
            exit(0);
        }
        else
        {
            
            printf("%s",buff);
        }
    }
    close(fd);
    exit(0);
}


