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

#include <nopoll.h>

#include "tsc.h"
#include "vcb.h"
#include "tsc_cmd_msg.h"
#include "if626bas.h"

// ============== global =================
static int g_exit = 0;
unsigned short tcp_server_port = 12000;
unsigned short ws_server_port = 12001;

// ============== list  =================
typedef struct node {
	int fd;
	void* data;
//	struct sockaddr_in addr;
	struct node* next;
} node_t;

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

node_t* del_node(node_t **list, int fd)
{
	if (*list == NULL) {
		return NULL;
	}

	node_t *ptr = *list;
	if (ptr->fd == fd) {
		*list = ptr->next;
		return ptr;
	}
	while (ptr && ptr->next) {
		if (ptr->next->fd == fd) {
			node_t* tmp = ptr->next;
			ptr->next = ptr->next->next;
			return tmp;
		}
		ptr = ptr->next;
	}
	return NULL;
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

// ============== operations on connlist  ================
node_t* tcp_conn_list = NULL;
node_t* ws_conn_list = NULL;

void broadcast_on_ws(char* src, int len)
{
	node_t *ptr = ws_conn_list; //global
	int count = 0;
	while (ptr) {
		count++;
		noPollConn *conn = (noPollConn*) ptr->data;
		int bytes_written = nopoll_conn_send_text(conn, src, len);
		bytes_written = nopoll_conn_flush_writes(conn, 2 * 1000 * 1000,
				bytes_written);
		if (ptr->next) {
			ptr = ptr->next;
		} else {
			break;
		}
	}
}

void broadcast_on_tcp(char* src, int len)
{
	node_t *ptr = tcp_conn_list; //global
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
//	printf("%d - %s\n", __LINE__, __func__);
	ssize_t ret;
	//tcp
	struct cmd_msg_head_t head;
	head.tag = 0xF2F1;
	head.type = SG_SWITCH;
	struct sg_switch_t data;
	head.len = sizeof(data);
	data.sg = sg;
	data.stat = stat;
	int buf_len = sizeof(head) + sizeof(data);
	unsigned char* buf = (unsigned char*) malloc(buf_len);
	memset(buf, 0, buf_len);
	memcpy(buf, &head, sizeof(head));
	memcpy(buf + sizeof(head), &data, sizeof(data));
	broadcast_on_tcp(buf, buf_len);
	free(buf);

	//use websocket
	buf_len = 128;
	buf = (unsigned char*) malloc(buf_len);
	memset(buf, 0, buf_len);
	sprintf(buf, "{\"sg\":\"%d\",\"stat\":\"%d\"}", sg, stat);
	broadcast_on_ws(buf, strlen(buf));
	free(buf);
}

// ============== serve functions  =================
int serve_test(struct cmd_msg_head_t *head, char* data, int sock_fd)
{
//	printf("vs_ocit_path:%s\n", vs_ocit_path());
#if 1
	printf("serve cmd TEST\n");
	char str[] = "test echo";
	head->len = sizeof(str);
	int buf_len = sizeof(struct cmd_msg_head_t) + head->len;
	unsigned char* buf = (unsigned char*) malloc(buf_len);
	memset(buf, 0, buf_len);
	memcpy(buf, head, sizeof(struct cmd_msg_head_t));
	memcpy(buf + sizeof(struct cmd_msg_head_t), str, head->len);
	send(sock_fd, buf, buf_len, MSG_DONTWAIT);
	free(buf);
#endif

	return 0;
}

int handle_msg(struct cmd_msg_head_t *head, char* data, int sock_fd)
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
	return 0;
}

// ============== websocket server  =================
void listener_on_message(noPollCtx *ctx, noPollConn *conn, noPollMsg *msg,
		noPollPtr user_data)
{
	printf("recv %d bytes: %s\n", nopoll_msg_get_payload_size(msg),
			nopoll_msg_get_payload(msg));
	//TODO. handle message
//	char buf[] = "Message received";
//	int bytes_written = nopoll_conn_send_text(conn, "Message received",
//			sizeof(buf));
//	bytes_written = nopoll_conn_flush_writes(conn, 2 * 1000 * 1000,
//			bytes_written);
}

void conn_on_close(noPollCtx * ctx, noPollConn * conn, noPollPtr user_data)
{
	node_t* tmp = del_node(&ws_conn_list, nopoll_conn_get_id(conn));
	if (tmp) {
		free(tmp); //tmp->data is pointer to conn
	}
}

nopoll_bool listener_on_ready(noPollCtx * ctx, noPollConn * conn,
		noPollPtr user_data)
{
//	printf("%d -\n", __LINE__);
//	printf("new websocket connect:%s\n", nopoll_conn_get_host_header(conn));
	node_t* info = (node_t*) malloc(sizeof(node_t));
	memset(info, 0, sizeof(node_t));
	info->fd = nopoll_conn_get_id(conn);
	info->data = conn;
	add_node(&ws_conn_list, info);
	nopoll_conn_set_on_close(conn, conn_on_close, NULL);
	return nopoll_true;
}

void* thr_nopoll(void* arg)
{
	printf("waiting for websocket connect ...\n");
	noPollCtx *ctx = nopoll_ctx_new();
	if (!ctx) {
		printf("nopoll_ctx_new error\n");
		pthread_exit(NULL);
	}

	char port[16] = { 0 };
	sprintf(port, "%d", ws_server_port);
	noPollConn *listener = nopoll_listener_new(ctx, "0.0.0.0", port);
	if (!nopoll_conn_is_ok(listener)) {
		printf("nopoll_conn_is_ok fail\n");
		pthread_exit(NULL);
	}
	nopoll_ctx_set_on_msg(ctx, listener_on_message, NULL);
	nopoll_ctx_set_on_ready(ctx, listener_on_ready, NULL);

	nopoll_loop_wait(ctx, 0); //FIXME. how to exit
	nopoll_ctx_unref(ctx);

	pthread_exit(NULL);
}

void* thr_serve(void* arg)
{
	node_t *info = (node_t*) arg;
	struct sockaddr_in *addr = (struct sockaddr_in*) info->data;
	printf("connect from %s:%d\n", inet_ntoa(addr->sin_addr),
			ntohs(addr->sin_port));

	struct cmd_msg_head_t head = { 0 };
	int buf_len = 1024;
	unsigned char* buf = (unsigned char*) malloc(buf_len); //初始存储分配
	int exit = 0;
	while (!g_exit && !exit) { //TODO. better for packet buffering parse
		memset(&head, 0, sizeof(head));
		ssize_t ret = recv(info->fd, &head, sizeof(head), 0);
		printf("%d - recv msg head.  ret: %d  \n", __LINE__, ret);
		if (ret != sizeof(head)) {
			printf("ret:%d ", ret);
			exit = 1;
			break;
		}
		if (head.tag != 0xF2F1) {
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

	printf("disconnet from %s:%d\n", inet_ntoa(addr->sin_addr),
			ntohs(addr->sin_port));
	node_t* tmp = del_node(&tcp_conn_list, info->fd);
	if (tmp) {
		free(tmp->data);
		free(tmp);
	}
	free(buf);
	pthread_exit(NULL);
}

void* thr_listen(void* arg)
{
	int server_fd;
	memcpy(&server_fd, arg, sizeof(server_fd));
	int client_fd;
	struct sockaddr_in client_addr;

	pthread_t tid;
	int sin_size = sizeof(struct sockaddr_in);

	printf("waiting for tcp connect ...\r\n");

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
		info->data = malloc(sizeof(struct sockaddr_in));
		memcpy(info->data, &client_addr, sizeof(client_addr));
		add_node(&tcp_conn_list, info);
		pthread_create(&tid, NULL, thr_serve, info);
		usleep(10 * 1000);
	}

	pthread_exit(NULL);
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
//	printf("create socket success\n");

	int reuse = 1;
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));

	memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sin_family = AF_INET;
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	s_addr.sin_port = htons(tcp_server_port);

	if (-1
			== bind(server_fd, (struct sockaddr *) (&s_addr),
					sizeof(struct sockaddr))) {
		printf("bind fail . %s\n", strerror(errno));
		return -1;
	}
//	printf("bind success\n");

	if (-1 == listen(server_fd, 5)) {
		printf("listen fail. %s\n", strerror(errno));
		return -1;
	}

	pthread_t tid_tcp, tid_ws;
	if (0 != pthread_create(&tid_tcp, NULL, thr_listen, &server_fd)) {
		g_exit = 1;
		printf("call pthread_create fail. %s\n", strerror(errno));
		return -1;
	}
	if (0 != pthread_create(&tid_ws, NULL, thr_nopoll, NULL)) {
		g_exit = 1;
		printf("call pthread_create fail. %s\n", strerror(errno));
		return -1;
	}

#if 0
	char buf[128];
	time_t t;
	struct tm *tmp;
	while (1) {
		dump(ws_conn_list);
		dump(tcp_conn_list);
		memset(buf, 0, sizeof(buf));
		t = time(NULL);
		tmp = localtime(&t);
		strftime(buf, sizeof(buf), "%F %T", tmp);
		printf("%s\n", buf);
		broadcast_on_ws(buf, strlen(buf));
		broadcast_on_tcp(buf, strlen(buf));
		sleep(3);
	}
#endif

	return 0;
}

void close_tsc_server(void)
{
	g_exit = 1;
}
