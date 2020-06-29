#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<unistd.h>

#include<pthread.h>

void *fun(void *arg)
{
	char buff[] = "1 2 3 4 5 6 7 8";
	char* p = strtok(buff," ");
	while(p)
	{
		printf("fun :%s\n",p);
		p = strtok(NULL," ");
		sleep(1);
	}
}
int main()
{
	pthread_t id;
	int res = pthread_create(&id,NULL,fun,NULL);
	assert(res == 0);

	char str[] = "a b c d e f g h";
	char* p = strtok(str," ");
	while(p)
	{
		printf("main :%s\n",p);
		p = strtok(NULL," ");
		sleep(1);
	}
	pthread_exit(0);
}
