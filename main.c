
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<assert.h>

int main()
{
	pid_t pid = fork();
	assert(pid != -1);

	if(pid == 0)
	{
		printf("i am child: my pid = %d\n",getpid());
		execl("./test","./test","hello","world",(char *)0);

		int i =0;
		for(;i<5;i++)
		{
			printf("i am child\n");
			sleep(1);
		}
	}

	else
	{
		int i = 0;
		for(;i<10;++i)
		{
			printf("i am father\n");
			sleep(1);
		}
	}
	exit(0);	
}

