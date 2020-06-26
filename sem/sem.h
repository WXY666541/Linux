#pragma once
#include<sys/sem.h>

union semval
{
	int val;
};

int CreateSem(int key,int init_val[],int len);
void SemP(int semid,int index);
void SemV(int semid,int index);
void Delete(int semid);

