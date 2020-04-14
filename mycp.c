
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<assert.h>
#include<string.h>

#include<sys/stat.h>
#include<fcntl.h>
//argv[0] = "./mycp"
//argv[1] = "./main.c"
//argv[2] = "./"


int main(int argc,char *argv[])
{
	if(argc < 3)//参数至少都得有三个
	{
		printf("mycp errpr, Parameter is not enough\n");
		exit(0);
	}
	
	int fr = open(argv[1],O_RDONLY);
	if(fr == -1)//打开失败
	{
		perror("open error");
		exit(0);
	}
	
	struct stat st;
	int n =stat(argv[2],&st);//看文件是否是目录文件（因为目录文件不能用open打开，操作方式有些不同）
	char path[128] = {0};
	
	strcpy(path,argv[2]);//把路径拷贝
	
	if(n != -1 && S_ISDIR(st.st_mode))//是目录文件并且文件不存在
	{
		char *p = argv[1]+strlen(argv[1]);//先找到最后，从最后开始搜索
		while(p != argv[1] && *p != '/')
		{
			p--;
		}
		strcat(path,"/");
		strcat(path,p);
	}
	
	int fw = open(path,O_WRONLY | O_CREAT | O_TRUNC,0664);
	if(fw == -1)
	{
		perror("open2 error");
		exit(0);
	}
	
	while(1)
	{
		char buff[128] = {0};
		int num = read(fr,buff,127);
		if(num<= 0)
		{
			break;
		}
		
		int res = write(fw,buff,num);
		if(res <= 0)
		{
			break;
		}
	}
	close(fr);
	close(fw);
}
	
