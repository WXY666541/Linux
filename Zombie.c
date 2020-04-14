/*
* Copyright (c) 2007-2018, 图论科技 All rights reserved.
* @Author : 王晨
* @Date : 2018-8-24
* 描述 ：  生成僵死进程，并利用wait函数处理僵死进程
*
*/

// 生成僵死进程
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
/*
int main()
{
	pid_t n = fork();
	assert(-1 != n);

	if(0 == n)
	{
		printf("child start\n");
		sleep(10);
		printf("child end\n");
	}
	else
	{
		// 保证子进程执行结束，父进程未结束
		printf("father starting\n");
		sleep(20);
		printf("father running\n");
		
	}

	exit(0);
}
*/
//在父进程刚开始运行时，调用wait处理僵死进程
int main()
{
	pid_t n = fork();
	assert(-1 != n);

	if(0 == n)
	{
		printf("child start:%d\n",getpid());
		sleep(10);
		printf("child end\n");
	}
	else
	{
		pid_t id = wait(NULL);
		printf("id = %d\n",id);//子进程的pid
		
		printf("father starting\n");
		sleep(20);
		printf("father over\n");

		
	}

	exit(0);
}

// 问题：  wait调用会使父进程阻塞运行，直到子进程退出，很坑？如何解决？

