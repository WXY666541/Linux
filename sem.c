
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>

char buff[128] = {0};

sem_t sem1;
sem_t sem2;

void* PthreadFun(void *arg)
{
	//函数线程完成将用户输入的数据存储到文件中
	while(1)
	{
		sem_wait(&sem1);//P操作

		if(strncmp(buff, "end", 3) == 0)
		{
			break;
		}

		printf("fun :%s",buff);
		memset(buff ,0,128);
		
		sem_post(&sem2);//函数线程唤醒主线程
	}
}

int main()
{
	sem_init(&sem1, 0, 0);
	sem_init(&sem2, 0, 1);

	pthread_t id;
	int res = pthread_create(&id, NULL, PthreadFun, NULL);
	assert(res == 0);

	//主线程完成获取用户数据的数据，并存储在全局数组buff中
	while(1)
	{
		sem_wait(&sem2);
		printf("please input data: ");

		fgets(buff, 128, stdin);

		sem_post(&sem1);//主线程来唤醒函数线程

		if(strncmp(buff, "end", 3) == 0)
		{
			break;
		}
	}
	
	//等待线程的结束，只有该线程结束了，才能确保没有线程使用这个信号量了
	pthread_join(id,NULL);
	sem_destroy(&sem1);
	sem_destroy(&sem2);
	pthread_exit(NULL);
}

