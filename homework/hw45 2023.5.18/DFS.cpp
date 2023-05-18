#include<iostream>
#include<string>
using namespace std;

//使用邻接矩阵完成图的遍历
struct Graph_array {
    int vexnum;  //图的顶点数
    int edge;    //图的边数
    int ** arc;  //邻接矩阵
    int kind;    //0,为有向图，1，为无向图
    string * infromation; //表示每个顶点的信息

};
//使用邻接矩阵表示的图
void createGraph_by_array(int **edge,Graph_array & g) {
    int i = 0;
    g.arc = new int*[g.vexnum];//为邻接矩阵开辟空间
    for (i = 0; i < g.vexnum; i++)
    {
        g.arc[i] = new int[g.vexnum];
        for (int j = 0; j < g.vexnum; j++)
            g.arc[i][j] = 0;
    }   
    for (i = 0; i < g.edge; i++)
    {
        //对矩阵进行赋值
        g.arc[edge[i][0] - 1][edge[i][1] - 1] = 1;
    }
}
//打印邻接矩阵
void print_array(Graph_array g) {

    int i = 0;
    for (i = 0; i <g.vexnum; i++) {
        //cout << g.infromation[i] << " ";
        for (int j = 0; j < g.vexnum; j++) {
            cout << g.arc[i][j] << " ";
        }
        cout << endl;
    }
}
//进行DFS遍历，
void DFS_store_array(Graph_array g,int v,bool * & visit) {
    cout << g.infromation[v] << " ";
    visit[v] = true;
    for (int i = 0; i < g.vexnum; i++) {//找出下一个位被访问的顶点
        if (g.arc[v][i] == 0 || g.arc[v][i] == INT_MAX) { //如果两个顶点不存在边
            continue;
        }
        else if (!visit[i]) {//如果没有被访问，访问该结点
            DFS_store_array(g, i, visit);
        }
    }
}
//调用对应的DFS函数进行图的遍历
void DFS_array_travel(Graph_array g,int begin) {
    bool * visit;
    visit = new bool[g.vexnum];
    int i;
    for (i = 0; i < g.vexnum; i++) {
        visit[i] = false;
    }
    cout << "图的DFS遍历结果：" << endl;
    DFS_store_array(g,begin - 1,visit);
    //如果图是非联通同，那么我们这里还需要对每个顶点遍历一次，保证
    //全部顶点都被访问了
    for (i = 0; i < g.vexnum; i++) {
        if (!visit[i])
            DFS_store_array(g, i,visit);
    }

}

//使用邻接表表示图进行图的遍历
//表结点
struct ArcNode {
    int adfvex;//表示该边的另外一个顶点在顶点表中的下标
    ArcNode * next; //表示依附在该顶点的下一条边的信息
};
//头结点
struct Vnode {
    string data; //记录基本信息i
    ArcNode * firstarc;//记录第一条依附在该顶点的边
};
//一个图的结构
struct Graph_List {
    int vexnum;  //图的顶点数
    int edge;    //图的边数
    Vnode * node;  //邻接表
    int kind;    //0,为有向图，1，为无向图
};
//建立邻接表
void createGraph_list(Graph_List & g, int **edge) {
    int i;
    for (i = 0; i < g.edge; i++)
    {
        ArcNode * next=new ArcNode;
        next->adfvex=edge[i][1]-1;
        next->next = NULL;
        //判断该顶点的是否已经有边依附
        if (g.node[edge[i][0]-1].firstarc == NULL) {
            g.node[edge[i][0]-1].firstarc = next;
        }
        else {//寻找链表的最后一个结点
            ArcNode * now;
            now = g.node[edge[i][0]-1].firstarc;
            while (now->next) {
                now = now->next; 
            }
            now->next = next;
        }
    }
}
//打印邻接表
void print_list(Graph_List g) {
    int i;
    for (i = 0; i < g.vexnum; i++) {
        cout << g.node[i].data << " ";
        ArcNode * now;
        now = g.node[i].firstarc;
        while (now) {
            cout << now->adfvex << " ";
            now = now->next;
        }
        cout << endl;
    }
}
//使用DFS进行遍历图，在邻接表的情况下进行遍历
void DFS_store_list(Graph_List g, int v, bool * & visit) {
    cout << g.node[v].data << " ";
    visit[v] = true;
    ArcNode * next = g.node[v].firstarc;
    while (next) {
        if (!visit[next->adfvex]) {
            DFS_store_list(g, next->adfvex, visit);//递归
        }
        else {
            next = next->next;
        }
    }
}
//调用上面那个函数进行图的遍历
void DFS_list(Graph_List g, int begin) {
    int i;
    bool * visit = new bool[g.vexnum];
    for (i = 0; i < g.vexnum; i++) {
        visit[i] = false;
    }
    cout << "图的DFS遍历结果：" << endl;
    DFS_store_list(g, begin - 1, visit);
    for (i = 0; i < g.vexnum; i++) {
        if(!visit[i])
            DFS_store_list(g, i, visit);
    }
}
int main()
{
    Graph_array g;
    Graph_List G;
    int i;
    cout << "输入图的种类：" << endl;
    cin >> g.kind; G.kind = g.kind;

    cout << "输入图的顶点个数" << endl;
    cin >> g.vexnum; G.vexnum = g.vexnum;

    cout << "输入图的边的个数" << endl;
    cin >> g.edge; G.edge = g.edge;

    g.infromation = new string[g.vexnum];
    G.node = new Vnode[G.vexnum];


    cout << "输入每个顶点信息:" << endl;
    for (i = 0; i < g.vexnum; i++) {
        cin >> g.infromation[i];
        G.node[i].data = g.infromation[i];
        G.node[i].firstarc = NULL;
    }

    int ** edge_information;
    edge_information = new int*[g.edge];

    cout << "输入每条边两个顶点的编号：" << endl;
    for (i = 0; i < g.edge; i++)
    {
        edge_information[i] = new int[2];
        cin >> edge_information[i][0];
        cin >> edge_information[i][1];
    }

    int **arc; //邻接矩阵
    //构造邻接矩阵,其中最后一次参数：1，代表无向图，0，代表有向图
    createGraph_by_array(edge_information,g);
    cout << "图的邻接矩阵为：" << endl;
    print_array(g);
    cout << endl;
    DFS_array_travel(g, 1);
    cout << endl;

    createGraph_list(G, edge_information);
    cout << "图的邻接表为：" << endl;
    print_list(G);
    cout << endl;
    DFS_list(G, 1);
    cout << endl;
    system("pause");
    return 0;


}
