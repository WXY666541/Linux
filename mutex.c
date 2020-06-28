#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<unistd.h>

#include<pthread.h>
#include<time.h>
#include<fcntl.h>

pthread_mutex_t mutex;

char buff[128] = {0};

void *fun(void *arg)
{
	while(1)
	{
		pthread_mutex_lock(&mutex);
		if(strncmp(buff,"end",3) == 0)
		{
			break;
		}

		printf("fun :%s\n",buff);
		memset(buff,0,128);

		int n = rand() % 3 +1;
		sleep(n);

		pthread_mutex_unlock(&mutex);
	    n = rand() % 3 + 1;
		sleep(n);
	}
}

int main()
{
	srand((unsigned int)(time(NULL) * time(NULL)));
	pthread_mutex_init(&mutex,NULL);//初始化的锁是解锁状态的

	pthread_t id;
	int res = pthread_create(&id,NULL,fun,NULL);
	assert(res == 0);

	while(1)
	{
		pthread_mutex_lock(&mutex);
		printf("input:");
		fgets(buff,127,stdin);
		pthread_mutex_unlock(&mutex);
		if(strncmp(buff,"end",3) == 0)
		{
			break;
		}
		int n = rand()%3 + 1;
		sleep(n);
	}
	pthread_join(id,NULL);
	pthread_mutex_destroy(&mutex);
	exit(0);

}

