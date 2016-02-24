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
#include <errno.h>

#include "tsc.h"
#include "vcb.h"
#include "tsc_cmd_msg.h"
#include "if626bas.h"

// ============== global =================
static int g_exit = 0;
unsigned short portnum = 12001;

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

void broadcast(char* src, int len)
{
	node_t *ptr = connect_info_list; //global
	int count = 0;
	while (ptr) {
		count++;
		send(ptr->fd, src, len, MSG_DONTWAIT);
		if (ptr->next) {
			ptr = ptr->next;
		} else {
			break;
		}
	}
}

void broadcast_sg_switch(int sg, int stat)
{
	ssize_t ret;
	struct msg_head head;
	struct sg_switch_t body;
	head.type = SG_SWITCH;
	head.len = sizeof(struct sg_switch_t);
	body.sg = sg;
	body.stat = stat;

	node_t *ptr = connect_info_list; //global
	int count = 0;
	while (ptr) {
		count++;
		send(ptr->fd, &head, sizeof(head), MSG_DONTWAIT);
		send(ptr->fd, &body, sizeof(body), MSG_DONTWAIT);
		if (ptr->next) {
			ptr = ptr->next;
		} else {
			break;
		}
	}
}
// ============== vsplus setting  =================
//int serve_set_det(char* data)
//{
//	struct set_det_data* set = (struct set_det_data*) data;
//	//printf("====\n");
//	//printf("msg:SET_DET\n");
//	printf("id:%d stat:%d\n", set->id, set->stat);
//	//printf("====\n");
//	tsc_det_op(set->id, set->stat);
//
//	return 0;
//}
//
//extern PTMSG g_tsc_pt;	//在tsc.c中定义
//extern int g_tsc_pt_new;
//int serve_set_pt(char* data)
//{
//	memcpy((char*) &g_tsc_pt, data, sizeof(struct set_pt_data));
//	g_tsc_pt_new = 1;
//	return 0;
//}

//int serve_det_exist(int sock_fd)
//{
//	char buf[DETMAX] = { 0 };
//	struct msg_head head = { 0 };
//	head.type = DET_EXIST_R;
//	head.len = DETMAX;
//	int i;
//	for (i = 0; i < DETMAX; i++) {
//		if (vcb_det_exist[i + 1]) {
//			buf[i] = i + 1;
//		}
//	}
//	write(sock_fd, &head, sizeof(struct msg_head));
//	write(sock_fd, buf, DETMAX);
//	return 0;
//}

int serve_test(struct msg_head *head, char* data, int sock_fd)
{
//	printf("vs_ocit_path:%s\n", vs_ocit_path());
#if 1
	printf("serve cmd TEST\n");
	char buf[] = "serve cmd TEST. echo >>>";
	send(sock_fd, buf, sizeof(buf), MSG_DONTWAIT);
	send(sock_fd, data, head->len, MSG_DONTWAIT);
#endif

	return 0;
}

//int serve_set_prg(int sock_fd, char* data)
//{
//	char prg_id = *(char*) data;
//	prg_track_cur_set(prg_id);
//
//	return 0;
//}

extern int vsplus_stat; //define in main.c

int handle_msg(struct msg_head *head, char* data, int sock_fd)
{
	switch (head->type) {
//	case SET_DET:
//		printf("SET_DET ");
//		serve_set_det(data);
//		break;
//	case SET_PT:
//		printf("SET_PT\n");
//		serve_set_pt(data);
//		break;
//	case DET_EXIST:
//		printf("DET_EXIST\n");
//		serve_det_exist(sock_fd);
//		break;
	case TEST:
		printf("recv cmd TEST\n");
//		printf("data: %s\n", data);
		serve_test(head, data, sock_fd);
		break;
//	case SET_PRG:
//		printf("SET_PRG\n");
//		serve_set_prg(sock_fd, data);
//		break;
//	case SET_STAT:
//		printf("SET_STAT:set vsplus run stat\n");
//		vsplus_stat = 0; //FIXME
//		break;
	default:
		printf("unknown cmd type:%d\n", head->type);
		break;
	}
}

int check_msg(struct cmd_msg_t *msg)
{
	if ((msg->type >> 12 == 0xF) && (msg->len >> 12 == 0xA)) {
		msg->len &= 0xFFF;
		msg->type &= 0xFFF;
		return 1;
	} else {
		return 0;
	}
}

void* thr_serve(void* arg)
{
	node_t *info = (node_t*) arg;
	struct msg_head head = { 0 };
	int buf_len = 1024;
	char* buf = malloc(buf_len); //初始存储分配
	printf("connect from %s:%d\n", inet_ntoa(info->addr.sin_addr),
			ntohs(info->addr.sin_port));

	int exit = 0;
	while (!g_exit && !exit) {
		memset(&head, 0, sizeof(head));
		ssize_t ret = recv(info->fd, &head, sizeof(head), 0);
		printf("%d - recv msg head.  ret: %d  \n", __LINE__, ret);
		if (ret != sizeof(head)) {
			printf("ret:%d ", ret);
			exit = 1;
			break;
		}
		if (!check_msg(&head)) {
			printf("invalid msg head\n");
		}

//		if (head.len > buf_len) {
//			buf = realloc(buf, buf_len + 1024);
//			buf_len += 1024;
//		}
		memset(buf, 0, buf_len);
		ret = recv(info->fd, buf, head.len, 0); //TODO check buf recv completely
		printf("%d - recv msg body ret: %d.  \n", __LINE__, ret);
		handle_msg(&head, buf, info->fd);
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

	pthread_t tid;
	int sin_size = sizeof(struct sockaddr_in);

	while (!g_exit) {
		client_fd = accept(server_fd, (struct sockaddr *) (&client_addr),
				&sin_size);
		if (-1 == client_fd) {
			printf("accept fail. %s\n", strerror(errno));
			g_exit = 1;
			break;
		}

//		printf("accept - fd: %d,  ip: %s\n", client_fd,
//				inet_ntoa(client_addr.sin_addr));

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

int open_tsc_server(void)
{
	static int server_fd;
	struct sockaddr_in s_addr;

	int sin_size;

	printf("Hello,welcome to vsplus-test server \n");
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == server_fd) {
		printf("create socket fail. %s\n", strerror(errno));
		return -1;
	}
	printf("create socket success\n");

	int reuse = 1;
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));

	memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sin_family = AF_INET;
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	s_addr.sin_port = htons(portnum);

	if (-1
			== bind(server_fd, (struct sockaddr *) (&s_addr),
					sizeof(struct sockaddr))) {
		printf("bind fail . %s\n", strerror(errno));
		return -1;
	}
	printf("bind success\n");

	if (-1 == listen(server_fd, 5)) {
		printf("listen fail. %s\n", strerror(errno));
		return -1;
	}
	printf("listen for connections ...\r\n");

	pthread_t tid;
	if (0 != pthread_create(&tid, NULL, thr_listen, &server_fd)) {
		g_exit = 1;
		printf("call pthread_create fail. %s\n", strerror(errno));
		return -1;
	}

//	char buf[1024];
//	while (1) {
//		sprintf(buf, "hello %d\n", time(NULL));
//		broadcast(buf, sizeof(buf));
//		sleep(1);
//	}
//	while (1) {
//		dump(connect_info_list);
//		sleep(1);
//	}

	return 0;
}

void close_tsc_server(void)
{
	g_exit = 1;
}
