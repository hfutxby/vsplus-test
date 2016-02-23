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
#include <netinet/tcp.h>

#include "tsc.h"
#include "vcb.h"
#include "test_msg.h"
#include "if626bas.h"

// ============== global =================
static int g_exit = 0;
unsigned short portnum = 12000;

//struct connect_info {
//	int fd; //connect socket fd
//	struct sockaddr_in addr; //client addr
//};

// ============== list  =================
typedef struct node {
	int fd;
	struct sockaddr_in addr;
	struct node* next;
} node_t;

node_t* connect_info_list = NULL;

int add_node(node_t** list, node_t* node)
{
	if (*list == NULL) {
		*list = node;
		return 0;
	}
	node_t *ptr = *list;
	while (ptr && ptr->next) {
		ptr = ptr->next;
	}
	ptr->next = node;

	return 0;
}

int del_node(node_t **list, int fd)
{
	if (*list == NULL) {
		return 0;
	}

	node_t *ptr = *list;
	if (ptr->fd == fd) {
		*list = ptr->next;
		free(ptr);
		return 0;
	}
	while (ptr && ptr->next) {
		if (ptr->next->fd == fd) {
			node_t* tmp = ptr->next;
			ptr->next = ptr->next->next;
			free(tmp);
		}
		ptr = ptr->next;
	}
}

dump(node_t* list)
{
	node_t *ptr = list;
	int count = 0;
	while (ptr) {
		count++;
		printf("index:%d, next:%p\n", ptr->fd, ptr->next);
		if (ptr->next) {
			ptr = ptr->next;
		} else {
			break;
		}
	}
	printf("=== total %d nodes ===\n", count);
}

broadcast(node_t* list)
{
	char buf[1024];
	node_t *ptr = list;
	int count = 0;
	while (ptr) {
		count++;
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "hello, %d\n", time(NULL));
		send(ptr->fd, buf, sizeof(buf), MSG_DONTWAIT);
//		printf("index:%d, next:%p\n", ptr->fd, ptr->next);
		if (ptr->next) {
			ptr = ptr->next;
		} else {
			break;
		}
	}
	printf("=== total %d nodes ===\n", count);
}
// ============== vsplus setting  =================
int serve_set_det(char* data)
{
	struct set_det_data* set = (struct set_det_data*) data;
	//printf("====\n");
	//printf("msg:SET_DET\n");
	printf("id:%d stat:%d\n", set->id, set->stat);
	//printf("====\n");
	tsc_det_op(set->id, set->stat);

	return 0;
}

extern PTMSG g_tsc_pt;	//在tsc.c中定义
extern int g_tsc_pt_new;
int serve_set_pt(char* data)
{
	memcpy((char*) &g_tsc_pt, data, sizeof(struct set_pt_data));
	g_tsc_pt_new = 1;
	return 0;
}

int serve_det_exist(int sock_fd)
{
	char buf[DETMAX] = { 0 };
	struct msg_head head = { 0 };
	head.type = DET_EXIST_R;
	head.len = DETMAX;
	int i;
	for (i = 0; i < DETMAX; i++) {
		if (vcb_det_exist[i + 1]) {
			buf[i] = i + 1;
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
	char prg_id = *(char*) data;
	prg_track_cur_set(prg_id);

	return 0;
}

extern int vsplus_stat; //define in main.c
int handle_msg(enum msg_type type, char* data, int sock_fd)
{
	//printf("recv msg type:");
	switch (type) {
	case SET_DET:
		printf("SET_DET ");
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
	case SET_STAT:
		printf("SET_STAT:set vsplus run stat\n");
		vsplus_stat = 0; //FIXME
		break;
	default:
		break;
	}
}

void* thr_serve(void* arg)
{
//	struct connect_info info;
	node_t *info = (node_t*) arg;
	struct msg_head head = { 0 };
	int buf_len = 1024;
	char* buf = malloc(buf_len); //初始存储分配
//	memcpy(&info, arg, sizeof(struct connect_info));
	printf("connect from %s:%d\n", inet_ntoa(info->addr.sin_addr),
			ntohs(info->addr.sin_port));

	int exit = 0;
	while (!g_exit && !exit) {
		memset(&head, 0, sizeof(head));
		ssize_t ret = recv(info->fd, &head, sizeof(head), 0);
		printf("%d - recv msg head.  cmd: %d, len: %d.  \n", __LINE__,
				head.type, head.len);
		if (ret != sizeof(head)) {
			printf("ret:%d ", ret);
			exit = 1;
			break;
		}
		if (head.len > buf_len) {
			buf = realloc(buf, buf_len + 1024);
			buf_len += 1024;
		}
		memset(buf, 0, buf_len);
		recv(info->fd, buf, head.len, 0);
		printf("%d - recv msg body ret: %d.  \n", __LINE__, ret);
//		handle_msg(head.type, buf, info.fd);
	}
	printf("disconnet from %s:%d\n", inet_ntoa(info->addr.sin_addr),
			ntohs(info->addr.sin_port));
	del_node(&connect_info_list, info->fd);
	free(buf);
}

void* thr_listen(void* arg)
{
	int server_fd;
	memcpy(&server_fd, arg, sizeof(server_fd));
	int client_fd;
	struct sockaddr_in client_addr;
//	struct connect_info info; //FIXME

	pthread_t tid;
	int sin_size = sizeof(struct sockaddr_in);

	while (!g_exit) {
		client_fd = accept(server_fd, (struct sockaddr *) (&client_addr),
				&sin_size);
		if (-1 == client_fd) {
			printf("accept fail !\r\n");
			g_exit = 1;
			break;
		}

		printf("accept - fd: %d,  ip: %s\n", client_fd,
				inet_ntoa(client_addr.sin_addr));

		int keep_alive = 1; //open keepalive
		int keep_idle = 5; //after this time, begin detect alive
		int keep_interval = 1; //alive detect frequency
		int keep_count = 5; //alive detect numbers
		setsockopt(client_fd, SOL_SOCKET, SO_KEEPALIVE, (void*) &keep_alive,
				sizeof(keep_alive));
		setsockopt(client_fd, SOL_TCP, TCP_KEEPIDLE, (void*) &keep_idle,
				sizeof(keep_idle));
		setsockopt(client_fd, SOL_TCP, TCP_KEEPINTVL, (void*) &keep_interval,
				sizeof(keep_interval));
		setsockopt(client_fd, SOL_TCP, TCP_KEEPCNT, (void*) &keep_count,
				sizeof(keep_count));

		node_t* info = (node_t*) malloc(sizeof(node_t));
		memset(info, 0, sizeof(node_t));
		info->fd = client_fd;
		memcpy(&info->addr, &client_addr, sizeof(client_addr));
		add_node(&connect_info_list, info);
		pthread_create(&tid, NULL, thr_serve, info);
		usleep(10 * 1000);
	}

	return 0;
}

int init_tsc_test_server(void)
{
	static int server_fd;
	struct sockaddr_in s_addr;

	int sin_size;
	unsigned short portnum = 12000;

	printf("Hello,welcome to vsplus-test server !\r\n");
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == server_fd) {
		printf("create socket fail ! \r\n");
		return -1;
	}
	printf("create socket success !\r\n");

	int reuse = 1;
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));

	memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sin_family = AF_INET;
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	s_addr.sin_port = htons(portnum);

	if (-1
			== bind(server_fd, (struct sockaddr *) (&s_addr),
					sizeof(struct sockaddr))) {
		printf("bind fail !\r\n");
		return -1;
	}
	printf("bind success !\r\n");

	if (-1 == listen(server_fd, 5)) {
		printf("listen fail !\r\n");
		return -1;
	}
	printf("listen for connections ...\r\n");

	pthread_t tid;
	if (0 != pthread_create(&tid, NULL, thr_listen, &server_fd)) {
		g_exit = 1;
		printf("call pthread_create fail\n");
		return -1;
	}

	while (1) {
		dump(connect_info_list);
		broadcast(connect_info_list);
		sleep(1);
	}

	return 0;
}

void deinit_tsc_test_server(void)
{
	g_exit = 1;
}
