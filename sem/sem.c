#include"sem.h"
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<unistd.h>

int CreateSem(int key,int init_val[],int len)
{
	//获取
	int semid = semget((key_t)key,0,0664);
	if(semid != -1)
	{
		return semid;
	}

	//创建
	semid = semget((key_t)key,len,IPC_CREAT|0664);
	if(semid == -1)
	{
		perror("Create Sem Error: ");
		return -1;
	}

	//初始化
	int i = 0;
	for(;i<len;i++)
	{
		union semval data;
		data.val = init_val[i];
		if(-1 == semctl(semid,i,SETVAL,data))
		{
			perror("Init Sem Value Fail:");
			return -1;
		}
	}

	return semid;
}

void SemP(int semid,int index)
{
	struct sembuf buf;
	buf.sem_num = index;
	buf.sem_op = -1;
	buf.sem_flg = SEM_UNDO;

	if(-1 == semop(semid,&buf,1))
	{
		perror("Sem P operation Fail:");
	}
}

void SemV(int semid,int index)
{
	struct sembuf buf;
	buf.sem_num = index;
	buf.sem_op = 1;
	buf.sem_flg = SEM_UNDO;

	if(-1 == semop(semid,&buf,1))
	{
		perror("Sem P operation Fail:");
	}
}

void DeleteSem(int semid)
{
	if(-1 == semctl(semid,0,IPC_RMID))
	{
		perror("Delete Sem Fail: ");
	}
}

