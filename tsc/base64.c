#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
//转换前 aaaaaabb ccccdddd eeffffff
//转换后 00aaaaaa 00bbcccc 00ddddee 00ffffff 

unsigned char EncodeIndex[] = {
	'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P',
	'Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f',
	'g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v',
	'w','x','y','z','0','1','2','3','4','5','6','7','8','9','+','/'};
unsigned char DecodeIndex[] = {
	0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x3E,0x40,0x40,0x40,0x3F,
	0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,
	0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,
	0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x40,0x40,0x40,0x40,0x40,0x40,
	0x40,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,
	0x0F,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x40,0x40,0x40,0x40,0x40,
	0x40,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,
	0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,0x30,0x31,0x32,0x33,0x40,0x40,0x40,0x40,0x40};
void Base64_Encode(unsigned char* src,unsigned char* dest, int srclen)
{
	int sign = 0;
	int i;
	for (i = 0; i!= srclen; i++,src++,dest++)
	{
		switch(sign)
		{
			case 0:
				*(dest) = EncodeIndex[*src >> 2];
				break;
			case 1:
				*dest = EncodeIndex[((*(src-1)  & 0x03) << 4) | (((*src) & 0xF0) >> 4)];
				break;
			case 2:
				*dest = EncodeIndex[((*(src-1) &0x0F) << 2) | ((*(src) & 0xC0) >> 6)];
				*(++dest) = EncodeIndex[(*(src) &0x3F)];
				break;
		}
		(sign == 2)?(sign = 0):(sign++);
	}
	switch(sign)
	{
		case 0:
			break;
		case 1:
			*(dest++) = EncodeIndex[((*(src-1)  & 0x03) << 4) | (((*src) & 0xF0) >> 4)];
			*(dest++) = '=';
			*(dest++) = '=';
			break;
		case 2:
			*(dest++) = EncodeIndex[((*(src-1) &0x0F) << 2) | ((*(src) & 0xC0) >> 6)];
			*(dest++) = '=';
			break;
		default:
			break;
	}
}

void Base64_Decode(unsigned char* src, unsigned char* dest, int srclen)
{//len%4 == 0总为true;
	int i;
	for (i = 0; i != srclen/4; i++)//对于不足4个的不作计算
	{
		*dest = (DecodeIndex[*src] << 2) | ((DecodeIndex[*(src+1)] & 0x30) >> 4);
		*(dest+1) = (DecodeIndex[*(src+1)] << 4) | ((DecodeIndex[*(src+2)] &0x3C) >> 2);
		*(dest+2) = ((DecodeIndex[*(src+2)] & 0x03) << 6) | (DecodeIndex[*(src+3)] & 0x3F);
		printf("%c %c %c %c ", *src, *(src+1), *(src+2), *(src+3));
		printf("%x %x %x\n", *dest, *(dest+1), *(dest+2));
		src += 4;
		dest += 3;
		if(i > 5) break;
	}
}

void decode(unsigned char* src, unsigned char* dest, int srclen)
{
	int i, j, index;
	unsigned buf[4];
	for(i = 0; i != srclen/4; i++){
		for(j = 0; j < sizeof(EncodeIndex); j++){
			if(EncodeIndex[j] == *src)
				buf[0] = j;
		}
		for(j = 0; j < sizeof(EncodeIndex); j++){
			if(EncodeIndex[j] == *(src+1))
				buf[1] = j;
		}
		for(j = 0; j < sizeof(EncodeIndex); j++){
			if(EncodeIndex[j] == *(src+2))
				buf[2] = j;
		}
		for(j = 0; j < sizeof(EncodeIndex); j++){
			if(EncodeIndex[j] == *(src+3))
				buf[3] = j;
		}
		*dest = (buf[0] << 2) | ((buf[1] & 0x30) >> 4);
		*(dest+1) = (buf[1] << 4) | ((buf[2] & 0x3C) >> 2);
		*(dest+2) = (buf[2] << 6) | (buf[3] & 0x3F);
		//printf("%c %c %c %c ", *src, *(src+1), *(src+2), *(src+3));
		//printf("%x %x %x\n", *dest, *(dest+1), *(dest+2));
		src += 4;
		dest += 3;
	}
}

int GetEncodeNewLen(const int len)
{
	return (len +(len%3 == 0? 0:(3-len%3)))/3*4 + 1;
}

int GetDecodeNewLen(const int len)
{
	//return len/4*3+1;
	return len*3/4;
}

#define SRC_FILE "para_64.vcb"
int main(void)
{
	int src_len, dest_len, ret;
	struct stat st_buf;
	if((ret = stat(SRC_FILE, &st_buf)) < 0){
		printf("%s\n", strerror(errno));
		return -1;
	};
	printf("%s len %d\n", SRC_FILE, (int)st_buf.st_size);
	src_len = st_buf.st_size;
	dest_len = GetDecodeNewLen(src_len);
	printf("dest len %d\n", dest_len);
	unsigned char *src_buf = (unsigned char*)malloc(src_len);
	memset(src_buf, 0, src_len);
	unsigned char *dest_buf = (unsigned char*)malloc(dest_len);
	memset(dest_buf, 0, dest_len);

	FILE *fp = fopen(SRC_FILE, "rb");
	if(fp == NULL){
		printf("%s\n", strerror(errno));
		return -1;
	}
	ret = fread(src_buf, 1, src_len, fp);
	fclose(fp);
	if(ret != src_len){
		printf("read error. src_len=%d, ret=%d\n", src_len, ret);
		return -1;
	}

	fp = fopen("para.vcb", "wb+");
	if(fp == NULL){
		printf("%s\n", strerror(errno));
		return -1;
	}
	//Base64_Decode(src_buf, dest_buf, src_len);
	decode(src_buf, dest_buf, src_len);
	ret = fwrite(dest_buf, 1, dest_len ,fp);
	if(ret != dest_len){
		printf("write error. dest_len=%d, ret=%d\n", dest_len, ret);
		return -1;
	}

	free(src_buf);
	free(dest_buf);

	return 0;
}	

/*
 ** main
 **
 ** parse and validate arguments and call b64 engine or help
 */

#if 0
using namespace std;
int main( int argc, char* argv[])
{
	string argStr;
	for (int i =1; i != argc; i++)
	{
		argStr += argv[i];
		argStr += ' ';
	}
	argStr.erase(argStr.size()-1);
	unsigned char* pArg = new unsigned char[GetEncodeNewLen(argStr.c_str()) +1];
	memset(pArg,0,GetEncodeNewLen(argStr.c_str()) +1);
	Base64_Encode((unsigned char *)argStr.c_str(),pArg,argStr.size());
	unsigned char* pDecode = new unsigned char[GetDecodeNewLen((const char *)pArg)+1];
	memset(pDecode,0,GetDecodeNewLen((const char *)pArg)+1);
	Base64_Decode(pArg,pDecode,strlen((const char *)pArg));
	cout << argStr.c_str() << endl << pArg<<endl<<pDecode <<endl;
	delete[] pArg;
	delete[] pDecode;
	//unsigned char * pEncodeStr, *pDecodeStr;
	//int srcStrLen;
	//int encodeStrLen ,decodeStrLen;
	//for (int i = 1; i != argc; i++)
	//{
	//	srcStrLen = strlen(argv[i]); // src
	//	encodeStrLen = (srcStrLen +(srcStrLen%3 == 0? 0:(3-srcStrLen%3)))/3*4 + 1; 
	//	pEncodeStr = new unsigned char[encodeStrLen];
	//	memset(pEncodeStr,0,encodeStrLen); //
	//	Base64_Encode((unsigned char *)argv[i], pEncodeStr, srcStrLen);
	//	decodeStrLen =  strlen((char*)pEncodeStr);
	//	pDecodeStr = new unsigned char[decodeStrLen+1];
	//	memset(pDecodeStr,0,encodeStrLen);
	//	Base64_Decode((unsigned char*)pEncodeStr,pDecodeStr,encodeStrLen);
	//	cout<< "string:  " <<argv[i] << "\n" << "Encode:  " << pEncodeStr << "\nDecode:  "<< pDecodeStr << endl;
	//	
	//	delete[] pEncodeStr;
	//	delete[] pDecodeStr;
	//}
	system("pause");
	return( 0 );
}

#endif	
