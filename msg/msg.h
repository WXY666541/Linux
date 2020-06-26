#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<unistd.h>

#include<sys/msg.h>

#pragma once
typedef struct msgdata
{
	long mtype;
	char mdata[128];
}MsgData;

