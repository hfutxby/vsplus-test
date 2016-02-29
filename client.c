/*
 * client connect to tsc_server
 */
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>

#include "tsc_cmd_msg.h"
#include "tsc.h"

int g_exit = 0;
unsigned short tcp_server_port = 12000;

void handle_msg(struct cmd_msg_head_t *head, char* data)
{
	if (head->type == TEST) {
		printf("recv test msg: %d - %s\n", head->type, data);
	}
}

void* thr_recv_cmd_msg(void* arg)
{
	int sock_fd = *(int*) arg;
	struct cmd_msg_head_t msg_head;
	ssize_t ret;
	unsigned char* buf = NULL;
	while (!g_exit) {
		memset(&msg_head, 0, sizeof(msg_head));
		ret = recv(sock_fd, &msg_head, sizeof(msg_head), 0);
		if (ret <= 0 || ret != sizeof(msg_head)) {
			printf("recv msg head error\n");
			g_exit = 1;
			break;
		}
		if (msg_head.tag != 0xF2F1) {
			printf("recv invalid head\n");
			g_exit = 1;
			break;
		}
		buf = (unsigned char*) malloc(msg_head.len);
		memset(buf, 0, msg_head.len);
		ret = recv(sock_fd, buf, msg_head.len, 0);
		if (ret <= 0 || ret != msg_head.len) {
			printf("recv msg body error\n");
			g_exit = 1;
			break;
		}
		handle_msg(&msg_head, buf);
		free(buf);
	}

	pthread_exit(NULL);
}

void client_set_vsplus_stat(int sock_fd)
{
	printf("input stat, 0-stop, 1-start\n");
	char msg[8] = { 0 };
	fgets(msg, sizeof(msg), stdin);
	char stat = atoi(msg);
	struct cmd_msg_head_t msg_head;
	msg_head.tag = 0xF2F1;
	msg_head.type = SET_VSPLUS_STAT;
	msg_head.len = 1;
	unsigned char *buf = malloc(sizeof(msg_head) + msg_head.len);
	memset(buf, 0, sizeof(msg_head) + msg_head.len);
	memcpy(buf, &msg_head, sizeof(msg_head));
	memcpy(buf + sizeof(msg_head), &stat, msg_head.len);
	send(sock_fd, buf, sizeof(msg_head) + msg_head.len, 0);
	free(buf);
}

void client_test(int sock_fd)
{
	printf("input send message string\n");
	char msg[128] = { 0 };
	fgets(msg, sizeof(msg), stdin);
	struct cmd_msg_head_t msg_head;
	msg_head.tag = 0xF2F1;
	msg_head.type = TEST;
	msg_head.len = strlen(msg);
	unsigned char *buf = malloc(sizeof(msg_head) + msg_head.len);
	memset(buf, 0, sizeof(msg_head) + msg_head.len);
	memcpy(buf, &msg_head, sizeof(msg_head));
	memcpy(buf + sizeof(msg_head), msg, msg_head.len);
	send(sock_fd, buf, sizeof(msg_head) + msg_head.len, 0);
	free(buf);
}

int main(int argc, char* argv[])
{
	printf("usage: %s ipaddr. default ip = 127.0.0.1\n", argv[0]);

	int sock_fd;
	struct sockaddr_in server_addr, client_addr;

	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == sock_fd) {
		printf("socket fail. %s\n", strerror(errno));
		return -1;
	}
	printf("create socket success\n");

	memset(&server_addr, 0, sizeof(struct sockaddr_in));
	server_addr.sin_family = AF_INET;
	if (argc != 2)
		server_addr.sin_addr.s_addr = inet_addr("192.168.7.1"); //TEST
	else
		server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_port = htons(tcp_server_port);

	if (-1
			== connect(sock_fd, (struct sockaddr *) (&server_addr),
					sizeof(struct sockaddr))) {
		printf("connect fail. %s\n", strerror(errno));
		return -1;
	}
	printf("connect to %s success\n", inet_ntoa(server_addr.sin_addr));

	pthread_t tid;
	pthread_create(&tid, NULL, thr_recv_cmd_msg, &sock_fd);

#if 1
	ssize_t ret;
	struct cmd_msg_head_t msg_head = { 0 };
	char inputs[128];
	while (!g_exit) {
		printf("\nselect a function:\n"
				"7\t SET_VSPLUS_STAT\n"
				"99\t TEST\n");
		fgets(inputs, sizeof(inputs), stdin);
		int type = atoi(inputs);
		printf("selected -> %d\n", type);
		switch (type) {
		case SET_VSPLUS_STAT:
			client_set_vsplus_stat(sock_fd);
			break;
		case TEST:
			client_test(sock_fd);
			break;
		default:
			printf("wrong select\n");
			break;
		}
		sleep(1);
	}
#endif

	close(sock_fd);

	return 0;
}

