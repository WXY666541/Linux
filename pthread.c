#include<stdio.h>
#include<assert.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<pthread.h>

void* fun(void* arg)
{
	int i = 0;
	for(;i<3;i++)
	{
		printf("fun runing\n");
		sleep(1);
	}
}

int main()
{
	pthread_t id;
	int res = pthread_create(&id,NULL,fun,NULL);
	assert(res == 0);

	int i = 0;
	for(;i<5;i++)
	{
		printf("main running\n");
	}
	exit(0);
}
