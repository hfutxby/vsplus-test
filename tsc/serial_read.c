#include <stdio.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "serial_pack.h"
#include "ring_buf.h"

//命令特征
pack pack_list[] = {
    {0xf0, 0x5c, 1},//det rising
    {0xf1, 0x5c, 3},//det falling
    {0xc1, 0x5c, 1},//watchdog
    {0x96, 0x69, 2},//sg switch
    };

static int g_fd_serial = -1; //串口
static pthread_mutex_t serial_write_mutex = PTHREAD_MUTEX_INITIALIZER;//写串口保护
static pthread_mutex_t serial_read_mutex = PTHREAD_MUTEX_INITIALIZER;//读串口保护
static pthread_mutex_t ring_mutex = PTHREAD_MUTEX_INITIALIZER;//ring_buf读写保护

static pthread_t g_tid_read; //读取串口
static int g_exit_read = 0;
static pthread_t g_tid_pop; //解析串口命令
static int g_exit_pop = 0;

ring_buf g_ring;

//检查元素串c是否和pack匹配,
//c的长度应该和pack是一样，至少不能比他短
int is_match(unsigned char *c, int len,  pack* pack)
{
	if(len < (pack->load_size + 2))
		return -1;//长度不够
    if(*(c) != pack->p1)
        return 0;//pack头不匹配
    if(*(c + pack->load_size + 1) != pack->p2)
        return 0;//pack尾不匹配

    return 1;//匹配
}

//从ring_buf中找到一个pack，
//返回 >0 表示找到pack的大小，buf内为找到的pack数据
//返回 -1 表示没找到
int pop_pack(ring_buf* r, pack pack_list[], int list_len, unsigned char* buf)
{
	int i, ret;
	int load_size, pack_index;
	int max_load_size = 0;
	unsigned char pre_buf[100];
	for(i = 0; i < list_len; i++){
		if(pack_list[i].load_size > max_load_size)
			max_load_size = pack_list[i].load_size;
	}
	//printf("max_load_size:%d\n", max_load_size);

	for(load_size = 1; load_size <= max_load_size; load_size++){//从最短pack开始匹配
		//printf("load_size:%d, max_load_size:%d\n", load_size, max_load_size); continue;
		for(pack_index = 0; pack_index < list_len; pack_index++){//逐个匹配当前长度报文
			if(pack_list[pack_index].load_size != load_size){
				//printf("load_size:%d, pack_index:%d\n", load_size, pack_index);
				continue;
			}
			memset(pre_buf, 0, sizeof(pre_buf));
			ret = ring_pre_reads(r, pre_buf, (load_size + 2));//预读
			ret = is_match(pre_buf, ret, &pack_list[pack_index]);//进行匹配
			if(ret == 1){
				ring_pops(r, pre_buf, load_size + 2);//弹出匹配部分
				for(i = 0; i < load_size + 2; i++){
					buf[i] = pre_buf[i];
				}
				return load_size + 2;
			}
		}
	}

	if(ret == 0){ //完全没找到,弹出一个元素,以便继续往下找
		ring_pops(r, pre_buf, 1);//max_load_size);
		return -1;
	}

	return -1;
}

void handle_pack(unsigned char* buf)
{
	switch(buf[0]){
		case 0xf0:
			printf("get a det rising slope\n");
			break;
		case 0xf1:
			printf("get a det falling slope\n");
			break;
		case 0x96:
			printf("get a sg switch request\n");
			break;
		case 0x5a:
			printf("get a watchdog feed\n");
			break;
		default:
			printf("unknown pack\n");
	}
}

//对缓存的串口数据进行解包
void* thr_pop(void* para)
{
    int ret, i;
    unsigned char buf[100];
    struct timeval tv1, tv2;
    int list_len = sizeof(pack_list) / sizeof(pack);
    while(!g_exit_pop){
        memset(buf, 0, sizeof(buf));
        pthread_mutex_lock(&ring_mutex);
        gettimeofday(&tv1, NULL);
        ret = pop_pack(&g_ring, pack_list, list_len, buf);
        gettimeofday(&tv2, NULL);
        pthread_mutex_unlock(&ring_mutex);
        if(ret > 0){
            printf("get a pack:");
            for(i = 0; i < ret; i++){
                printf("0x%02x ", buf[i]);
            }
			printf("\n");
			handle_pack(buf);//处理串口命令
            printf("\n\n");
        }
		else
            usleep(1000);
    }
}

int open_port(char* tty_name)
{
	char portname[20];
	int fd;
	//sprintf(portname, "/dev/ttyS%d", port);
	sprintf(portname, "/dev/%s", tty_name);
	fd = open(portname, O_RDWR|O_NOCTTY);//|O_NONBLOCK);//|O_NDELAY);
	if(fd == -1){
		perror("cannot open port");
		return -1;
	}

	printf("open %s sucess\n", portname);

	return fd;
}

int set_opt(int fd, int speed, int bits, char event, int stop)
{
	struct termios newtio, oldtio;
	if(tcgetattr(fd, &oldtio) != 0){
		perror("tcgetattr fail");
		return -1;
	}
	bzero(&newtio, sizeof(newtio));
	newtio.c_cflag |= CLOCAL|CREAD;

	newtio.c_cflag &= ~CSIZE;
	switch(bits){//数据位数
	case 7:
		newtio.c_cflag |= CS7;
		break;
	case 8:
		newtio.c_cflag |= CS8;
		break;
	}

	switch(event){
	case 'O'://奇校验
		newtio.c_cflag |= PARENB;
		newtio.c_cflag |= PARODD;
		newtio.c_iflag |= INPCK;
		break;
	case 'E'://偶校验
		newtio.c_iflag |= INPCK;
		newtio.c_iflag |= PARENB;
		newtio.c_iflag &= ~PARODD;
		break;
	case 'N':
		newtio.c_cflag &= ~PARENB;
		//newtio.c_cflag &= ~INPCK;
		break;
	}

	switch(speed){
	case 2400:
		cfsetispeed(&newtio, B2400);
		cfsetospeed(&newtio, B2400);
		break;
	case 4800:
		cfsetispeed(&newtio, B4800);
		cfsetospeed(&newtio, B4800);
		break;
	case 9600:
		cfsetispeed(&newtio, B9600);
		cfsetospeed(&newtio, B9600);
		break;
	case 19200:
		cfsetispeed(&newtio, B19200);
		cfsetospeed(&newtio, B19200);
		break;
	case 115200:
		cfsetispeed(&newtio, B115200);
		cfsetospeed(&newtio, B115200);
		break;
	default:
		cfsetispeed(&newtio, B9600);
		cfsetospeed(&newtio, B9600);
		break;
	}

	if(stop == 1){
		newtio.c_cflag &= ~CSTOPB;
	}
	else if(stop == 2){
		newtio.c_cflag |= CSTOPB;
	}

	newtio.c_cc[VTIME] = 0;
	newtio.c_cc[VMIN] = 0;

	tcflush(fd, TCIFLUSH);

	if((tcsetattr(fd, TCSANOW, &newtio)) != 0){
		perror("set error");
		return -1;
	}

	printf("set done\n");
	return 0;
}

//读串口原始数据，未解包
void* thr_read(void* para)
{
	fd_set set_read;
	int ret, i;
	unsigned char buf[200] = {0};
	struct timeval timeout;

	while(!g_exit_read){
		FD_ZERO(&set_read);
		FD_SET(g_fd_serial, &set_read);

		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

		ret = select(g_fd_serial + 1, &set_read, NULL, NULL, &timeout);
		switch(ret){
			case -1:
				printf("select error\n");
				break;
			case 0:
				usleep(1000);//printf("*");//timeout
				break;
			default:
				if(FD_ISSET(g_fd_serial, &set_read)){
					pthread_mutex_lock(&serial_read_mutex);
					ret = read(g_fd_serial, buf, sizeof(buf));
					pthread_mutex_unlock(&serial_read_mutex);
					printf("thr_read:%d:", ret);
					for( i = 0; i < ret; i++)
						printf("0x%02x ", 0xff&buf[i]);
					printf("\n");
					if(ret > 0){
						pthread_mutex_lock(&ring_mutex);
						ring_adds_over(&g_ring, buf, ret);
						pthread_mutex_unlock(&ring_mutex);
					}
				}
		}
	}
}

int main(int argc, char* argv[])
{
	int ret = 0;
	if(argc == 2)
		g_fd_serial = open_port(argv[1]);
	else
		g_fd_serial = open_port("ttyUSB0");

	if(g_fd_serial < 0){
		perror("open_port() error");
		return -1;
	}

	if(set_opt(g_fd_serial, 19200, 8, 'N', 1) < 0){
		perror("set_opt() error");
		return -1;
	}

	printf("g_fd_serial=%d\n", g_fd_serial);

	init_ring(&g_ring);

	pthread_create(&g_tid_read, NULL, thr_read, NULL);
	pthread_create(&g_tid_pop, NULL, thr_pop, NULL);
	
	while(1){
		sleep(1);
	}

	g_exit_read = 1;
	if(g_tid_read)
		pthread_join(g_tid_read, NULL);

	g_exit_pop = 1;
	if(g_tid_pop)
		pthread_join(g_tid_pop, NULL);

	close(g_fd_serial);

	return 0;

}

