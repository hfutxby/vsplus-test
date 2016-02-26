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

unsigned short tcp_server_port = 12000;

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
		server_addr.sin_addr.s_addr = inet_addr("192.168.7.98"); //TEST
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

#if 0
	struct msg_head head = {0};
	head.type = SET_PRG;
	head.len = 1;
	int prg_id = 0;
	printf("set prg: ");
	scanf("%d", &prg_id);
	write(sock_fd, &head, sizeof(struct msg_head));
	write(sock_fd, &prg_id, sizeof(prg_id));
#endif

#if 1
	ssize_t ret;
//	struct test_t data = { 0 };
	char data[] = "hello test";
	struct cmd_msg_head_t msg_head = { 0 };
	msg_head.tag = 0xF2F1;
	msg_head.type = TEST;
	msg_head.len = sizeof(data);
//	msg.data = &data;
	int send_size = sizeof(msg_head) + msg_head.len;
	unsigned char *send_buf = (unsigned char*) malloc(send_size);
	memset(send_buf, 0, send_size);
	memcpy(send_buf, &msg_head, sizeof(msg_head));
	memcpy(send_buf + sizeof(msg_head), &data, msg_head.len);

	ret = send(sock_fd, send_buf, send_size, 0);

	int buf_len = 1024;
	unsigned char *buf = (unsigned char*) malloc(buf_len);
//	memset(buf, 0, buf_len);
	do {
		memset(&msg_head, 0, sizeof(msg_head));
		ret = recv(sock_fd, &msg_head, sizeof(msg_head), 0);
		if (ret <= 0 || ret != sizeof(msg_head)) {
			printf("recv msg head error\n");
			break;
		}
		memset(buf, 0, buf_len);
		ret = recv(sock_fd, buf, msg_head.len, 0);
		if (ret <= 0 || ret != msg_head.len) {
			printf("recv msg body error\n");
			break;
		} else {
			printf("recv %d bytes => ", ret);
			int i;
			for (i = 0; i < sizeof(msg_head); i++) {
				printf("%#x ", *((unsigned char*) &msg_head + i));
			}
			for (i = 0; i < ret; i++) {
				printf("%#x ", *(buf + i));
			}
			printf("\n");
		}
	} while (1);
#endif

//	char buf[1024] = {0};
//	ssize_t ret = 0;
//	do {
//			ret = recv(sock_fd, buf, sizeof(buf), 0);
//			if(ret <= 0) {
//				printf("recv fail. %s\n", strerror(errno));
//				break;
//			} else {
//				printf("%s\n", buf);
//			}
//		}while(1);
//	char buf[] = "12345678";
//	while (1) {
//		write(sock_fd, buf, 8);
//		sleep(1);
//	}

	close(sock_fd);

	return 0;
}

