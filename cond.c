#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<unistd.h>

#include<pthread.h>

char buff[128] = {0};

pthread_cond_t cond;
pthread_mutex_t mutex;

void *fun(void *arg)
{
	char* s = (char*)arg;
	while(1)
	{
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&cond,&mutex);
		pthread_mutex_unlock(&mutex);

		if(strncmp(buff,"end",3) == 0)
		{
			break;
		}
		printf("%s:%s",s,buff);
	}
}
int main()
{
	pthread_mutex_init(&mutex,NULL);
	pthread_cond_init(&cond,NULL);

	pthread_t id[2];
	int res = pthread_create(&id[0],NULL,fun,"thread1");
	assert(res == 0);
	res = pthread_create(&id[1],NULL,fun,"thread2");
	assert(res == 0);

	while(1)
	{
		printf("input:");
		fgets(buff,127,stdin);
		//唤醒在条件变量cond上等待的所有线程
		if(strncmp(buff,"end",3) == 0)
		{
			pthread_mutex_lock(&mutex);
			pthread_cond_broadcast(&cond);
			pthread_mutex_unlock(&mutex);
		}
		//唤醒在条件变量上等待的一个线程
		else
		{
			pthread_mutex_lock(&mutex);
			pthread_cond_signal(&cond);
			pthread_mutex_unlock(&mutex);
		}
	}

	pthread_join(id[0],NULL);
	pthread_ioin(id[1],NULL);

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);

	exit(0);
}

