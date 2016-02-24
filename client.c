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

unsigned short portnum = 12001;

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
	server_addr.sin_port = htons(portnum);

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

#if 0
	ssize_t ret;
	char buf[1024] = "TEST message";
	struct msg_head head = { 0 };
	head.type = TEST;
	head.len = sizeof(buf);
	head.type = (head.type & 0xFFF) | 0xF000;
	head.len = (head.len & 0xFFF) | 0xA000;
	printf("sizeof(struct msg_head):%d\n", sizeof(struct msg_head));
	ret = write(sock_fd, &head, sizeof(struct msg_head));
	ret = write(sock_fd, buf, strlen(buf));
	memset(buf, 0, sizeof(buf));
	do {
		ret = recv(sock_fd, buf, sizeof(buf), 0);
		if(ret <= 0) {
			printf("recv fail. %s\n", strerror(errno));
			break;
		} else {
			printf("%s\n", buf);
		}
	}while(1);
#endif

	char buf[1024] = {0};
	ssize_t ret = 0;
	do {
			ret = recv(sock_fd, buf, sizeof(buf), 0);
			if(ret <= 0) {
				printf("recv fail. %s\n", strerror(errno));
				break;
			} else {
				printf("%s\n", buf);
			}
		}while(1);
//	char buf[] = "12345678";
//	while (1) {
//		write(sock_fd, buf, 8);
//		sleep(1);
//	}

	close(sock_fd);

	return 0;
}

