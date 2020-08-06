# include<stdio.h>
# include<stdlib.h>
# include<assert.h>
# include<string.h>

int main()
{
    FILE* file=fopen("./a.txt","w+");
    assert(file!=NULL);
    while(1)//循环读取数据
    {
        printf("input:");
        char buff[128]={0};
        fgets(buff,128,stdin);//标准输入
        if(strncmp(buff,"end",3)==0)
        {
            break;
        }
        int n=fwrite(buff,sizeof(char),strlen(buff),file);
        if(n==0)
        {
            printf("fwrite error");
            exit(0);
        }
    }
    printf("------fwrite over----------\n");
    
    int n=fseek(file,0L,SEEK_SET);//重定位
    if(n==-1)
    {
        printf("fseek error");
        exit(0);
    }
    while(1)
    {
        char buff[128]={0};
        int n=fread(buff,sizeof(char),128,file);
        if(n==0)
        {
            printf("------fread over----------\n");
            exit(0);
        }
        else
        {
            
            printf("%s",buff);
        }
    }
    fclose(file);
    exit(0);
}


