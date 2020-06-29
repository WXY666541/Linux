#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<unistd.h>
#include<string.h>

#include<pthread.h>

int g = 0;
void* fun(void* arg)
{
	int i = 0;
	for(;i<10000;i++)
	{
		g++;
	}
}
int main()
{
	pthread_t id;
	int res = pthread_create(&id,NULL,fun,NULL);
	assert(res == 0);

	int i = 0;
	for(;i<10000;i++)
	{
		g++;
	}

	pthread_join(id,NULL);//保证函数线程已经结束了
	printf("g = %d\n",g);
	exit(0);
}
