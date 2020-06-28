#include<stdio.h>
#include<assert.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

pthread_rwlock_t rwlock;
char buff[128] = {0};

void *fun(void* arg)
{
	while(1)
	{
		pthread_rwlock_rdlock(&rwlock);
		printf("fun :%s\n",buff);
		sleep(1);
		pthread_rwlock_unlock(&rwlock);
		sleep(1);
	}
}

int main()
{
	pthread_t id;
	pthread_rwlock_init(&rwlock,NULL);//初始化一个读写锁

	pthread_create(&id,NULL,fun,NULL);

	while(1)
	{
		pthread_rwlock_wrlock(&rwlock);
		printf("input:");
		fgets(buff,128,stdin);

		pthread_rwlock_unlock(&rwlock);
		sleep(2);
	}

	pthread_join(id,NULL);
	pthread_rwlock_destroy(&rwlock);
}
