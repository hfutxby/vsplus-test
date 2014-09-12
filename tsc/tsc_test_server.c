#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>

#include "test_msg.h"

static int g_exit = 0;

struct connect_info{
	int fd;//connect fd
	struct sockaddr_in addr;//client addr
};

int serve_set_det(struct test_msg* msg)
{
	struct set_det_data* ptr = (struct set_det_data*)msg->data;
	tsc_det_op(ptr->id, ptr->stat);

	return 0;
}

int handle_msg(struct test_msg* msg)
{
	printf("msg->type:%d\n", msg->type);
	switch(msg->type){
		case SET_DET:
			serve_set_det(msg);
			break;
		default: 
			break;
	}
}

#define MAXCOUNT 10
void* thr_serve(void* arg)
{
	struct connect_info info;
	int ret = 0;
	struct test_msg msg = {0};
	memcpy(&info, arg, sizeof(struct connect_info));
	printf("connet from %s:%d\n", inet_ntoa(info.addr.sin_addr), ntohs(info.addr.sin_port));

	int count = MAXCOUNT;
	int exit = 0;
	while(!g_exit && !exit){
		memset(&msg, 0, sizeof(msg));
		ret = read(info.fd, &msg, sizeof(msg));
		if(ret == 0){//no data
			usleep(100 * 1000);
			count--;
			if(!count)
				exit = 1;
			else
				continue;
		}
		count = MAXCOUNT;
		printf("read data size:%d\n", ret);
		handle_msg(&msg);
	}
	printf("disconnet from %s:%d\n", inet_ntoa(info.addr.sin_addr), ntohs(info.addr.sin_port));
} 

int thr_listen(void)
{
	int server_fd, client_fd;
	struct sockaddr_in s_addr;
	struct sockaddr_in client_addr;
	struct connect_info info;

	pthread_t tid;
	int sin_size;
	unsigned short portnum=0x8888;

	printf("Hello,welcome to my server !\r\n");
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == server_fd)
	{
		printf("socket fail ! \r\n");
		return -1;
	}
	printf("socket ok !\r\n");


	bzero(&s_addr, sizeof(struct sockaddr_in));
	s_addr.sin_family=AF_INET;
	s_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	s_addr.sin_port=htons(portnum);

	if(-1 == bind(server_fd,(struct sockaddr *)(&s_addr), sizeof(struct sockaddr)))
	{
		printf("bind fail !\r\n");
		return -1;
	}
	printf("bind ok !\r\n");

	if(-1 == listen(server_fd, 5))
	{
		printf("listen fail !\r\n");
		return -1;
	}
	printf("listen ok\r\n");

	sin_size = sizeof(struct sockaddr_in);

	while(!g_exit){
		client_fd = accept(server_fd, (struct sockaddr *)(&client_addr), &sin_size);
		if(-1 == client_fd)
		{
			printf("accept fail !\r\n");
			return -1;
		}
		info.fd = client_fd;
		memcpy(&info.addr, &client_addr, sizeof(struct sockaddr));
		pthread_create(&tid, NULL, thr_serve, &info);
		usleep(10 * 1000);
	}

	return 0;
}

int init_tsc_test_server(void)
{
	pthread_t tid;
	pthread_create(&tid, NULL, thr_listen, NULL);
}

void deinit_tsc_test_server(void)
{
	g_exit = 1;
}
