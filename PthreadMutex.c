
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>

//全局空间的数据
char buff[128] = {0};

pthread_mutex_t mutex;

void* PthreadFun(void *arg)
{
	while(1)
	{

		//要使用临界资源，则对其进行加锁操作
		pthread_mutex_lock(&mutex);

		if(strncmp(buff, "end", 3) == 0)
		{
			break;
		}
		
		printf("fun:%s\n",buff);
		memset(buff, 0, 128);

		int n = rand() % 3 + 1;
		sleep(n);


		pthread_mutex_unlock(&mutex);
		n = rand()%3 +1;// 保证主线程能够加锁成功
		sleep(n);
	}
}

int main()
{
	srand((unsigned int)(time(NULL) * time(NULL)));
	//初始化的锁是解锁状态的
	pthread_mutex_init(&mutex, NULL);

	//创建线程
	pthread_t id;
	int res = pthread_create(&id, NULL, PthreadFun, NULL);
	assert(res == 0);

	//主线程完成获取用户数据的数据，并存储在全局数组buff中
	while(1)
	{
		pthread_mutex_lock(&mutex);
		printf("please input data: ");

		fgets(buff, 128, stdin);//将输入的数据往buff里面存放

		pthread_mutex_unlock(&mutex);

		if(strncmp(buff, "end", 3) == 0)
		{
			break;
		}
		int n = rand()%3+1;
		sleep(n);
	}


}
