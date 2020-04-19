#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<unistd.h>
#include<fcntl.h>

//写入数据
int main()
{
	int fd = open("./FIFO",O_WRONLY);
	assert(fd != -1);

	printf("Write open fifo success\n");

	//获取操作者输入
	while(1)
	{
		char buff[128] = {0};
		printf("input: ");
		fgets(buff,127,stdin);

		if(strncmp(buff,"end",3) == 0)
		{
			break;
		}

		write(fd,buff,strlen(buff)-1);
	}
	close(fd);
}
