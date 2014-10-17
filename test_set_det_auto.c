#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#include "test_msg.h"
#include "tsc.h"

unsigned short portnum = 0x8888; 


int main(int argc, char* argv[])
{
	printf("usage: %s ipaddr. default ip = 127.0.0.1\n", argv[0]);

	int sock_fd;
	struct sockaddr_in server_addr, client_addr;

	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == sock_fd)
	{
		printf("socket fail ! \r\n");
		return -1;
	}
	printf("socket ok !\r\n");

	bzero(&server_addr, sizeof(struct sockaddr_in));
	server_addr.sin_family = AF_INET;
	if(argc != 2)
		server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	else
		server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_port = htons(portnum);

	if(-1 == connect(sock_fd, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr)))
	{
		printf("connect fail !\r\n");
		return -1;
	}
	printf("connect ok !\r\n");

	struct msg_head head = {0};   
	head.type = DET_EXIST;
	write(sock_fd, &head, sizeof(struct msg_head));
	read(sock_fd, &head, sizeof(struct msg_head));
	char *exist = malloc(head.len);
	memset(exist, 0, head.len);
	read(sock_fd, exist, head.len);
	int i;
	printf("len:%d\n", head.len);
	printf("det exist:");
	for(i = 0; i < head.len; i++){
		if(exist[i])
			printf("%2d ", i+1);
	}
	printf("\n");

	int len = sizeof(struct set_det_data);
	head.type = SET_DET;
	head.len = len;
	struct set_det_data *data = (struct set_det_data*)malloc(len);
	int index, op;
	struct timeval tv;
#if 1 //all det occpied
	for(index = 1; index < DETMAX; ++index){
		data->id = index;
		data->stat = RISING;
		write(sock_fd, &head, sizeof(struct msg_head));
		write(sock_fd, data, len);	
	}
#else
	while(1){
		gettimeofday(&tv, NULL);
		srandom(tv.tv_usec);
		index = random() % DETMAX;
		if(!exist[index])//没有这个检测器
			continue;
		gettimeofday(&tv, NULL);
		srandom(tv.tv_usec);
		op = random() % 2 + 1;

		data->id = index+1;//检测器编号1 ~ DETMAX
		data->stat = op;
		printf("index:%d op:%d\n", index+1, op);
		write(sock_fd, &head, sizeof(struct msg_head));
		write(sock_fd, data, len);
		sleep(1);
	}
#endif

	close(sock_fd);

	return 0;
}

