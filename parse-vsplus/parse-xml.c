/*
 * test.c
 *
 *  Created on: 2016年3月1日
 *      Author: byxu
 */

#include <stdio.h>
#include <string.h>

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#include "vsp_param.h"

VSPSysData_t VSPSysData;
VSPDetData_t VSPDetData;
VSPSigData_t VSPSigData;

char g_sg_list[MAX_SIG_GROUP][32] = { 0 };

int sg_name2id(char* name)
{
	int i;
	for (i = 0; i < MAX_SIG_GROUP; i++) {
		if (!xmlStrcmp(name, &g_sg_list[i][0])) {
			return i + 1;
		}
	}
	return -1;
}

int parse_ocit(xmlNodePtr par)
{
	xmlNodePtr cur;
	xmlNodePtr child = xmlFirstElementChild(par);
	while (child) {
		char *text = xmlNodeGetContent(child);
//		printf("%s = %s\n", child->name, text);
		if (!xmlStrcmp(child->name, "ZNr")) {
			VSPSysData.area_num = atoi(text);
		}
		if (!xmlStrcmp(child->name, "FNr")) {
			VSPSysData.con_num = atoi(text);
		}
		if (!xmlStrcmp(child->name, "Relknoten")) {
			VSPSysData.node_num = atoi(text);
		}
		free(text); //must free xmlNodeGetContent returns
		child = xmlNextElementSibling(child);
	}

	return 0;
}

int parse_det(xmlNodePtr par)
{
	xmlNodePtr child = xmlFirstElementChild(par);
	while (child) {
		xmlNodePtr cur = xmlFirstElementChild(child);
		int id = 0;
		while (cur) {
			xmlChar* text = xmlNodeGetContent(cur);
//			printf("%s=%s\n", cur->name, text);
			if (!xmlStrcmp(cur->name, "OCITOutstationNr")) {
				id = atoi(text);
				VSPDetData.VSPDetDataList[id - 1].det_valid = 1;
				VSPDetData.MaxDetDataNum =
						VSPDetData.MaxDetDataNum <= id ?
								id : VSPDetData.MaxDetDataNum;
			}
			free(text);
			cur = xmlNextElementSibling(cur);
		}
		child = xmlNextElementSibling(child);
	}

	return 0;
}

int parse_prg(xmlNodeSetPtr list)
{
	int i;
	for(i = 0; i < list->nodeNr; i++){
		xmlNodePtr child = list->nodeTab[i];
		xmlNodePtr cur = xmlFirstElementChild(child);
		int id = 0, tu = 0;
		while(cur){
			xmlChar* text = xmlNodeGetContent(cur);
//			printf("%s=%s\n", cur->name, text);
			if(!xmlStrcmp(cur->name, "OCITOutstationNr")){
				id = atoi(text);
			}
			if(!xmlStrcmp(cur->name, "SPKopfzeile")){
				xmlNodePtr tmp = xmlFirstElementChild(cur);
				xmlChar* str = xmlNodeGetContent(tmp);
				tu = atoi(str);
				free(str);
			}
			free(text);
			cur = xmlNextElementSibling(cur);
		}
		VSPSysData.vsp_cycletime[id - 1] = tu * 10;
	}

	return 0;
}

int parse_sw_off_stage(xmlNodePtr par)
{
	xmlNodePtr cur;
	xmlNodePtr child = xmlFirstElementChild(par);
	while (child) {
		char *text = xmlNodeGetContent(child);
//		printf("%s = %s\n", child->name, text);
		if (!xmlStrcmp(child->name, "Dauer")) {
			VSPSysData.sw_off_time = atoi(text);
		}
		if (!xmlStrcmp(child->name, "EAZeile")) { //one element per sg
			cur = xmlFirstElementChild(child);
			int id = 0, stat = 0;
			while (cur) {
				char *text = xmlNodeGetContent(cur); //inner redefined
//				printf("%s = %s\n", cur->name, text);
				if (!xmlStrcmp(cur->name, "Signalgruppe")) {
					id = sg_name2id(text);
//					printf("%s(%d):name= %s, %d\n", __func__, __LINE__, text, id);
				}
				if (!xmlStrcmp(cur->name, "StartSignalbild")) {
					stat = atoi(text) == 30 ? 1 : 0;
				}
				free(text);
				cur = xmlNextElementSibling(cur);
			}
//			printf("sg:%d ->  %d\n", id, stat);
			VSPSysData.sw_off_sg[id - 1] = stat;
		}
		free(text); //must free xmlNodeGetContent returns
		child = xmlNextElementSibling(child);
	}

	return 0;
}

int parse_sg_info(xmlNodePtr par)
{
//	xmlNodePtr cur;
	xmlNodePtr child = xmlFirstElementChild(par);
	while (child) {
//		printf("%s = %s\n", child->name, xmlNodeGetContent(child));
		if (!xmlStrcmp(child->name, "Signalgruppe")) { //one element per sg
			xmlNodePtr cur = xmlFirstElementChild(child); //into inner element, point to sg's attributes
			char name[32];
			int id, mingreen_time, minred_time, prep_time, amber_time,
					green_blink;
			while (cur) {
				xmlChar *text = xmlNodeGetContent(cur);
//				printf("%s = %s\n", cur->name, text);
				if (!xmlStrcmp(cur->name, "BezeichnungKurz")) { //sg name
					strncpy(name, text, sizeof(name));
				}
				if (!xmlStrcmp(cur->name, "OCITOutstationNr")) { //sg id
					id = atoi(text);
				}
				if (!xmlStrcmp(cur->name, "MindestFreigabe")) { //min green time
					mingreen_time = atoi(text);
				}
				if (!xmlStrcmp(cur->name, "MindestGesperrt")) { //min red time
					minred_time = atoi(text);
				}
#if 10 //close->free
				if (!xmlStrcmp(cur->name, "AnwurfUebergang")) { //prep time, ts of close->free, usually yellow light
					xmlNodePtr child = xmlFirstElementChild(cur); //into Uebergangselement
					while (child) {
						xmlNodePtr cur = xmlFirstElementChild(child);
						while (cur) {
							xmlChar *text = xmlNodeGetContent(cur);
							if (!xmlStrcmp(cur->name, "Zeitdauer")) {
								prep_time = atoi(xmlNodeGetContent(cur));
							}
							cur = xmlNextElementSibling(cur);
							free(text);
						}

						child = xmlNextElementSibling(child);
					}
				}
#endif
#if 10 //free->close
				if (!xmlStrcmp(cur->name, "AbwurfUebergang")) { //amber time, ts of free->close, usually first green_blink, then yellow light
					xmlNodePtr child = xmlFirstElementChild(cur); //into Uebergangselement
					while (child) {
						int type = 0; //1 - yellow, 2 - green blink
						int time = 0;
						xmlNodePtr cur = xmlFirstElementChild(child);
						while (cur) {
							xmlChar *text = xmlNodeGetContent(cur);
							if (!xmlStrcmp(cur->name, "Signalbild")) {
								if (!xmlStrcmp(text, "20")) {
									type = 2; //green blink
								} else {
									type = 1; //yellow
								}
							}
							if (!xmlStrcmp(cur->name, "Zeitdauer")) {
								time = atoi(text);
							}
							free(text);
							cur = xmlNextElementSibling(cur);
						}
//						printf("type:%d time:%d\n", type, time);
						if (type == 1) {
							amber_time = time;
						} else if (type == 2) {
							green_blink = time;
						}

						child = xmlNextElementSibling(child);
					}
				}
#endif
				free(text);

				cur = xmlNextElementSibling(cur);
			}
//			printf(
//					"id:%d\tmingreen_time:%d\tminred_time:%d\tprep_time:%d\tamber_time:%d\tgreen_blink:%d\n",
//					id, mingreen_time, minred_time, prep_time, amber_time,
//					green_blink);
			VSPSigData.VSPSigDataList[id - 1].sig_valid = 1;
			VSPSigData.VSPSigDataList[id - 1].sig_state = 2;
			VSPSigData.VSPSigDataList[id - 1].amber_time = amber_time * 10;
			VSPSigData.VSPSigDataList[id - 1].prep_time = prep_time * 10;
			VSPSigData.VSPSigDataList[id - 1].mingreen_time = mingreen_time
					* 10;
			VSPSigData.VSPSigDataList[id - 1].minred_time = minred_time * 10;
			VSPSigData.MaxSigDataNum =
					VSPSigData.MaxSigDataNum <= id ?
							id : VSPSigData.MaxSigDataNum;
			//use to convert sg name with id
			strncpy(&g_sg_list[id - 1][0], name, sizeof(g_sg_list[id - 1]));
		}

		child = xmlNextElementSibling(child);
	}

	return 0;
}

int parse_xml(char* filename)
{
	xmlDocPtr doc;
	xmlXPathContextPtr ctx;
	xmlXPathObjectPtr result;

	/* Load XML document */
	doc = xmlParseFile(filename);
	if (doc == NULL) {
		printf("Error: unable to parse file \"%s\"\n", filename);
		return -1;
	}

	/* Create xpath evaluation context */
	ctx = xmlXPathNewContext(doc);
	if (ctx == NULL) {
		printf("Error: unable to create new XPath context\n");
		xmlFreeDoc(doc);
		return -1;
	}

	/* sg info, should be first call to initialize convert sg name to id */
	xmlChar *path = "//SignalgruppeListe";
	result = xmlXPathEvalExpression(path, ctx);
	if (xmlXPathNodeSetIsEmpty(result->nodesetval)) {
		printf("Error: not found signal info. eval xpath \"%s\"\n", path);
		xmlXPathFreeObject(result);
		xmlXPathFreeContext(ctx);
		xmlFreeDoc(doc);
		return -1;
	} else {
		parse_sg_info(result->nodesetval->nodeTab[0]);
	}
	xmlXPathFreeObject(result);

	/* switch off stage */
	path = "//SignalprogrammListe/Ausschaltprogramm";
	result = xmlXPathEvalExpression(path, ctx);
	if (xmlXPathNodeSetIsEmpty(result->nodesetval)) {
		printf("Warning: not found switch off stage. eval xpath \"%s\"\n",
				path);
	} else {
		parse_sw_off_stage(result->nodesetval->nodeTab[0]); //only accept one switch off stage
	}
	xmlXPathFreeObject(result);

	/* ocit num */
	path = "//Kopfdaten/Identifikation/OCITOutstationKennung";
	result = xmlXPathEvalExpression(path, ctx);
	if (xmlXPathNodeSetIsEmpty(result->nodesetval)) {
		printf("Error: not found ocit num. eval xpath \"%s\"\n", path);
		xmlXPathFreeObject(result);
		xmlXPathFreeContext(ctx);
		xmlFreeDoc(doc);
		return -1;
	} else {
		parse_ocit(result->nodesetval->nodeTab[0]);
	}
	xmlXPathFreeObject(result);

	/* det */
	path = "//EingangListe";
	result = xmlXPathEvalExpression(path, ctx);
	if (xmlXPathNodeSetIsEmpty(result->nodesetval)) {
		printf("Error: not found det info.  eval xpath \"%s\"\n", path);
		xmlXPathFreeObject(result);
		xmlXPathFreeContext(ctx);
		xmlFreeDoc(doc);
		return -1;
	} else {
		parse_det(result->nodesetval->nodeTab[0]);
	}
	xmlXPathFreeObject(result);

	/* prg */
	path = "//SignalprogrammListe/Signalprogramm";
	result = xmlXPathEvalExpression(path, ctx);
	if (xmlXPathNodeSetIsEmpty(result->nodesetval)) {
		printf("Error: not found prg info.  eval xpath \"%s\"\n", path);
		xmlXPathFreeObject(result);
		xmlXPathFreeContext(ctx);
		xmlFreeDoc(doc);
		return -1;
	} else {
		parse_prg(result->nodesetval);
	}
	xmlXPathFreeObject(result);

	xmlXPathFreeContext(ctx);
	xmlFreeDoc(doc);

	return 0;
}

void dump_vspconfig(void)
{
	int i, j;
	printf("=== VSPSigData ===\n");
	printf("%6s %6s %6s %6s %6s %6s %6s\n", "id", "state", "amber", "red",
			"prep", "green", "blink");
	for (i = 0; i < VSPSigData.MaxSigDataNum; i++) {
		if (VSPSigData.VSPSigDataList[i].sig_valid) {
			printf("%6d %6d %6d %6d %6d %6d %6d \n", i + 1,
					VSPSigData.VSPSigDataList[i].sig_state,
					VSPSigData.VSPSigDataList[i].amber_time,
					VSPSigData.VSPSigDataList[i].minred_time,
					VSPSigData.VSPSigDataList[i].prep_time,
					VSPSigData.VSPSigDataList[i].mingreen_time,
					VSPSigData.VSPSigDataList[i].green_blink);
		} else
			printf("%6d %6s %6s %6s %6s %6s %6s\n", i + 1, "invalid", "-", "-",
					"-", "-", "-");
	}

	printf("=== VSPDetData ===\n");
	printf("%6s %6s\n", "id", "exist");
	for (i = 0; i < VSPDetData.MaxDetDataNum; i++) {
		if (VSPDetData.VSPDetDataList[i].det_valid)
			printf("%6d %6d\n", i + 1, 1);
		else
			printf("%6d %6s\n", i + 1, "invalid");
	}

	printf("=== VSPSysData ===\n");
	printf("ZNr:%d, FNr:%d, Relknoten:%d\n", VSPSysData.area_num,
			VSPSysData.con_num, VSPSysData.node_num);
	printf("active_prog:%d\n", VSPSysData.active_prog);
	for (i = 0; i < MAX_VSP_PROG; i++) {
		if (VSPSysData.vsp_cycletime[i])
			printf("prg_id:%d tu:%6d\n", i + 1, VSPSysData.vsp_cycletime[i]);
	}

	printf("%4s %4s %4s\n", "sgr", "sge", "val");
	for (i = 0; i < MAX_SIG_GROUP; i++) {
		for (j = 0; j < MAX_SIG_GROUP; j++) {
			if (VSPSysData.inter_green[i][j])
				printf("%4d %4d %4d\n", i + 1, j + 1,
						VSPSysData.inter_green[i][j]);
		}
	}
}

int main(int argc, char* argv[])
{
	if (argc != 2) {
		printf("input filename\n");
		return -1;
	}

	memset(&VSPSysData, 0, sizeof(VSPSysData_t));
	memset(&VSPDetData, 0, sizeof(VSPDetData_t));
	memset(&VSPSigData, 0, sizeof(VSPSigData_t));

	VSPSysData.active_prog = 1;

	parse_xml(argv[1]);

	dump_vspconfig();

	return 0;
}

