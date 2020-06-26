#include"msg.h"
int main()
{

	//获取一个消息
	int msgid = msgget((key_t)1234,0664|IPC_CREAT);	
	assert(msgid != -1);

	MsgData data;
	memset(&data,0,sizeof(data));

	msgrcv(msgid,&data,127,100,0);//从消息队列中取出消息类型为100的消息
	printf("data.type:%d\n",data.mtype);
	printf("data.text:%s\n",data.mdata);

	exit(0);
}
