#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<unistd.h>
#include<string.h>

#include<pthread.h>

pthread_mutex_t mutex;

void *fun(void *arg)
{
	sleep(1);
	pid_t pid = fork();//调用fork时，mutex应该是加锁状态的
	assert(pid != -1);

	if(pid == 0)
	{
		printf("i am child,my pid = %d\n",getpid());
		pthread_mutex_lock(&mutex);
		printf("child lock success\n");
		sleep(2);
		pthread_mutex_unlock(&mutex);
		printf("child unlock success\n");
	}
	else
	{
		printf("i am father and fun,my pid = %d\n",getpid());
		pthread_mutex_lock(&mutex);
		printf("father lock success\n");
		sleep(2);
		pthread_mutex_unlock(&mutex);
		printf("father unlock success\n");
	}
}

int main()
{
	pthread_mutex_init(&mutex,NULL);
	printf("father pid = %d\n",getpid());
	pthread_t id;
	int res = pthread_create(&id,NULL,fun,NULL);
	assert(res == 0);

	pthread_mutex_lock(&mutex);
	sleep(1);
	int i = 0;
	for(;i<5;i++)
	{
		printf("i am main,i pid = %d\n",getpid());
		sleep(1);
	}
	pthread_mutex_unlock(&mutex);
	printf("main unlock success\n");
	pthread_exit(NULL);
}
