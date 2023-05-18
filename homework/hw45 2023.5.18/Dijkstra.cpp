#include <iostream>
#define Max 503
#define INF 0xcffffff

using namespace std;

typedef struct AMGraph {		
	int vex, arc;
	int arcs[Max][Max];						
};

int dis[Max], path[Max];							
bool book[Max];									

void Dijkstra(AMGraph &G)						
{
	for (int i = 1; i <= G.vex; i++)				
	{
		dis[i] = G.arcs[1][i];						
		path[i] = dis[i] < INF ? 1 : -1;			
	}
	book[1] = true;									
	dis[1] = 0;									
	for (int i = 2; i <= G.vex; i++)				
	{
		int mins = INF, u = 1;
		for (int j = 1; j <= G.vex; j++)			
		{
			if (!book[j] && mins > dis[j]) {
				mins = dis[j];
				u = j;
			}
		}
		book[u] = true;							
		for (int j = 1; j <= G.vex; j++)			
		{
			if (!book[j] && dis[j] > dis[u] + G.arcs[u][j]) {
				dis[j] = dis[u] + G.arcs[u][j];		
				path[j] = u;						
			}
		}
	}
}

void find(int x)//递归输出最短路径
{
	if (path[x] == 1) {
		cout << 1;
	}
	else {
		find(path[x]);
	}
	cout << " -> " << x;
	return;
}

void putin(AMGraph &G)//输入图
{
	cin >> G.vex >> G.arc;
	for (int i = 1; i <= G.vex; i++)//初始化邻接矩阵
		for (int j = 1; j <= G.vex; j++)
			G.arcs[i][j] = INF;

	for (int i = 1; i <= G.arc; i++)			
	{
		int u, v, w;
		cin >> u >> v >> w;
		G.arcs[u][v] = w;
	}
}

void putout(AMGraph &G)	//输出
{
	cout << "起点 v1 到各点的最短路程为: \n";
	for (int i = 1; i < G.vex; i++)
	{
		cout << dis[i] << " ";
	}
	cout << dis[G.vex] << endl;
	for (int i = 2; i <= G.vex; i++)
	{
		cout << "起点 v1 到 v" << i << " 的路径为： ";
		find(i);
		cout << endl;
	}
}

int main()
{
	AMGraph G;
	putin(G);
	Dijkstra(G);
	putout(G);
	return 0;
}

