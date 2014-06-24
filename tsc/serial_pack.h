#ifndef __SERIAL_PACK_H
#define __SERIAL_PACK_H

typedef struct{
    unsigned char p1;//帧头
    unsigned char p2;//帧尾
    int load_size;//携带数据量
}pack;

int init_serial(char* dev);
void deinit_serial(void);
int serial_command(unsigned char* buf, int size);

#endif /* __SERIAL_PACK_H*/
