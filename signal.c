
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<assert.h>

#include<signal.h>

void fun(int sign)
{
	printf("hello world: %d\n",sign);
}

int main()
{
	signal(SIGINT,fun);

	while(1)
	{
		sleep(5);
		printf("i am main,running \n");
	}
}

