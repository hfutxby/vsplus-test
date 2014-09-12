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

unsigned short portnum = 0x8888; 


int main(int argc, char* argv[])
{
	if(argc != 2){
		printf("usage: %s ipaddr\n", argv[0]);
		return -1;
	}

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
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_port = htons(portnum);

	if(-1 == connect(sock_fd, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr)))
	{
		printf("connect fail !\r\n");
		return -1;
	}
	printf("connect ok !\r\n");

	struct test_msg msg = {0};   
	struct set_det_data *data = (struct set_det_data*)msg.data;
	int index, op;
	while(1){
		printf("input det num:");
		scanf("%d", &index);
		printf("op: 1 = rising; 2 = falling; 3 = fault;");
		scanf("%d", &op);
		memset(&msg, 0, sizeof(msg));
		msg.type = SET_DET;
		data->id = index;
		data->stat = op;
		write(sock_fd, &msg, sizeof(msg));
	}

	close(sock_fd);

	return 0;
}

