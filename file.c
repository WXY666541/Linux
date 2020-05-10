#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<pthread.h>
#include<fcntl.h>

void *fun(void *arg)
{
	*(int*)arg = open("main.c",O_RDONLY);
	if(*(int*)arg != -1)
	{
		printf("fun open file sucess\n");
		return NULL;
	}
	printf("fun open file fail\n");
}
int main()
{
	int fd = -1;
	pthread_t id;
	int res = pthread_create(&id,NULL,fun,(void*)&fd);
	assert(fd == 0);
	sleep(2);
	printf("main:fd=%d\n",fd);
	char buff[128] = {0};
	int n = read(fd,buff,127);
	if(n == -1)
	{
		printf("main read fial\n");
		return 0;
	}
	printf("n = %d\n",n,buff);
}
