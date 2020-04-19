#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>

int main()
{
	int fd = open("./FIFO",O_RDONLY);
	assert(fd != -1);

	printf("Read open fifo success\n");

	while(1)
	{
		char buff[128] = {0};

		int n = read(fd,buff,127);
		if(n <= 0)
		{
			break;
		}

		printf("Read: %s\n",buff);
	}
	close(fd);
}
