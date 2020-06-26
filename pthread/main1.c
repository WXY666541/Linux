#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<unistd.h>
#include<pthread.h>

void *fun(void*);
int main()
{
	int a = 10;
	pthread_t id;

	//fun并不是函数调用，仅仅是给路一个函数的地址
	int res = pthread_create(&id,NULL,fun,(void*)a);
	assert(res == 0);

	a = 20;

	int i = 0;
	for(;i<5;i++)
	{
		printf("main running\n");
		sleep(1);
	}
	exit(0);
}
void *fun(void *arg)
{
	sleep(1);
	int b = (int)arg;
	printf("b == %d\n",b);
	int i =0;
	for(;i<3;i++)
	{
		printf("fun running\n");
		sleep(1);
	}
}

