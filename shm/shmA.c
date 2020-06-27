#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<unistd.h>
#include<time.h>

#include<sys/shm.h>
#include"./sem.h"
#include"./sem.c"

int main()
{
	//获取一个共享内存
	int shmid = shmget((key_t)1234,128,IPC_CREAT|0664);
	assert(shmid != -1);

	//做类型强转
	char* ptr = (char*)shmat(shmid,NULL,0);
	assert(ptr != NULL);

	//创建两个信号量做同步控制
	int init_val[2] = {0,1};
	int semid = CreateSem(1234,init_val,2);
	assert(semid != -1);

	while(1)
	{
		SemP(semid,1);
		printf("intput:");
		fgets(ptr,127,stdin);
		SemV(semid,0);
		if(strncmp(ptr,"end",3) == 0)
		{
			break;
		}
	}
	//断开一个链接
	shmdt(ptr);	
	//删除共享内存
	shmctl(shmid,IPC_RMID,NULL);
}

