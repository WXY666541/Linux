#include"msg.h"
int main()
{
	MsgData data;
	memset(&data,0,sizeof(data));//防止对下面数据有影响
	data.mtype=100;
	strcpy(data.mdata,"hello");

	//创建一个消息
	int msgid = msgget((key_t)1234,0664|IPC_CREAT);
	assert(msgid != -1);

	//发送一个消息
	msgsnd(msgid,&data,strlen(data.mdata),0);
	exit(0);
}
