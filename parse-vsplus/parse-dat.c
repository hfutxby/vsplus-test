/*
 * parse-dat.c
 *
 * 将vsp_param.dat解析成xml格式，便于后台软件操作。
 */

#include <stdio.h>
#include <unistd.h>
#include <getopt.h>

#include <libxml/parser.h>

#include "vsp_param.h"

const char g_version[] = "0.2";

int parse() {
	xmlNodePtr cur;
	xmlDocPtr doc;

	doc = xmlParseFile("1.xml");
	if (doc == NULL) {
		fprintf(stderr, "Document not parsed successfully. \n");
		return -1;
	}

	cur = xmlDocGetRootElement(doc);
	if (cur == NULL) {
		fprintf(stderr, "empty document\n");
		xmlFreeDoc(doc);
		return -1;
	}
	printf("root name: %s\n", cur->name);

	return 0;
}

//把dat文件解析成xml文件
int create(char* dat_file, char* xml_file) {
	VSPSysData_t VSPSysData;
	VSPDetData_t VSPDetData;
	VSPSigData_t VSPSigData;

	FILE *fp = fopen(dat_file, "rb");
	fread(&VSPSysData, 1, sizeof(VSPSysData), fp);
	fread(&VSPDetData, 1, sizeof(VSPDetData), fp);
	fread(&VSPSigData, 1, sizeof(VSPSigData), fp);
	fclose(fp);

	xmlDocPtr doc = xmlNewDoc("1.0");
	xmlNodePtr root = xmlNewNode(NULL, "vs_param");
	xmlDocSetRootElement(doc, root);
	char buf[128];

	xmlNodePtr cur = xmlNewNode(NULL, "area_num");
	sprintf(buf, "%d", VSPSysData.area_num);
	xmlNodeSetContent(cur, buf);
	xmlAddChild(root, cur);

	cur = xmlNewNode(NULL, "con_num");
	sprintf(buf, "%d", VSPSysData.con_num);
	xmlNodeSetContent(cur, buf);
	xmlAddChild(root, cur);

	cur = xmlNewNode(NULL, "node_num");
	sprintf(buf, "%d", VSPSysData.node_num);
	xmlNodeSetContent(cur, buf);
	xmlAddChild(root, cur);

//	cur = xmlNewNode(NULL, "active_prog");
//	sprintf(buf, "%d", VSPSysData.active_prog);
//	xmlNodeSetContent(cur, buf);
//	xmlAddChild(root, cur);

	xmlNodePtr prog_list = xmlNewNode(NULL, "vsp_cycletime");
	xmlAddChild(root, prog_list);

	int i, j;
	xmlNodePtr row;
	for (i = 0; i < MAX_VSP_PROG; i++) {
		if (VSPSysData.vsp_cycletime[i]) {
			row = xmlNewNode(NULL, "prog");
			xmlAddChild(prog_list, row);

			cur = xmlNewNode(NULL, "id");
			sprintf(buf, "%d", i+1);
			xmlNodeSetContent(cur, buf);
			xmlAddChild(row, cur);

			cur = xmlNewNode(NULL, "tu");
			sprintf(buf, "%d", VSPSysData.vsp_cycletime[i]/10);
			xmlNodeSetContent(cur, buf);
			xmlAddChild(row, cur);
		}
	}

	xmlNodePtr ig_matrix = xmlNewNode(NULL, "inter_green");
	xmlAddChild(root, ig_matrix);
	for (i = 0; i < MAX_SIG_GROUP; i++) {
		for (j = 0; j < MAX_SIG_GROUP; j++) {
			if (VSPSysData.inter_green[i][j]) {
				row = xmlNewNode(NULL, "ig");
				xmlAddChild(ig_matrix, row);

				cur = xmlNewNode(NULL, "sgr");
				sprintf(buf, "%d", i+1);
				xmlNodeSetContent(cur, buf);
				xmlAddChild(row, cur);

				cur = xmlNewNode(NULL, "sge");
				sprintf(buf, "%d", j+1);
				xmlNodeSetContent(cur, buf);
				xmlAddChild(row, cur);

				cur = xmlNewNode(NULL, "T");
				sprintf(buf, "%d", VSPSysData.inter_green[i][j]/10);
				xmlNodeSetContent(cur, buf);
				xmlAddChild(row, cur);
			}
		}
	}

//	cur = xmlNewNode(NULL, "MaxDetDataNum");
//	sprintf(buf, "%d", VSPDetData.MaxDetDataNum);
//	xmlNodeSetContent(cur, buf);
//	xmlAddChild(root, cur);

	xmlNodePtr det_list = xmlNewNode(NULL, "det_list");
	xmlAddChild(root, det_list);
	for (i = 0; i < MAX_DET_GROUP; i++) {
		if (VSPDetData.VSPDetDataList[i].det_valid) {
			cur = xmlNewNode(NULL, "id");
			sprintf(buf, "%d", i+1);
			xmlNodeSetContent(cur, buf);
			xmlAddChild(det_list, cur);
		}
	}

//	cur = xmlNewNode(NULL, "MaxSigDataNum");
//	sprintf(buf, "%d", VSPSigData.MaxSigDataNum);
//	xmlNodeSetContent(cur, buf);
//	xmlAddChild(root, cur);

	xmlNodePtr sg_list = xmlNewNode(NULL, "sig_list");
	xmlAddChild(root, sg_list);
	for (i = 0; i < MAX_SIG_GROUP; i++) {
		if (VSPSigData.VSPSigDataList[i].sig_valid) {
			row = xmlNewNode(NULL, "sg");
			xmlAddChild(sg_list, row);

			cur = xmlNewNode(NULL, "id");
			sprintf(buf, "%d", i+1);
			xmlNodeSetContent(cur, buf);
			xmlAddChild(row, cur);

			cur = xmlNewNode(NULL, "amber_time");
			sprintf(buf, "%d", VSPSigData.VSPSigDataList[i].amber_time/10);
			xmlNodeSetContent(cur, buf);
			xmlAddChild(row, cur);

			cur = xmlNewNode(NULL, "prep_time");
			sprintf(buf, "%d", VSPSigData.VSPSigDataList[i].prep_time/10);
			xmlNodeSetContent(cur, buf);
			xmlAddChild(row, cur);

			cur = xmlNewNode(NULL, "minred_time");
			sprintf(buf, "%d", VSPSigData.VSPSigDataList[i].minred_time/10);
			xmlNodeSetContent(cur, buf);
			xmlAddChild(row, cur);

			cur = xmlNewNode(NULL, "mingreen_time");
			sprintf(buf, "%d", VSPSigData.VSPSigDataList[i].mingreen_time/10);
			xmlNodeSetContent(cur, buf);
			xmlAddChild(row, cur);

			cur = xmlNewNode(NULL, "green_blink");
			sprintf(buf, "%d", VSPSigData.VSPSigDataList[i].green_blink/10);
			xmlNodeSetContent(cur, buf);
			xmlAddChild(row, cur);
		}
	}

	if(-1 != xmlSaveFormatFile(xml_file, doc, 1)){
		printf("write xml file success\n");
	}

	return 0;
}

int main(int argc, char* argv[]) {
	extern char* optarg;
	extern int optind, opterr, optopt;

	opterr = 0;
	char optstring[] = "vhd::";
	struct option long_options[] = {
			{ "version", no_argument, 0, 'v' },
			{ "help", no_argument, 0, 'h' },
			{ "debug", optional_argument, 0, 'd' },
			{ 0,0, 0, 0 }
	};
	int opt;
	int option_index;
	while ((opt = getopt_long(argc, argv, optstring, long_options,
			&option_index)) != -1) {
		switch (opt) {
		case 'h':
			printf("usage:%s datfile xmlfile\n", argv[0]);
			printf("\t-h,--help: help\n");
			printf("\t-v,--version: version\n");
			printf("\t-d[n],--debug=[n]: print debug message\n");
			return 0;
			break;
		case 'v':
			printf("version %s, build at: %s %s\n", g_version, __DATE__,
					__TIME__);
			return 0;
			break;
		case 'd':
			printf("debug_level:%d\n", optarg ? atoi(optarg) : 0);
			break;
		default:
			printf("usage:%s datfile xmlfile, -h for more info\n", argv[0]);
			return 0;
			break;
		}
	}

	char dat_file[128]={}, xml_file[128]={};
	if(!argv[optind]){
		printf("usage:%s datfile xmlfile, -h for more info\n", argv[0]);
		return -1;
	}
	else{
		sprintf(dat_file,"%s", argv[optind]);
		optind++;
	}
	if(!argv[optind]){
		printf("default outfile: vsp_param.xml\n");
		sprintf(xml_file, "%s", "vsp_param.xml");
	}
	else{
		sprintf(xml_file, "%s", argv[optind]);
		optind++;
	}

	if(-1 == access(dat_file, R_OK)){
		printf("%s can't read\n", dat_file);
		return -1;
	}

	create(dat_file, xml_file);

	return 0;
}
