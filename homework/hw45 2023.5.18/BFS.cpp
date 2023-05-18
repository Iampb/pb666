#include<iostream>
#include<string>
#include<queue>
using namespace std;

//ʹ���ڽӾ������ͼ�ı���
struct Graph_array {
    int vexnum;  //ͼ�Ķ�����
    int edge;    //ͼ�ı���
    int ** arc;  //�ڽӾ���
    int kind;    //0,Ϊ����ͼ��1��Ϊ����ͼ
    string * infromation; //��ʾÿ���������Ϣ

};
//ʹ���ڽӾ����ʾ��ͼ
void createGraph_by_array(int **edge, Graph_array & g) {
    int i = 0;
    g.arc = new int*[g.vexnum];//Ϊ�ڽӾ��󿪱ٿռ�
    for (i = 0; i < g.vexnum; i++)
    {
        g.arc[i] = new int[g.vexnum];
        for (int j = 0; j < g.vexnum; j++)
            g.arc[i][j] = 0;
    }
    for (i = 0; i < g.edge; i++)
    {
        //�Ծ�����и�ֵ
        g.arc[edge[i][0] - 1][edge[i][1] - 1] = 1;
    }
}
//��ӡ�ڽӾ���
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

//���ö�Ӧ��BFS��������ͼ�ı���
void BFS_array_travel(Graph_array g, int begin) {
    bool * visit;
    visit = new bool[g.vexnum];
    int i;
    for (i = 0; i < g.vexnum; i++) {
        visit[i] = false;
    }
    cout << "ͼ��BFS���������" << endl;
    //ͨ������֮ǰ˵���㷨˼·�����ǿ���֪��
    //������Ҫʹ���Ƚ��ȳ������ݴ洢�ṹʵ�����ǵ�BFS����ʵ�Ǿ��Ƕ���
    queue<int>  q;
    for (int v = 0; v < g.vexnum; v++) {//����ѭ����Ϊ�˱�֤����ͨͬ������£�ÿ�����㶼���Ա�����
        if (!visit[(begin-1 + v) % g.vexnum])//ע����㲻һ����v1
        {
            cout << g.infromation[(begin - 1 + v) % g.vexnum] << " ";
            visit[(begin - 1 + v) % g.vexnum] = true;
            q.push((begin - 1 + v) % g.vexnum);//��ʼ�����ǵĶ���
            while (!q.empty())
            {
                int u = q.front();
                q.pop();
                for (int j = 0; j < g.vexnum; j++) {
                    if (g.arc[u][j] == 0 || g.arc[u][j] == INT_MAX) { //����������㲻���ڱ�
                        continue;
                    }
                    else if (!visit[j] ) {//�ȷ������к�u�����Ķ��㣬���Ұ����Ǽ������
                        cout << g.infromation[j] << " ";
                        visit[j] = true;
                        q.push(j);
                    }
                }
            }
        }

    }
    cout << "���" << endl;

}

//ʹ���ڽӱ���ʾͼ����ͼ�ı���
//�����
struct ArcNode {
    int adfvex;//��ʾ�ñߵ�����һ�������ڶ�����е��±�
    ArcNode * next; //��ʾ�����ڸö������һ���ߵ���Ϣ
};
//ͷ���
struct Vnode {
    string data; //��¼������Ϣi
    ArcNode * firstarc;//��¼��һ�������ڸö���ı�
};
//һ��ͼ�Ľṹ
struct Graph_List {
    int vexnum;  //ͼ�Ķ�����
    int edge;    //ͼ�ı���
    Vnode * node;  //�ڽӱ�
    int kind;    //0,Ϊ����ͼ��1��Ϊ����ͼ
};
//�����ڽӱ�
void createGraph_list(Graph_List & g, int **edge) {
    int i;
    for (i = 0; i < g.edge; i++)
    {
        ArcNode * next = new ArcNode;
        next->adfvex = edge[i][1] - 1;
        next->next = NULL;
        //�жϸö�����Ƿ��Ѿ��б�����
        if (g.node[edge[i][0] - 1].firstarc == NULL) {
            g.node[edge[i][0] - 1].firstarc = next;
        }
        else {//Ѱ�����������һ�����
            ArcNode * now;
            now = g.node[edge[i][0] - 1].firstarc;
            while (now->next) {
                now = now->next;
            }
            now->next = next;
        }
    }
}
//��ӡ�ڽӱ�
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

//����BFS����ͼ�ı���
void BFS_list(Graph_List g, int begin) {
    int i;
    bool * visit = new bool[g.vexnum];
    for (i = 0; i < g.vexnum; i++) {
        visit[i] = false;
    }
    cout << "ͼ��BFS���������" << endl;
    queue<int>  q;
    for (int v = 0; v < g.vexnum; v++) {
        if (!visit[(begin - 1 + v) % g.vexnum])//ע����㲻һ����v1
        {
            cout << g.node[(begin - 1 + v) % g.vexnum].data << " ";
            visit[(begin - 1 + v) % g.vexnum] = true;
            q.push((begin - 1 + v) % g.vexnum);//��ʼ�����ǵĶ���
            while (!q.empty())
            {
                int u = q.front();
                q.pop();
                ArcNode * next;
                next = g.node[u].firstarc;//��������ڸö���ĵ�һ���ߵ���Ϣ
                while (next) {//�����������ϵ����еĵ�
                    if (!visit[next->adfvex]) {
                        cout << g.node[next->adfvex].data << " ";
                        visit[next->adfvex] = true;
                        q.push(next->adfvex);
                    }
                    next = next->next;
                }
            }
        }
    }
}
int main()
{
    Graph_array g;
    Graph_List G;
    int i;
    cout << "����ͼ�����ࣺ" << endl;
    cin >> g.kind; G.kind = g.kind;

    cout << "����ͼ�Ķ������" << endl;
    cin >> g.vexnum; G.vexnum = g.vexnum;

    cout << "����ͼ�ıߵĸ���" << endl;
    cin >> g.edge; G.edge = g.edge;

    g.infromation = new string[g.vexnum];
    G.node = new Vnode[G.vexnum];


    cout << "����ÿ��������Ϣ�������ƣ�:" << endl;
    for (i = 0; i < g.vexnum; i++) {
        cin >> g.infromation[i];
        G.node[i].data = g.infromation[i];
        G.node[i].firstarc = NULL;
    }

    int ** edge_information;
    edge_information = new int*[g.edge];

    cout << "����ÿ������������ı�ţ�" << endl;
    for (i = 0; i < g.edge; i++)
    {
        edge_information[i] = new int[2];
        cin >> edge_information[i][0];
        cin >> edge_information[i][1];
    }

    int **arc; //�ڽӾ���
               //�����ڽӾ���,�������һ�β�����1����������ͼ��0����������ͼ
    createGraph_by_array(edge_information, g);
    cout << "ͼ���ڽӾ���Ϊ��" << endl;
    print_array(g);
    cout << endl;
    BFS_array_travel(g, 1);
    cout << endl;

    createGraph_list(G, edge_information);
    cout << "ͼ���ڽӱ�Ϊ��" << endl;
    print_list(G);
    cout << endl;
    BFS_list(G, 1);
    cout << endl;
    system("pause");
    return 0;


}