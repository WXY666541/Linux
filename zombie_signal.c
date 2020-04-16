#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

#include<signal.h>

void fun(int sign)
{
	pid_t pid = wait(NULL);
	printf("fun :: pid = %d\n",pid);
}
int main()
{
	signal(SIGCHLD,fun);
	pid_t pid = fork();
	assert(pid != -1);
	if(pid == 0)
	{
		printf("child begin\n");
		sleep(5);
		printf("child over\n");
	}
	else
	{
		printf("father begin\n");
		sleep(10);
		printf("father over\n");
	}
}
