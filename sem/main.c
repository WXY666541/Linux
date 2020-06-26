#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<unistd.h>
#include<time.h>
#include"sem.h"
#include"sem.c"

int main(int argc,char* argv[])
{

	srand((unsigned int)(time(NULL)*time(NULL)));
	int val = 1;
	int semid = CreateSem(1234,&val,1);
	assert(semid != -1);

	int count = 0;

	while(1)
	{
		SemP(semid,0);
		printf("%s\n",argv[1]);
		int n = rand()%4;
		sleep(n);
		printf("%s\n",argv[1]);
		SemV(semid,0);

		n = rand()%4;
		sleep(n);

		count++;
		if(count == 5)
		{
			break;
		}
	}
}

