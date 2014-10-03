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

#include "tsc.h"
#include "vcb.h"
#include "test_msg.h"
#include "if626bas.h"

static int g_exit = 0;

struct connect_info{
	int fd;//connect fd
	struct sockaddr_in addr;//client addr
};

int serve_set_det(char* data)
{
	struct set_det_data* set = (struct set_det_data*)data;
	//printf("====\n");
	//printf("msg:SET_DET\n");
   	//printf("id:%d stat:%d\n", set->id, set->stat);	
	//printf("====\n");
	tsc_det_op(set->id, set->stat);

	return 0;
}

extern PTMSG g_tsc_pt;//在tsc.c中定义
extern int g_tsc_pt_new;
int serve_set_pt(char* data)
{
	memcpy((char*)&g_tsc_pt, data, sizeof(struct set_pt_data));
	g_tsc_pt_new = 1;
	return 0;
}

int serve_det_exist(int sock_fd)
{
	char buf[DETMAX] = {0};
	struct msg_head head = {0};
	head.type = DET_EXIST_R;
	head.len = DETMAX;
	int i;
	for(i = 0; i < DETMAX; i++){
		if(vcb_det_exist[i+1]){
			buf[i] = i+1;
		}
	}
	write(sock_fd, &head, sizeof(struct msg_head));
	write(sock_fd, buf, DETMAX);
	return 0;
}

int serve_test(int sock_fd)
{
	printf("vs_ocit_path:%s\n", vs_ocit_path());

	return 0;
}

int serve_set_prg(int sock_fd, char* data)
{
	char prg_id = *(char*)data;
	prg_track_cur_set(prg_id);

	return 0;
}

int handle_msg(enum msg_type type, char* data, int sock_fd)
{
	printf("recv msg type:");
	switch(type){
		case SET_DET:
			printf("SET_DET\n");
			serve_set_det(data);
			break;
		case SET_PT:
			printf("SET_PT\n");
			serve_set_pt(data);
			break;
		case DET_EXIST:
			printf("DET_EXIST\n");
			serve_det_exist(sock_fd);
			break;
		case TEST:
			printf("TEST\n");
			serve_test(sock_fd);
			break;
		case SET_PRG:
			printf("SET_PRG\n");
			serve_set_prg(sock_fd, data);
			break;
		default: 
			break;
	}
}

static const int g_maxcount = 10;
void* thr_serve(void* arg)
{
	struct connect_info info;
	int ret = 0;
	struct msg_head head = {0};
	int buf_len = 1024;
	char* buf = malloc(buf_len);//初始存储分配
	memcpy(&info, arg, sizeof(struct connect_info));
	printf("connet from %s:%d\n", inet_ntoa(info.addr.sin_addr), ntohs(info.addr.sin_port));

	int count = g_maxcount;
	int exit = 0;
	while(!g_exit && !exit){
		memset(&head, 0, sizeof(head));
		ret = read(info.fd, &head, sizeof(head));
		if(ret == 0){//no data
			usleep(100 * 1000);
			count--;
			if(!count)
				exit = 1;
			else
				continue;
		}
		count = g_maxcount;
		if(head.len > buf_len){
			buf = realloc(buf, buf_len+1024);
			buf_len += 1024;
		}
		memset(buf, 0, buf_len);
		read(info.fd, buf, head.len);
		handle_msg(head.type, buf, info.fd);
	}
	printf("disconnet from %s:%d\n", inet_ntoa(info.addr.sin_addr), ntohs(info.addr.sin_port));
	free(buf);
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
		//printf("bind fail !\r\n");
		debug(1, "bind fail !\r\n");
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
