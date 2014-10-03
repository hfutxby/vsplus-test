#include <stdio.h>
#include <sys/time.h>
#include <time.h>


int main(void)
{
	struct timeval tv1;
	struct timeval tv2;
	gettimeofday(&tv1, NULL);
	tv1.tv_sec -= 10;

	gettimeofday(&tv2, NULL);
	time_t tt = 1410832727;//tv2.tv_sec;
	struct tm* t = NULL;
	struct tm st = {0};
	localtime_r(&tt, &st);
	printf("tv1:%ld, tv2:%ld\n", tv1.tv_sec, tv2.tv_sec);
	//printf("%4d-%02d-%02d %02d:%02d:%02d(%03ld)\n", t->tm_year+1900,
	//		t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, 
	//		t->tm_sec, tv2.tv_usec/1000);
	printf("%4d-%02d-%02d %02d:%02d:%02d(%03ld)\n", st.tm_year+1900,
			st.tm_mon+1, st.tm_mday, st.tm_hour, st.tm_min, 
			st.tm_sec, tv2.tv_usec/1000);

	sleep(1);
	time_t gtt = tv1.tv_sec;
	struct tm* gt = localtime(&gtt);
	//tv1.tv_sec -= 10;
	//tv1.tv_sec = tv2.tv_sec;

	printf("tv1:%ld, tv2:%ld\n", tv1.tv_sec, tv2.tv_sec);
	//printf("%4d-%02d-%02d %02d:%02d:%02d(%03ld)\n", t->tm_year+1900,
	//		t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, 
	//		t->tm_sec, tv2.tv_usec/1000);
	printf("%4d-%02d-%02d %02d:%02d:%02d(%03ld)\n", st.tm_year+1900,
			st.tm_mon+1, st.tm_mday, st.tm_hour, st.tm_min, 
			st.tm_sec, tv2.tv_usec/1000);



	return 0;
}
