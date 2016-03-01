CC = gcc

all:
	$(CC) parse-xml.c `xml2-config --libs --cflags` -o parse-xml.out
	$(CC) parse-dat.c `xml2-config --libs --cflags` -o parse-dat.out
	$(CC) test.c `xml2-config --libs --cflags` -o test.out
	
static:
	$(CC) parse-xml.c `xml2-config --cflags --libs` -static -o parse-xml.exe
	$(CC) parse-dat.c `xml2-config --cflags --libs` -static -o parse-dat.exe
	
clean:
	$(RM) *.out *.exe