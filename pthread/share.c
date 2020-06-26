#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<pthread.h>
#include<string.h>

#include<pthread.h>
#include<fcntl.h>

int gdata = 10;//验证.text段数据
int fd = -1;//验证文件描述符

int *ptr = NULL;//验证.heap段数据

void *fun(void *arg)
{
	printf("gdata = %d\n",gdata);
	gdata = 20;
	printf("gdata = %d\n",gdata);

	printf("*ptr = %d\n",*ptr);
	*ptr = 20;
	printf("*ptr = %d\n",*ptr);

	fd = open("main.c",O_RDONLY);
	assert(fd != -1);
}

int main()
{
	ptr = (int*)malloc(4);
	*ptr = 10;

	//创建一个线程
	pthread_t id;
	int res = pthread_create(&id,NULL,fun,NULL);
	assert(res == 0);

	sleep(2);
	printf("main gdata = %d\n",gdata);
	printf("main *ptr = %d\n",*ptr);

	char buff[128] = {0};
	int n = read(fd ,buff,10);
	assert(n != -1);
	printf("read data :%s\n",buff);
}


