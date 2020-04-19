/*************************************************************************
    > File Name: pipe.c
    > Author: wangchen
    > Mail: w_chen1024@qq.com 
    > Created Time: Tue 09 Oct 2018 08:32:11 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<assert.h>

int main()
{
	int fds[2] = {-1,-1};
	//  pipe必须在fork之前调用
	int res = pipe(fds); //  创建并打开一个无名管道，fd[0]为读端，fd[1]为写端

	assert(res != -1);

	pid_t n = fork();

	assert(n != -1);

	if(n == 0)
	{
		close(fds[1]);//子进程直接关闭管道的写端
		while(1)//读取数据放到buff里面
		{
			char buff[128] = {0};
			int n = read(fds[0], buff, 127);
			if(n <= 0 || 0 == strncmp(buff, "end", 3))
			{
				break;
			}

			printf("child: %s\n", buff);
		}
		close(fds[0]);
	}
	else
	{
		close(fds[0]);//父进程中直接关闭管道的读端

		while(1)
		{
			printf("please input:");
			char buff[128] = {0};
			fgets(buff, 128, stdin);

			write(fds[1], buff, strlen(buff) - 1);

			if(strncmp(buff, "end", 3) == 0)
			{
				break;
			}
		}

		close(fds[1]);
	}

}
