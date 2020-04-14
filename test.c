
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<assert.h>

int main(int argc,char *argv[])
{
	printf("i am test:%d\n",getpid());
	int i =0;
	for(;i<argc;++i)
	{
		printf("argv[%d] = %s\n",i,argv[i]);
	}
	exit(0);
}
