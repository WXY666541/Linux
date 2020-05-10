
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<pthread.h>
#include<malloc.h>

int gdata = 10;
int *ptr = NULL;

void *fun(void *arg)
{
	int ldata = 10;
	printf("before of change:%d\n",gdata);

	gdata = 20;
	ptr = (int*)malloc(4);
	*ptr = 100;
	printf("after of change:%d\n",gdata);
	printf("fun:ptr = %x\n",ptr);
}
int main()
{
	pthread_t id;
	int res=pthread_create(&id,NULL,fun,NULL);
	assert(res == 0);
	printf("main start\n");
	sleep(2);//保证函数线程能够执行完成
	printf("main:gdata:%d\n",gdata);
	printf("main:ptr=%x,*ptr = %d\n",ptr,*ptr);
}

