#include<stdlib.h>
#include<malloc.h>
#include<Windows.h>
#include<stdio.h>
#include<string.h>
#include<conio.h>
#include<math.h>
typedef int datatype;
#define maxsize 10
#define maxvertics 10
#define maxweight 10000

typedef struct {
	datatype list[maxsize];
	int size;
}seqlist;

typedef struct {
	seqlist vertices;
	int edge[maxvertics][maxvertics];
	int numofedge;
}adjmgraph;

void listinitiate(seqlist*l) {
	l->size = 0;
}

void menu(adjmgraph* g1);

int listlength(seqlist l) {
	return l.size;
}
int listinsert(seqlist*l, int i, datatype x) {
	int j;
	//printf("%d", i);
	//printf("应有的数字\n");
	if (l->size >= maxsize) {
		printf("顺序表已满无法插入\n");
		return 0;
	}
	else if (i<0 || i>l->size) {
		printf("参数i不合法\n");
		return 0;
	}
	else {
		for (j = l->size; j > i; j--)
			l->list[j] = l->list[j - 1];
		l->list[i] = x;

		l->size++;
		return 1;

	}
}

int listdelete(seqlist*l, int i, datatype*x) {
	int j;
	if (l->size <= 0) {
		printf("顺序表已空无数据元素可删\n");
		return 0;
	}
	else if (i<0 || i>l->size - 1) {
		printf("参数i不合法\n");
		return 0;
	}
	else {
		*x = l->list[i];
		for (j = i + 1; j <= l->size - 1; j++)
			l->list[j - 1] = l->list[j];
		l->size--;
		return 1;

	}
}
int listget(seqlist l, int i, datatype*x) {
	if (i<0 || i>l.size - 1) {
		printf("参数i不合法\n");
		return 0;
	}
	else {
		*x = l.list[i];
		return 1;
	}
}

typedef struct {
	int row;
	int col;
	int weight;
}rowcolweight;

void graphinitiate(adjmgraph*g) {
	int i, j;
	for (i = 0; i < maxvertics; i++)
		for (j = 0; j < maxvertics; j++) {
			if (i == j) {
				g->edge[i][j] = 0;
			}
			else {
				g->edge[i][j] = maxweight;
			}
		}
	g->numofedge = 0;
	listinitiate(&g->vertices);
}

void insertvertex(adjmgraph*g, datatype vertex) {
	if (listinsert(&g->vertices, g->vertices.size, vertex) == 0) {
		printf("插入顶点时空间已满无法插入");
		exit(1);
	}
}

void insertedge(adjmgraph*g, int v1, int v2, int weight) {
	datatype x;
	if (v1 != v2) {
		if ((listget(g->vertices, v1, &x) == 0) || (listget(g->vertices, v2, &x) == 0)) {
			printf("插入边时参数v1和v2越界出错\n");
			exit(1);
		}
		g->edge[v1][v2] = weight;
		g->edge[v2][v1] = weight;
		g->numofedge++;
	}
}

void deleteedge(adjmgraph*g, int v1, int v2) {
	g->edge[v1][v2] = maxweight;
	g->edge[v2][v1] = maxweight;
	g->numofedge--;
}

void deletevertex(adjmgraph*g, int v) {
	int m3, i, j;
	m3 = v - 1;
	if (m3 < 0 || m3 >= g->vertices.size) {
		printf("此链路内没有您想要删除的路由器节点\n");
		exit(0);
	}
	else {
		// for (i = m3; i < g->vertices.size; i++)
		for (j = 0, i = m3; j < g->vertices.size; j++) {
			g->edge[j][i] = maxweight;
		}
		// for (i = m3; i < g->vertices.size; i++)
		for (i = m3, j = 0; j < g->vertices.size; j++)
			g->edge[i][j] = maxweight;
		// for (i = m3; i < g->vertices.size; i++)
		// g->vertices.list[i] = g->vertices.list[i] - 1;
		// g->vertices.size--;
		//printf("删除节点成功\n");
	}
}

int getfirstvex(adjmgraph g, int v) {
	int col;
	datatype x;
	v = v - 1;
	if (listget(g.vertices, v, &x) == 0) {
		printf("取第一个邻接点时参数v越界\n");
		exit(1);
	}
	for (col = 0; col < g.vertices.size; col++)
		if (g.edge[v][col] > 0 && g.edge[v][col] < maxweight)
			return col;
	return-1;
}

void _creatgraph(adjmgraph*g, datatype v[], int n, rowcolweight w[], int e) {
	int i, k;
	graphinitiate(g);
	for (i = 0; i < n; i++) {
		insertvertex(g, v[i]);
	}
	for (k = 0; k < e; k++)
		insertedge(g, w[k].row, w[k].col, w[k].weight);
}

void dijkstra(adjmgraph*g, int v0, int distance[], int path[]) {
	int n = g->vertices.size;
	int*s = (int*)malloc(sizeof(int)*n);
	int mindis, i, j, u;
	FILE*fp;
	for (i = 0; i < n; i++) {
		distance[i] = g->edge[v0][i];
		s[i] = 0;
		if (i != v0 && distance[i] < maxweight)
			path[i] = v0;
		else path[i] = -1;
	}
	s[v0] = 1;//标记v0点
	for (i = 1; i < n; i++) {
		mindis = maxweight;
		for (j = 0; j<n; j++)
			if (s[j] == 0 && distance[j] < mindis) {
				u = j;
				mindis = distance[j];
			}
		if (mindis == maxweight)
			return;
		s[u] = 1;//标记查找到的点
		for (j = 0; j<n; j++)
			if (s[j] == 0 && g->edge[u][j] < maxweight&&distance[u] + g->edge[u][j] < distance[j]) {
				distance[j] = distance[u] + g->edge[u][j];
				path[j] = u;
			}
		printf("目的路由    下一跳路由\n");
		fopen_s(&fp, "luyoubiao.txt", "w");
		for (i = 0; i < n; i++) {
			if (i == v0)
				continue;
			j = i;
			while (path[j] != v0) {
				j = path[j];
				if (j == -1)
					break;
			}
			printf("%5d%12d\n", i + 1, j + 1);
			fprintf(fp, "%5d%12d\n", i + 1, j + 1);
		}
		fclose(fp);
	}
}

int getnextvex(adjmgraph g, int v1, int v2) {
	int col;
	datatype x;
	if ((listget(g.vertices, v1, &x) == 0) || listget(g.vertices, v2, &x) == 0) {
		printf("取下一领接顶点时参数越界出错\n");
		exit(1);
	}
	if (g.edge[v1][v2] == 0) {
		printf("v2不是v1的领接顶点\n");
		exit(1);
	}

	for (col = v2 + 1; col < g.vertices.size; col++)
		if (g.edge[v1][col] > 0 && g.edge[v1][col] < maxweight)
			return col;
	return-1;
}

void Putlist(adjmgraph*g1)
{
	int i, j;
	int a[maxsize];
	int distance[maxsize], path[maxsize];
	printf("请输入你要生成的路由表的起始路由器是第几个路由器v\n");
	scanf_s("%d", &i);
	i--;
	dijkstra(g1, i, distance, path);
	printf("从顶点%d到其他各顶点的最短距离为：\n", i);
	for (j = 0; j < g1->vertices.size; j++)
		printf("到顶点%d的最短距离为%d\n", j, distance[j]);
}

void Deletevertex(adjmgraph*g1)
{
	int i;
	printf("请输入你要删除的是第几个顶点v\n");
	scanf_s("%d", &i);
	deletevertex(g1, i);
	menu(g1);
}


void Deleteedge(adjmgraph*g1)
{
	int i, j;
	printf("请输入你要删除的边的两个顶点v1,v2\n");
	scanf_s("%d%d", &i, &j);
	deleteedge(g1, i - 1, j - 1);
	menu(g1);
}

void Insertedge(adjmgraph*g1)
{
	int i, j, w;
	printf("请输入你要删除的边的两个顶点和其权直v1,v2,w\n");
	scanf_s("%d%d%d", &i, &j, &w);
	insertedge(g1, i - 1, j - 1, w);
	menu(g1);
}
void menu(adjmgraph*g1) {
	int p;
	printf("**********************网络路由生成************************\n");
	printf("**********************请选择相应实现功能******************\n");
	printf("**********************1.路由表输出并打印******************\n");
	printf("**********************2.删除节点**************************\n");
	printf("**********************3.删除边****************************\n");
	printf("**********************4.填加节点和边****************************\n");
	printf("**********************5.退出系统**************************\n");
	printf("**********************请输入你的选择（1~6）***************\n");
	scanf_s("%d", &p);
	if (p < 1 || p>5)
	{
		printf("\n\n输入序号不属于菜单，请重新输入\n\n");
		p = 6;
	}
	switch (p)
	{
	case 1:
		Putlist(g1);
		break;
	case 2:
		Deletevertex(g1);
		break;
	case 3:
		Deleteedge(g1);
		break;
	case 4:
		Insertedge(g1);
		break;
	case 5:
		exit(0);
		break;
	case 6:
		menu(g1);
	}
	menu(g1);
}





int main()
{
	int i, j, k;
	adjmgraph g1;
	int a[maxsize];
	rowcolweight rcw[100];
	FILE *fp = NULL;
	fopen_s(&fp, "tuoputu.txt", "r");
	fscanf_s(fp, "%d", &i);
	for (k = 0; k < i; k++)
	{
		fscanf_s(fp, "%d", &a[k]);
	}
	fscanf_s(fp, "%d", &j);
	for (k = 0; k < j; k++)
	{
		fscanf_s(fp, "%d%d%d", &rcw[k].row, &rcw[k].col, &rcw[k].weight);
	}
	fclose(fp);
	graphinitiate(&g1);
	_creatgraph(&g1, a, i, rcw, j);
	menu(&g1);
}

