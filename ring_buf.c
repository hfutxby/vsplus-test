#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
//#include "serial_pack.h"
#include "ring_buf.h"


//#define FREE_LEN
#ifndef FREE_LEN

int init_ring(ring_buf* r)
{
	memset(r, 0, sizeof(ring_buf));
	r->size = RINGBUF_SIZE;

	return 0;
}
#else

//分配最大容量为size的环存储
ring_buf* alloc_ring(int size)
{
	size++;
	ring_buf *r = (ring_buf*)malloc(sizeof(ring_buf));
	if(!r){
		perror("alloc ring failed");
		return NULL;
	}
	memset(r, 0, sizeof(ring_buf));
	r->buf = (unsigned char*)malloc(size);
	if(!r->buf){
		perror("alloc ring failed");
		return NULL;
	}
	r->size = size;

	return r;
}

//释放
void free_ring(ring_buf* r)
{
	if(r->buf)
		free(r->buf);
	if(r)
		free(r);
}
#endif

void __dump(ring_buf* r)
{
	int i;
	printf("head:%d, tail:%d\n", r->head, r->tail);
	for(i = 0; i < r->size; i++)
		printf("0x%02x ", r->buf[i]);
	printf("\n");
}

//head=tail表示环是空的
int is_empty(ring_buf* r)
{
	return (r->head == r->tail) ? 1 : 0;
}

//tail+1=head表示环已满
int is_full(ring_buf* r)
{
	return ((r->tail + 1) % r->size == r->head) ? 1 : 0;
}

//环当前已有元素个数
int ring_size(ring_buf* r)
{
	return (r->tail + r->size - r->head) % r->size;
}

//环还可以容纳元素个数
int ring_space(ring_buf* r)
{
	return r->size - 1 - ring_size(r);
}

//加一个元素
int ring_add(ring_buf* r, unsigned char c)
{
	if(ring_space(r) >= 1){
		r->buf[r->tail] = c;
		r->tail = (r->tail + 1) % r->size;
		return 0;
	}
	else
		return -1;
}

//减一个元素
int ring_pop(ring_buf* r, unsigned char *c)
{
	if(ring_size(r) >= 1){
		*c = r->buf[r->head];
		r->buf[r->head] = 0;
		r->head = (r->head + 1) % r->size;
		return 0;
	}
	else
		return -1;
}

//加一个元素，向后覆盖
int ring_add_over(ring_buf* r, unsigned char c)
{
	unsigned char tmp;

	if(ring_space(r) >= 1)
		ring_add(r, c);
	else{
		ring_pop(r, &tmp);
		ring_add(r, c);
	}

	return 0;
}

//加一串元素
int ring_adds(ring_buf* r, unsigned char *c, int len)
{
	int i;
	if(ring_space(r) >= len){
		for(i = 0; i < len; i++){
			r->buf[r->tail] = *(c+i);
			r->tail = (r->tail + 1) % r->size;
		}
		return 0;
	}
	else
		return -1;
}

//加一串元素，向后覆盖
int ring_adds_over(ring_buf* r, unsigned char *c, int len)
{
	int i;
	for(i = 0; i < len; i++){
		ring_add_over(r, *(c + i));
	}

	return 0;
}


//减一串元素
int ring_pops(ring_buf* r, unsigned char *c, int len)
{
	int i;
	if(ring_size(r) >= len){
		for(i = 0; i < len; i++){
			*(c + i) = r->buf[r->head];
			r->buf[r->head] = 0;
			r->head = (r->head + 1) % r->size;
		}
		return len;
	}
	else
		return -1;
}

//减一串元素，可以不完整，返回值表示实际减的个数
int ring_pops_over(ring_buf* r, unsigned char *c, int len)
{
	int i, ret;
	unsigned char tmp;

	for(i = 0; i < len; i++){
		if(!is_empty(r)){
			*(c + i) = r->buf[r->head];
			r->buf[r->head] = 0;
			r->head = (r->head + 1) % r->size;
		}
		else
			break;
	}

	return i;
}

//预读一串元素，返回值为实际预读元素个数
int ring_pre_reads(ring_buf *r, unsigned char *c, int len)
{
	int i, head, size;
	head = r->head;
	size = ring_size(r); 
	if(len > size)//预读请求过长
		len = size;

	for(i = 0; i < len; i++){
		*(c + i) = r->buf[head];
		head = (head + 1) % r->size;
	}

	return len;
}

//所有元素求和
int ring_sum(ring_buf *r)
{
	if(r->head == r->tail)
		return 0;

	int sum = 0;
	int index = r->head;
	do{
		sum += r->buf[index];
		index = (index + r->size + 1) % r->size;
	}while(index != r->tail);

	return sum;
}

int ring_clr(ring_buf *r)
{
	r->head = 0;
	r->tail = 0;	
}
