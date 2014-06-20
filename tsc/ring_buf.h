#ifndef __RING_BUF_H
#define __RING_BUF_H

//#define FREE_LEN //动态分配的ringbuf
#ifndef FREE_LEN
//固定分配形式
#define RINGBUF_SIZE 100
typedef struct{
	int head;
	int tail;
	int size;//分配的存储空间长度，实际可用与存储数据的长度要减1
	unsigned char buf[RINGBUF_SIZE];
}ring_buf;

int init_ring(ring_buf* r);

#else
typedef struct{
	int head;
	int tail;
	int size;
	unsigned char *buf;
}ring_buf;//分配最大容量为size的环存储

ring_buf* alloc_ring(int size)
void free_ring(ring_buf* r)
#endif

void __dump(ring_buf* r);
int is_empty(ring_buf* r);
int is_full(ring_buf* r);
//ring中已有元素
int ring_size(ring_buf* r);
//ring中剩余空间
int ring_space(ring_buf* r);
//加一个元素
int ring_add(ring_buf* r, unsigned char c);
//减一个元素
int ring_pop(ring_buf* r, unsigned char *c);
//加一个元素，向后覆盖
int ring_add_over(ring_buf* r, unsigned char c);
//加一串元素
int ring_adds(ring_buf* r, unsigned char *c, int len);
//加一串元素，向后覆盖
int ring_adds_over(ring_buf* r, unsigned char *c, int len);
//减一串元素
int ring_pops(ring_buf* r, unsigned char *c, int len);
//减一串元素，可以不完整，返回值表示实际减的个数
int ring_pops_over(ring_buf* r, unsigned char *c, int len);
//预读一串元素，返回值为实际预读元素个数
int ring_pre_reads(ring_buf *r, unsigned char *c, int len);

#endif /*__RING_BUF_H*/
