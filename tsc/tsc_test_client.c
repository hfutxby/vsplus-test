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

#if 1
	struct msg_head head = {0};   
	head.type = SET_PRG;
	head.len = 1;
	int prg_id = 0;
	printf("set prg: ");
	scanf("%d", &prg_id);
	write(sock_fd, &head, sizeof(struct msg_head));
	write(sock_fd, &prg_id, sizeof(prg_id));
#endif

#if 0
	struct msg_head head = {0};   
	head.type = TEST;
	write(sock_fd, &head, sizeof(struct msg_head));
#endif
	close(sock_fd);

	return 0;
}

