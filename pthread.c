
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<unistd.h>
#include<pthread.h>

void *fun(void*);

int main()
{
	pthread_t id;
	int res = pthread_create(&id,NULL,fun,NULL);
	assert(res == 0);

	int i =0;
	for(;i<5;i++)
	{
		printf("main running\n");
		sleep(1);
	}
	exit(0);
}

void *fun(void *arg)
{
	int i =0;
	for(;i<3;i++)
	{
		printf("fun running\n");
		sleep(1);
	}
}

