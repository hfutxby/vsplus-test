#include <stdio.h>

#include "tsc.h"

int main(int argc, char* argv[])
{
	int val;
	PTMSG pt = {0};
	printf("=== PT ===\n");
	printf("MP:");
	scanf("%d", &val);
	pt.MP = val;

	printf("Linie:");
	scanf("%d", &val);
	pt.Linie = val;

	printf("Kurs:");
	scanf("%d", &val);
	pt.Kurs = val;

	printf("Route:");
	scanf("%d", &val);
	pt.Route = val;

	printf("Prioritaet:");
	scanf("%d", &val);
	pt.Prioritaet = val;

	printf("Laenge:");
	scanf("%d", &val);
	pt.Laenge = val;

	printf("RichtungVonHand:");
	scanf("%d", &val);
	pt.RichtungVonHand = val;

	printf("FahrplanAbweichnung:");
	scanf("%d", &val);
	pt.FahrplanAbweichnung = val;

	printf("set==>\n");
	printf("MP:%d\n", pt.MP);
	printf("Linie:%d\n", pt.Linie);
	printf("Kurs:%d\n", pt.Kurs);
	printf("Route:%d\n", pt.Route);
	printf("Prioritaet:%d\n", pt.Prioritaet);
	printf("Laenge:%d\n", pt.Laenge);
	printf("RichtungVonHand:%d\n", pt.RichtungVonHand);
	printf("FahrplanAbweichnung:%d\n", pt.FahrplanAbweichnung);
	
	FILE* fp = fopen("pt.dat", "wb");
	if(!fp){
		printf("fopen pt.dat fail\n");
		return -1;
	}
	char start = 1;
	fwrite(&start, sizeof(start), 1, fp);
	fwrite(&pt, sizeof(PTMSG), 1, fp);

	fclose(fp);

	return 0;
}
