#include <iostream>
#include <cstring>
using namespace std;
#define maxn 205
#define inf 100000
int G[maxn][maxn];
int prim(int start, int N);
int main()
{
    int N, M;
    while (cin >> N >> M)
    {
        // memset(G, inf, sizeof(G));
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                G[i][j] = inf;
        while (M--)
        {
            int a, b, l;
            cin >> a >> b >> l;
            G[a][b] = G[b][a] = l;
        }
        //prim
        int ans = prim(0, N);
        cout << endl; 
        if (ans == -1)
            cout << "There is no minimum spanning tree.\n";
        else
            cout << ans << endl;
    }
}
int prim(int start, int N)
{
    // ds vis p lowcost
    int vis[maxn] = {0}, p[maxn] = {0}, lowcost[maxn] = {0};
    vis[start] = 1;
    for (int i = 0; i < N; ++i)
    {
        if (!vis[i]) //右边的 未加入的结点
        {
            p[i] = start;
            lowcost[i] = G[start][i];
        }
    }
    int sum = 0;
    //每次循环加入一个 结点 一共需要n-1次
    for (int i = 0; i < N - 1; ++i)
    {
        int mini = inf + 1;
        int idx = -1;
        for (int i = 0; i < N; ++i) //找最小值
        {
            if (!vis[i] && lowcost[i] < mini)
            {
                mini = lowcost[i];
                idx = i;
            }
        }
        if (mini == inf)
            return -1;
        sum += mini;
        //最小值 是inf 就是没有路径 还有结点没有加到左边里面 故没有最小生成树
        vis[idx] = 1;
        for (int i = 0; i < N; ++i)
        {
            if (!vis[i] && G[i][idx] < lowcost[i])
            {
                lowcost[i] = G[i][idx];
                p[i] = idx;
            }
        }
    }
    return sum;
}

