/*
 * parse-xml.c
 *
 * 将WS3C生成的XML配置文件解析生成dat和vcb文件供信号机使用
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <getopt.h>

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#include "vsp_param.h"

const char* g_version = "0.5";
char g_outfile[128] = { 0 };
int g_debug = 0x0;

/* 在父节点cur下面找到第一个名字符合path的子节点 */
xmlNodePtr _find_node(xmlNodePtr cur, char path[64])
{
	//printf("%s(%d):cur->name %s, path %s\n", __func__, __LINE__, cur->name, path);
	xmlNodePtr child = NULL;
	int child_count = xmlChildElementCount(cur);
	//printf("child_count:%d\n", child_count);

	child = xmlFirstElementChild(cur);
	while (child_count) {
		//printf("child_name:%s\n", child->name);
		if (!xmlStrcmp(child->name, path))
			return child;
		child = xmlNextElementSibling(child);
		child_count--;
	}

	return NULL;
}

/***** 解析检测器参数 *****/
xmlChar DET_PATH[] = "//EingangListe/Eingang";
typedef struct {
	char name[64];
	int id;
} det_node;

struct det_info {
	int num;
	det_node* node;
};
struct det_info g_det_info = { };

int set_det(xmlNodeSetPtr nodes)
{
	xmlNodePtr cur, child;

	int count, child_count, size;
	int i;

	count = (nodes) ? nodes->nodeNr : 0;
	//printf("Result (%d nodes):\n", count);
	g_det_info.num = count;
	size = g_det_info.num * sizeof(det_node);
	g_det_info.node = malloc(size);
	memset(g_det_info.node, 0, size);

	for (i = 0; i < count; ++i) {
		assert(nodes->nodeTab[i]);
		cur = nodes->nodeTab[i];

		child_count = xmlChildElementCount(cur);
		child = xmlFirstElementChild(cur);
		while (child_count) {
			if (!xmlStrcmp(child->name, "BezeichnungKurz")) {
				//printf("%s=%s\n", "BezeichnungKurz", xmlNodeGetContent(child));
				sprintf(g_det_info.node[i].name, "%s",
						xmlNodeGetContent(child));
			} else if (!xmlStrcmp(child->name, "OCITOutstationNr")) {
				//printf("%s=%s\n", "OCITOutstationNr", xmlNodeGetContent(child));
				g_det_info.node[i].id = atoi(xmlNodeGetContent(child));
			}
			child = xmlNextElementSibling(child);
			child_count--;
		}
	}
}

void dump_det(void)
{
	int i;
	printf("=== dump_det ===\n");
	printf("%10s %10s\n", "name", "id");
	for (i = 0; i < g_det_info.num; i++) {
		printf("%10s %10d\n", g_det_info.node[i].name, g_det_info.node[i].id);
	}
	printf("\n");
}

int parse_det(char* filename)
{
	xmlDocPtr doc;
	xmlXPathContextPtr xpathCtx;
	xmlXPathObjectPtr xpathObj;

	/* Load XML document */
	doc = xmlParseFile(filename);
	if (doc == NULL) {
		fprintf(stderr, "Error: unable to parse file \"%s\"\n", filename);
		return (-1);
	}

	/* Create xpath evaluation context */
	xpathCtx = xmlXPathNewContext(doc);
	if (xpathCtx == NULL) {
		fprintf(stderr, "Error: unable to create new XPath context\n");
		xmlFreeDoc(doc);
		return (-1);
	}

	/* Evaluate xpath expression */
	xpathObj = xmlXPathEvalExpression(DET_PATH, xpathCtx);
	if (xpathObj == NULL) {
		fprintf(stderr, "Error: unable to evaluate xpath expression \"%s\"\n",
				DET_PATH);
		xmlXPathFreeContext(xpathCtx);
		xmlFreeDoc(doc);
		return (-1);
	}

	/* Print results */
	set_det(xpathObj->nodesetval);
	if (g_debug)
		dump_det();

	/* Cleanup */
	xmlXPathFreeObject(xpathObj);
	xmlXPathFreeContext(xpathCtx);
	xmlFreeDoc(doc);

	return 0;
}

/***** 解析信号灯参数 *****/
xmlChar SG_PATH[] = "//SignalgruppeListe/Signalgruppe";
typedef struct {
	char name[64];
	int id;
	int free_sum;
	int close_sum;
	int amber;
	int prep;
	int green_blink;
} sg_node;

struct sg_info {
	int num;
	sg_node* node;
};
struct sg_info g_sg_info = { };

int set_sg(xmlNodeSetPtr nodes)
{
	xmlNodePtr cur, child;
	xmlNodePtr child_2, child_3;

	int count, child_count, size;
	int i;

	count = (nodes) ? nodes->nodeNr : 0;
	//printf("Result (%d nodes):\n", count);
	g_sg_info.num = count;
	size = g_sg_info.num * sizeof(sg_node);
	g_sg_info.node = malloc(size);
	memset(g_sg_info.node, 0, size);

	for (i = 0; i < count; ++i) {
		assert(nodes->nodeTab[i]);
		cur = nodes->nodeTab[i];

		child_count = xmlChildElementCount(cur);
		child = xmlFirstElementChild(cur);
		while (child_count) {
			if (!xmlStrcmp(child->name, "BezeichnungKurz")) {
				//printf("%s=%s\n", "BezeichnungKurz", xmlNodeGetContent(child));
				sprintf(g_sg_info.node[i].name, "%s", xmlNodeGetContent(child));
			} else if (!xmlStrcmp(child->name, "OCITOutstationNr")) {
				//printf("%s=%s\n", "OCITOutstationNr", xmlNodeGetContent(child));
				g_sg_info.node[i].id = atoi(xmlNodeGetContent(child));
			} else if (!xmlStrcmp(child->name, "MindestFreigabe")) {
				//printf("%s=%s\n", "MindestFreigabe", xmlNodeGetContent(child));
				g_sg_info.node[i].free_sum = atoi(xmlNodeGetContent(child))
						* 10;
			} else if (!xmlStrcmp(child->name, "MindestGesperrt")) {
				//printf("%s=%s\n", "MindestGesperrt", xmlNodeGetContent(child));
				g_sg_info.node[i].close_sum = atoi(xmlNodeGetContent(child))
						* 10;
			} else if (!xmlStrcmp(child->name, "AnwurfUebergang")) {
				child_2 = _find_node(child, "Uebergangselement");
				while (child_2) {
					child_3 = _find_node(child_2, "Signalbild");
					if (child_3
							&& (!xmlStrcmp(xmlNodeGetContent(child_3),
									"rotgelb")
									|| (!xmlStrcmp(xmlNodeGetContent(child_3),
											"rotblk"))
									|| (!xmlStrcmp(xmlNodeGetContent(child_3),
											"02")))) {
						//printf("%s(%d)\n", __func__, __LINE__);
						child_3 = xmlNextElementSibling(child_3);
						//printf("amber=%d\n", atoi(xmlNodeGetContent(child)));
						g_sg_info.node[i].prep = atoi(
								xmlNodeGetContent(child_3)) * 10;
					}
					child_2 = xmlNextElementSibling(child_2);
				}
			} else if (!xmlStrcmp(child->name, "AbwurfUebergang")) {
				child_2 = _find_node(child, "Uebergangselement");
				while (child_2) {
					child_3 = _find_node(child_2, "Signalbild");
					if (child_3
							&& (!xmlStrcmp(xmlNodeGetContent(child_3), "gelb"))
							|| (!xmlStrcmp(xmlNodeGetContent(child_3), "0C"))) {
						//printf("%s(%d)\n", __func__, __LINE__);
						child_3 = xmlNextElementSibling(child_3);
						g_sg_info.node[i].amber = atoi(
								xmlNodeGetContent(child_3)) * 10;
					} else if (child_3
							&& (!xmlStrncmp(xmlNodeGetContent(child_3),
									"gruenblk", strlen("gruenblk"))
									|| (!xmlStrcmp(xmlNodeGetContent(child_3),
											"20")))) {
						child_3 = xmlNextElementSibling(child_3);
						g_sg_info.node[i].green_blink = atoi(
								xmlNodeGetContent(child_3)) * 10;
					}

					child_2 = xmlNextElementSibling(child_2);
				}
			}

			child = xmlNextElementSibling(child);
			child_count--;
		}
	}
}

void dump_sg(void)
{
	int i;
	printf("=== dump_sg ===\n");
	printf("%10s %10s %10s %10s %10s %10s %10s\n", "name", "id", "free_sum",
			"close_sum", "prep", "amber", "green_blink");
	for (i = 0; i < g_sg_info.num; i++) {
		printf("%10s %10d %10d %10d %10d %10d %10d\n", g_sg_info.node[i].name,
				g_sg_info.node[i].id, g_sg_info.node[i].free_sum,
				g_sg_info.node[i].close_sum, g_sg_info.node[i].prep,
				g_sg_info.node[i].amber, g_sg_info.node[i].green_blink);
	}
	printf("\n");
}

int parse_sg(char* filename)
{
	xmlDocPtr doc;
	xmlXPathContextPtr xpathCtx;
	xmlXPathObjectPtr xpathObj;

	/* Load XML document */
	doc = xmlParseFile(filename);
	if (doc == NULL) {
		fprintf(stderr, "Error: unable to parse file \"%s\"\n", filename);
		return (-1);
	}

	/* Create xpath evaluation context */
	xpathCtx = xmlXPathNewContext(doc);
	if (xpathCtx == NULL) {
		fprintf(stderr, "Error: unable to create new XPath context\n");
		xmlFreeDoc(doc);
		return (-1);
	}

	/* Evaluate xpath expression */
	xpathObj = xmlXPathEvalExpression(SG_PATH, xpathCtx);
	if (xpathObj == NULL) {
		fprintf(stderr, "Error: unable to evaluate xpath expression \"%s\"\n",
				SG_PATH);
		xmlXPathFreeContext(xpathCtx);
		xmlFreeDoc(doc);
		return (-1);
	}

	/* Print results */
	set_sg(xpathObj->nodesetval);
	if (g_debug)
		dump_sg();

	/* Cleanup */
	xmlXPathFreeObject(xpathObj);
	xmlXPathFreeContext(xpathCtx);
	xmlFreeDoc(doc);

	return 0;
}

/***** 解析路口ID参数 *****/
xmlChar ID_PATH[] = "//Kopfdaten/Identifikation/OCITOutstationKennung";
struct id_info {
	int ZNr;
	int FNr;
	int Relknoten;
};
struct id_info g_id_info = { };

int set_id(xmlNodeSetPtr nodes)
{
	xmlNodePtr cur, child;

	int count, child_count, size;
	int i;

	count = (nodes) ? nodes->nodeNr : 0;
	//printf("Result (%d nodes):\n", count);

	for (i = 0; i < count; ++i) {
		assert(nodes->nodeTab[i]);
		cur = nodes->nodeTab[i];

		child_count = xmlChildElementCount(cur);
		child = xmlFirstElementChild(cur);
		while (child_count) {
			if (!xmlStrcmp(child->name, "ZNr")) {
				//printf("%s=%s\n", "ZNr", xmlNodeGetContent(child));
				g_id_info.ZNr = atoi(xmlNodeGetContent(child));
			} else if (!xmlStrcmp(child->name, "FNr")) {
				//printf("%s=%s\n", "FNr", xmlNodeGetContent(child));
				g_id_info.FNr = atoi(xmlNodeGetContent(child));
			} else if (!xmlStrcmp(child->name, "Relknoten")) {
				//printf("%s=%s\n", "Relknoten", xmlNodeGetContent(child));
				g_id_info.Relknoten = atoi(xmlNodeGetContent(child));
			}
			child = xmlNextElementSibling(child);
			child_count--;
		}
	}
}

void dump_id(void)
{
	int i;
	printf("=== dump_id ===\n");
	printf("%10s %10s %10s\n", "ZNr", "FNr", "Relknoten");
	printf("%10d %10d %10d\n\n", g_id_info.ZNr, g_id_info.FNr,
			g_id_info.Relknoten);
}

int parse_id(char* filename)
{
	xmlDocPtr doc;
	xmlXPathContextPtr xpathCtx;
	xmlXPathObjectPtr xpathObj;

	/* Load XML document */
	doc = xmlParseFile(filename);
	if (doc == NULL) {
		fprintf(stderr, "Error: unable to parse file \"%s\"\n", filename);
		return (-1);
	}

	/* Create xpath evaluation context */
	xpathCtx = xmlXPathNewContext(doc);
	if (xpathCtx == NULL) {
		fprintf(stderr, "Error: unable to create new XPath context\n");
		xmlFreeDoc(doc);
		return (-1);
	}

	/* Evaluate xpath expression */
	xpathObj = xmlXPathEvalExpression(ID_PATH, xpathCtx);
	if (xpathObj == NULL) {
		fprintf(stderr, "Error: unable to evaluate xpath expression \"%s\"\n",
				ID_PATH);
		xmlXPathFreeContext(xpathCtx);
		xmlFreeDoc(doc);
		return (-1);
	}

	/* Print results */
	set_id(xpathObj->nodesetval);
	if (g_debug)
		dump_id();

	/* Cleanup */
	xmlXPathFreeObject(xpathObj);
	xmlXPathFreeContext(xpathCtx);
	xmlFreeDoc(doc);

	return 0;
}

/***** parse switch on/off phase *****/
xmlChar SW_OFF_PATH[] = "//SignalprogrammListe/Ausschaltprogramm";
typedef struct sw_prg {
	int time;
	char sg[MAX_SIG_GROUP];
} sw_prg_t;
typedef struct sw_prg_list {
	int num;
	sw_prg_t* list;
} sw_prg_list_t;
sw_prg_list_t g_sw_prg_list;

int set_sw_off(xmlNodeSetPtr nodes)
{
	printf("%d - %s\n", __LINE__, __func__);
	xmlNodePtr cur, child;
	int count, child_count, i;

	int prg_count = (nodes) ? nodes->nodeNr : 0; //may more than one switch programs
	if (prg_count) {
		g_sw_prg_list.num = prg_count;
		g_sw_prg_list.list = (sw_prg_t*) malloc(sizeof(sw_prg_t) * prg_count);
		memset(g_sw_prg_list.list, 0, sizeof(sw_prg_t) * prg_count);
	}

	int prg_id = 0, sg_id = 0;
	for (prg_id = 0; prg_id < prg_count; prg_id++) {
		assert(nodes->nodeTab[prg_id]);
		cur = nodes->nodeTab[prg_id];
		child_count = xmlChildElementCount(cur);
		child = xmlFirstElementChild(cur);
		sw_prg_t *prg = &g_sw_prg_list.list[prg_id];

		for (i = 0; i < child_count; i++) {
			if (!xmlStrcmp(child->name, "Dauer")) {
				printf("read: %s = %s\n", "Dauer", xmlNodeGetContent(child));
				prg->time = atoi(xmlNodeGetContent(child));
			}
			if (!xmlStrcmp(child->name, "EAZeile")) {
				int sg_count = xmlChildElementCount(child);
				xmlNodePtr sg_cur = xmlFirstElementChild(child);
				while (sg_cur) {
					if (!xmlStrcmp(sg_cur->name, "Signalgruppe")) {
						sg_id = name2id(xmlNodeGetContent(sg_cur));
						printf("%s = %s\n", sg_cur->name, xmlNodeGetContent(sg_cur));
					}
					if (!xmlStrcmp(sg_cur->name, "StartSignalbild")) {
						printf("%s = %s\n", sg_cur->name, xmlNodeGetContent(sg_cur));
						if (!xmlStrcmp(xmlNodeGetContent(sg_cur), "30")) {
							prg->sg[sg_id-1] = 1;
						}
					}
					sg_cur = xmlNextElementSibling(sg_cur);
				}
			}
			child = xmlNextElementSibling(child);
		}
	}
}

int parse_sw(char* filename)
{
	xmlDocPtr doc;
	xmlXPathContextPtr xpathCtx;
	xmlXPathObjectPtr xpathObj;

	/* Load XML document */
	doc = xmlParseFile(filename);
	if (doc == NULL) {
		fprintf(stderr, "Error: unable to parse file \"%s\"\n", filename);
		return (-1);
	}

	/* Create xpath evaluation context */
	xpathCtx = xmlXPathNewContext(doc);
	if (xpathCtx == NULL) {
		fprintf(stderr, "Error: unable to create new XPath context\n");
		xmlFreeDoc(doc);
		return (-1);
	}

	/* Evaluate xpath expression */
	xpathObj = xmlXPathEvalExpression(SW_OFF_PATH, xpathCtx);
	if (xpathObj == NULL) {
		fprintf(stderr, "Error: unable to evaluate xpath expression \"%s\"\n",
				SW_OFF_PATH);
		xmlXPathFreeContext(xpathCtx);
		xmlFreeDoc(doc);
		return (-1);
	}

	/* Print results */
	set_sw_off(xpathObj->nodesetval);
//	if (g_debug)
//		dump_prg();

	/* Cleanup */
	xmlXPathFreeObject(xpathObj);
	xmlXPathFreeContext(xpathCtx);
	xmlFreeDoc(doc);

	return 0;
}

/***** 解析配时方案参数 *****/
xmlChar PRG_PATH[] = "//SignalprogrammListe/Signalprogramm";
typedef struct {
	char name[64];
	int id;
	int tu;
} prg_node;

struct prg_info {
	int num;
	prg_node* node;
};
struct prg_info g_prg_info = { };

int set_prg(xmlNodeSetPtr nodes)
{
	xmlNodePtr cur, child;
	xmlNodePtr child_2;

	int count, child_count, size;
	int i;

	count = (nodes) ? nodes->nodeNr : 0;
	//printf("Result (%d nodes):\n", count);
	g_prg_info.num = count;
	size = g_prg_info.num * sizeof(prg_node);
	g_prg_info.node = malloc(size);
	memset(g_prg_info.node, 0, size);

	for (i = 0; i < count; ++i) {
		assert(nodes->nodeTab[i]);
		cur = nodes->nodeTab[i];

		child_count = xmlChildElementCount(cur);
		child = xmlFirstElementChild(cur);
		while (child_count) {
			if (!xmlStrcmp(child->name, "BezeichnungKurz")) {
				//printf("%s=%s\n", "BezeichnungKurz", xmlNodeGetContent(child));
				sprintf(g_prg_info.node[i].name, "%s",
						xmlNodeGetContent(child));
			} else if (!xmlStrcmp(child->name, "OCITOutstationNr")) {
				//printf("%s=%s\n", "OCITOutstationNr", xmlNodeGetContent(child));
				g_prg_info.node[i].id = atoi(xmlNodeGetContent(child));
			} else if (!xmlStrcmp(child->name, "SPKopfzeile")) {
				child_2 = xmlFirstElementChild(child);
				while (child_2) {
					if (!xmlStrcmp(child_2->name, "TU")) {
						//printf("%s=%s\n", "TU", xmlNodeGetContent(child));
						g_prg_info.node[i].tu = atoi(xmlNodeGetContent(child))
								* 10;
					}
					child_2 = xmlNextElementSibling(child_2);
				}
			}
			child = xmlNextElementSibling(child);
			child_count--;
		}
	}
}

void dump_prg(void)
{
	int i;
	printf("=== dump_prg ===\n");
	printf("%20s %10s %10s\n", "name", "id", "tu");
	for (i = 0; i < g_prg_info.num; i++) {
		printf("%20s %10d %10d\n", g_prg_info.node[i].name,
				g_prg_info.node[i].id, g_prg_info.node[i].tu);
	}
	printf("\n");
}

int parse_prg(char* filename)
{
	xmlDocPtr doc;
	xmlXPathContextPtr xpathCtx;
	xmlXPathObjectPtr xpathObj;

	/* Load XML document */
	doc = xmlParseFile(filename);
	if (doc == NULL) {
		fprintf(stderr, "Error: unable to parse file \"%s\"\n", filename);
		return (-1);
	}

	/* Create xpath evaluation context */
	xpathCtx = xmlXPathNewContext(doc);
	if (xpathCtx == NULL) {
		fprintf(stderr, "Error: unable to create new XPath context\n");
		xmlFreeDoc(doc);
		return (-1);
	}

	/* Evaluate xpath expression */
	xpathObj = xmlXPathEvalExpression(PRG_PATH, xpathCtx);
	if (xpathObj == NULL) {
		fprintf(stderr, "Error: unable to evaluate xpath expression \"%s\"\n",
				PRG_PATH);
		xmlXPathFreeContext(xpathCtx);
		xmlFreeDoc(doc);
		return (-1);
	}

	/* Print results */
	set_prg(xpathObj->nodesetval);
	if (g_debug)
		dump_prg();

	/* Cleanup */
	xmlXPathFreeObject(xpathObj);
	xmlXPathFreeContext(xpathCtx);
	xmlFreeDoc(doc);

	return 0;
}

/***** 解析绿间隔参数 *****/
//必须先解析信号灯参数
xmlChar INTER_PATH_sp[] = "//Zwischenzeitenmatrix/ZwiZt";
xmlChar INTER_PATH_base[] = "//SicherheitsrelevanteZwischenzeitenmatrix/ZwiZt";
typedef struct {
	int sgr;
	int sge;
	int val;
} inter_node;

struct inter_info {
	int num;
	inter_node* node;
};
struct inter_info g_inter_info = { };

int name2id(char* name)
{
	int i;
	int id;
	for (i = 0; i < g_sg_info.num; i++) {
		if (!xmlStrcmp(g_sg_info.node[i].name, name)) {
			//printf("%s:%d\n", name, g_sg_info.node[i].id);
			return g_sg_info.node[i].id;
		}
	}
	return 0;
}

int set_inter(xmlNodeSetPtr nodes)
{
	xmlNodePtr cur, child;

	int count, child_count, size;
	int i;

	count = (nodes) ? nodes->nodeNr : 0;
	//printf("Result (%d nodes):\n", count);
	g_inter_info.num = count;
	size = g_inter_info.num * sizeof(inter_node);
	g_inter_info.node = malloc(size);
	memset(g_inter_info.node, 0, size);

	for (i = 0; i < count; ++i) {
		assert(nodes->nodeTab[i]);
		cur = nodes->nodeTab[i];

		child_count = xmlChildElementCount(cur);
		child = xmlFirstElementChild(cur);
		while (child_count) {
			if (!xmlStrcmp(child->name, "Raeumer")) {
				//printf("%s=%s\n", "Raeumer", xmlNodeGetContent(child));
				g_inter_info.node[i].sgr = name2id(xmlNodeGetContent(child));
			} else if (!xmlStrcmp(child->name, "Einfahrer")) {
				//printf("%s=%s\n", "Einfahrer", xmlNodeGetContent(child));
				g_inter_info.node[i].sge = name2id(xmlNodeGetContent(child));
			} else if (!xmlStrcmp(child->name, "T")) {
				//printf("%s=%s\n", "T", xmlNodeGetContent(child));
				g_inter_info.node[i].val = atoi(xmlNodeGetContent(child)) * 10;
			}
			child = xmlNextElementSibling(child);
			child_count--;
		}
	}
	return count;
}

void dump_inter(void)
{
	int i;
	printf("=== dump_inter ===\n");
	printf("%10s %10s %10s\n", "sgr", "sge", "val");
	for (i = 0; i < g_inter_info.num; i++) {
		printf("%10d %10d %10d\n", g_inter_info.node[i].sgr,
				g_inter_info.node[i].sge, g_inter_info.node[i].val);
	}
	printf("\n");
}

int parse_inter(char* filename)
{
	xmlDocPtr doc;
	xmlXPathContextPtr xpathCtx;
	xmlXPathObjectPtr xpathObj;

	/* Load XML document */
	doc = xmlParseFile(filename);
	if (doc == NULL) {
		fprintf(stderr, "Error: unable to parse file \"%s\"\n", filename);
		return (-1);
	}

	/* Create xpath evaluation context */
	xpathCtx = xmlXPathNewContext(doc);
	if (xpathCtx == NULL) {
		fprintf(stderr, "Error: unable to create new XPath context\n");
		xmlFreeDoc(doc);
		return (-1);
	}

	/* Evaluate xpath expression */
	xpathObj = xmlXPathEvalExpression(INTER_PATH_base, xpathCtx);
	if (xpathObj == NULL) {
		fprintf(stderr, "Error: unable to evaluate xpath expression \"%s\"\n",
				INTER_PATH_base);
		xmlXPathFreeContext(xpathCtx);
		xmlFreeDoc(doc);
		return (-1);
	}

	/* Print results */
	int ret;
	ret = set_inter(xpathObj->nodesetval);
	if (ret == 0) {
		printf("%s(%d)\n", __func__, __LINE__);
		xmlXPathFreeObject(xpathObj);
		xpathObj = xmlXPathEvalExpression(INTER_PATH_sp, xpathCtx);
		if (xpathObj == NULL) {
			fprintf(stderr,
					"Error: unable to evaluate xpath expression \"%s\"\n",
					INTER_PATH_sp);
			xmlXPathFreeContext(xpathCtx);
			xmlFreeDoc(doc);
			return (-1);
		}
		ret = set_inter(xpathObj->nodesetval);
	}

	if (g_debug)
		dump_inter();

	/* Cleanup */
	xmlXPathFreeObject(xpathObj);
	xmlXPathFreeContext(xpathCtx);
	xmlFreeDoc(doc);

	return 0;
}

/**** 从XML文件中解码出vcb文件****/
#define VCB_NAME "vsp_param.vcb"
xmlChar VCB_PATH[] = "//VAParameter/BinaerdatenListe/Binaerdaten/B_Data";

unsigned char EncodeIndex[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
		'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
		'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k',
		'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y',
		'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/' };

int decode_vcb(char* src, int src_len)
{
	int dest_len = src_len * 3 / 4;
	char* dest = malloc(dest_len);
	char* ptr = dest;
	memset(dest, 0, dest_len);

	char filename[128] = { 0 };
	sprintf(filename, "%s.vcb", g_outfile);

	//FILE* fp = fopen(VCB_NAME, "wb");
	FILE* fp = fopen(filename, "wb");
	if (fp == NULL) {
		printf("%s\n", strerror(errno));
		return -1;
	}

	int i, j, index;
	unsigned buf[4];
	int last = 3;
	for (i = 0; i < src_len / 4; i++) {
		for (j = 0; j < sizeof(EncodeIndex); j++) {
			if (EncodeIndex[j] == *src)
				buf[0] = j;
		}
		for (j = 0; j < sizeof(EncodeIndex); j++) {
			if (EncodeIndex[j] == *(src + 1))
				buf[1] = j;
		}
		for (j = 0; j < sizeof(EncodeIndex); j++) {
			if (EncodeIndex[j] == *(src + 2))
				buf[2] = j;
		}
		for (j = 0; j < sizeof(EncodeIndex); j++) {
			if (EncodeIndex[j] == *(src + 3))
				buf[3] = j;
		}
		*dest = (buf[0] << 2) | ((buf[1] & 0x30) >> 4);
		*(dest + 1) = (buf[1] << 4) | ((buf[2] & 0x3C) >> 2);
		*(dest + 2) = (buf[2] << 6) | (buf[3] & 0x3F);
		//printf("%-6d: 0x%02x 0x%02x 0x%02x 0x%02x ==>", i*4, *(src)&0xff, *(src+1)&0xff, *(src+2)&0xff, *(src+3)&0xff);
		//printf("0x%02x 0x%02x 0x%02x\n", *(dest)&0xff, *(dest+1)&0xff, *(dest+2)&0xff);
		if (i == (src_len / 4 - 1)) {
			if (*(src + 2) == '=')
				last = 1;
			else if (*(src + 3) == '=')
				last = 2;
		}
		fwrite(dest, last, 1, fp);
		src += 4;
		dest += 3;
	}

	fclose(fp);
	printf("File[%s.vcb] write success\n", g_outfile);

	free(ptr);

	return 0;
}

int set_vcb(xmlNodeSetPtr nodes)
{
	xmlNodePtr cur, child;
	xmlChar* src = NULL;

	int count, child_count, size;
	int i;

	count = (nodes) ? nodes->nodeNr : 0;
	//printf("Result (%d nodes):\n", count);

	for (i = 0; i < count; ++i) {
		assert(nodes->nodeTab[i]);
		cur = nodes->nodeTab[i];
		src = xmlNodeGetContent(cur);
		decode_vcb(src, strlen(src));
		//printf("src:%s\n", src);
		//printf("len:%d\n", strlen(src));
	}
}

int parse_vcb(char* filename)
{
	xmlDocPtr doc;
	xmlXPathContextPtr xpathCtx;
	xmlXPathObjectPtr xpathObj;

	/* Load XML document */
	doc = xmlParseFile(filename);
	if (doc == NULL) {
		fprintf(stderr, "Error: unable to parse file \"%s\"\n", filename);
		return (-1);
	}

	/* Create xpath evaluation context */
	xpathCtx = xmlXPathNewContext(doc);
	if (xpathCtx == NULL) {
		fprintf(stderr, "Error: unable to create new XPath context\n");
		xmlFreeDoc(doc);
		return (-1);
	}

	/* Evaluate xpath expression */
	xpathObj = xmlXPathEvalExpression(VCB_PATH, xpathCtx);
	if (xpathObj == NULL) {
		fprintf(stderr, "Error: unable to evaluate xpath expression \"%s\"\n",
				VCB_PATH);
		xmlXPathFreeContext(xpathCtx);
		xmlFreeDoc(doc);
		return (-1);
	}

	/* Print results */
	set_vcb(xpathObj->nodesetval);

	/* Cleanup */
	xmlXPathFreeObject(xpathObj);
	xmlXPathFreeContext(xpathCtx);
	xmlFreeDoc(doc);

	return 0;
}

/***********************/
int parse_xml(char* filename)
{
	assert(filename);

	/* Init libxml */
	LIBXML_TEST_VERSION

	parse_det(filename);
	parse_sg(filename);
	parse_id(filename);
	parse_prg(filename);
	parse_inter(filename);
	parse_vcb(filename);
	parse_sw(filename);

	/* Shutdown libxml */
	xmlCleanupParser();

	return 0;
}

/******** 生成配置文件  **********/
VSPSysData_t VSPSysData;
VSPDetData_t VSPDetData;
VSPSigData_t VSPSigData;

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

int vspconfig(void)
{
	FILE* pf;
	int i, j, index;
	int val;

	memset(&VSPSysData, 0, sizeof(VSPSysData));
	memset(&VSPDetData, 0, sizeof(VSPDetData));
	memset(&VSPSigData, 0, sizeof(VSPSigData));

	VSPSysData.area_num = g_id_info.ZNr;
	VSPSysData.con_num = g_id_info.FNr;
	VSPSysData.node_num = g_id_info.Relknoten;
	VSPSysData.active_prog = 1;
	VSPSysData.sw_off_time = g_sw_prg_list.list[0].time;

	for(i = 0; i < MAX_SIG_GROUP; i++){
		VSPSysData.sw_off_sg[i] = g_sw_prg_list.list[0].sg[i];
	}

	for (i = 0; i < g_prg_info.num; i++) {
		index = g_prg_info.node[i].id;
		VSPSysData.vsp_cycletime[index - 1] = g_prg_info.node[i].tu;
	}
	for (index = 0; index < g_inter_info.num; index++) {
		i = g_inter_info.node[index].sgr - 1;
		j = g_inter_info.node[index].sge - 1;
		val = g_inter_info.node[index].val;
		VSPSysData.inter_green[i][j] = val;
	}

	int max = 0;
	for (i = 0; i < g_det_info.num; i++) {
		index = g_det_info.node[i].id;
		max = (max < index) ? index : max;
		VSPDetData.VSPDetDataList[index - 1].det_valid = 1;
	}
	VSPDetData.MaxDetDataNum = max;

	max = 0;
	for (i = 0; i < g_sg_info.num; i++) {
		index = g_sg_info.node[i].id;
		max = (max < index) ? index : max;
		VSPSigData.VSPSigDataList[index - 1].sig_valid = 1;
		VSPSigData.VSPSigDataList[index - 1].sig_state = 2;
		VSPSigData.VSPSigDataList[index - 1].amber_time =
				g_sg_info.node[i].amber;
		VSPSigData.VSPSigDataList[index - 1].minred_time =
				g_sg_info.node[i].close_sum;
		VSPSigData.VSPSigDataList[index - 1].prep_time = g_sg_info.node[i].prep;
		VSPSigData.VSPSigDataList[index - 1].mingreen_time =
				g_sg_info.node[i].free_sum;
		VSPSigData.VSPSigDataList[index - 1].green_blink =
				g_sg_info.node[i].green_blink;
	}
	VSPSigData.MaxSigDataNum = max;

	char filename[128] = { 0 };
	sprintf(filename, "%s.dat", g_outfile);
	//pf = fopen(VSP_PARAMFILE,"w");
	pf = fopen(filename, "w");
	if (fwrite(&VSPSysData, sizeof(VSPSysData), 1, pf) <= 0) {
		perror("fwrite[VSPSysData]");
		fclose(pf);
		return EXIT_FAILURE;
	}
	if (fwrite(&VSPDetData, sizeof(VSPDetData), 1, pf) <= 0) {
		perror("fwrite[VSPDetData]");
		fclose(pf);
		return EXIT_FAILURE;
	}
	if (fwrite(&VSPSigData, sizeof(VSPSigData), 1, pf) <= 0) {
		perror("fwrite[VSPSigData]");
		fclose(pf);
		return EXIT_FAILURE;
	}
	fclose(pf);

	printf("File[%s.dat] write success\n", g_outfile);

	return EXIT_SUCCESS;
}

int main(int argc, char* argv[])
{
	extern char* optarg;
	extern optind, opterr, optopt;
	opterr = 0;
	char xmlfile[128] = { 0 };

	char optstring[] = "vhd::";
	struct option long_options[] = { { "version", no_argument, 0, 'v' }, {
			"help", no_argument, 0, 'h' },
			{ "debug", optional_argument, 0, 'd' }, { 0, 0, 0, 0 } };
	int opt;
	int option_index;
	while ((opt = getopt_long(argc, argv, optstring, long_options,
			&option_index)) != -1) {
		switch (opt) {
		case 'h':
			printf("usage:%s xmlfile outfile\n", argv[0]);
			printf("\t-h,--help: help\n");
			printf("\t-v,--version: version\n");
			printf("\t-d[n],--debug=[n]: print debug info\n");
			return 0;
			break;
		case 'v':
			printf("version %s, build at: %s %s\n", g_version, __DATE__,
			__TIME__);
			return 0;
			break;
		case 'd':
			g_debug = optarg ? atoi(optarg) : 1;
			printf("debug_level:%d\n", optarg ? atoi(optarg) : 0);
			break;
		default:
			printf("unknown option: %s\n", argv[optind - 1]);
			printf("usage:%s xmlfile outfile, -h for more info \n", argv[0]);
			return -1;
			break;
		}
	}
	if (!argv[optind]) {
		printf("usage:%s xmlfile outfile, -h for more info \n", argv[0]);
		return -1;
	} else {
		sprintf(xmlfile, "%s", argv[optind]);
		optind++;
	}

	if (!argv[optind]) {
		printf("default outfile: vsp_param\n");
		sprintf(g_outfile, "%s", "vsp_param");
	} else {
		sprintf(g_outfile, "%s", argv[optind]);
		optind++;
	}

	parse_xml(xmlfile);

	vspconfig();
	//dump_vspconfig();

	return 0;
}
