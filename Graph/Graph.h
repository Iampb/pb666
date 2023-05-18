#include<iostream>
#include"../Stack/Stack.h"
#include"../Queue2/Queue2.h"
using namespace std;

typedef enum { UNDISCOVERED, DISCOVERED, VISITED } VStatus; //顶点状态
typedef enum { UNDETERMINED, TREE, CROSS, FORWARD, BACKWARD } EType; //边在遍历树中所属的类型

template <typename Tv, typename Te> //顶点类型、边类型
class Graph { //图Graph模板类
private:
	void reset() { //所有顶点、边的辅助信息复位
       for ( Rank v = 0; v < n; v++ ) { //所有顶点的
          status( v ) = UNDISCOVERED; dTime( v ) = fTime( v ) = -1; //状态，时间标签
          parent( v ) = -1; priority( v ) = INT_MAX; //（在遍历树中的）父节点，优先级数
          for ( Rank u = 0; u < n; u++ ) //所有边的
             if ( exists( v, u ) ) type( v, u ) = UNDETERMINED; //类型
       }
    }
    void BFS( int, int& ); //（连通域）广度优先搜索算法
    void DFS( int, int& ); //（连通域）深度优先搜索算法
    void BCC( int, int&, Stack<int>& ); //（连通域）基于DFS的双连通分量分解算法
    bool TSort( int, int&, Stack<Tv>* ); //（连通域）基于DFS的拓扑排序算法
    template <typename PU> void PFS( int, PU ); //（连通域）优先级搜索框架
public:
// 顶点
    int n; //顶点总数
    virtual int insert( Tv const& ) = 0; //插入顶点，返回编号
    virtual Tv remove( int ) = 0; //删除顶点及其关联边，返回该顶点信息
    virtual Tv& vertex( int ) = 0; //顶点的数据（该顶点的确存在）
    virtual int inDegree( int ) = 0; //顶点的入度（该顶点的确存在）
    virtual int outDegree( int ) = 0; //顶点的出度（该顶点的确存在）
	virtual int firstNbr( int ) = 0; //顶点的首个邻接顶点
    virtual int nextNbr( int, int ) = 0; //顶点（相对当前邻居的）下一邻居
    virtual VStatus& status( int ) = 0; //顶点的状态
    virtual int& dTime( int ) = 0; //顶点的时间标签dTime
    virtual int& fTime( int ) = 0; //顶点的时间标签fTime
    virtual int& parent( int ) = 0; //顶点在遍历树中的父亲
    virtual int& priority( int ) = 0; //顶点在遍历树中的优先级数
// 边：这里约定，无向边均统一转化为方向互逆的一对有向边，从而将无向图视作有向图的特例
    int e; //边总数
    virtual bool exists( int, int ) = 0; //边(v, u)是否存在
    virtual void insert( Te const&, int, int, int ) = 0; //在两个顶点之间插入指定权重的边
    virtual Te remove( int, int ) = 0; //删除一对顶点之间的边，返回该边信息
    virtual EType& type( int, int ) = 0; //边的类型
	virtual Te& edge( int, int ) = 0; //边的数据（该边的确存在）
    virtual int& weight( int, int ) = 0; //边(v, u)的权重
// 算法
	void bfs( int ); //广度优先搜索算法
    void dfs( int ); //深度优先搜索算法
    void bcc( int ); //基于DFS的双连通分量分解算法
    Stack<Tv>* tSort( int ); //基于DFS的拓扑排序算法
    void prim( int ); //最小支撑树Prim算法
    void dijkstra( int ); //最短路径Dijkstra算法
    template <typename PU> void pfs( int, PU ); //优先级搜索框架
};


template <typename Tv, typename Te> //广度优先搜索BFS算法（全图）
void Graph<Tv, Te>::bfs( Rank s ) { // s < n
    reset(); Rank dClock = 0; //全图复位
    for ( Rank v = s; v < s + n; v++ ) //从s起顺次检查所有顶点
       if ( UNDISCOVERED == status( v % n ) ) //一旦遇到尚未发现者
          BFS( v % n, dClock ); //即从它出发启动一次BFS
 } //如此可完整覆盖全图，且总体复杂度依然保持为O(n+e)



template <typename Tv, typename Te> //广度优先搜索BFS算法（单个连通域）
void Graph<Tv, Te>::BFS( Rank v, Rank& dClock ) { // v < n
    Queue < Rank > Q; status( v ) = DISCOVERED; Q.enqueue( v ); dTime( v ) = dClock++; //起点入队
    for ( Rank fClock = 0; !Q.empty(); ) { //在Q变空之前，反复地
       if ( dTime( v ) < dTime( Q.front() ) ) //dTime的增加，意味着开启新的一代，因此
          dClock++, fClock = 0; //dTime递增，fTime复位
       v = Q.dequeue(); //取出首顶点v，并
      for ( Rank u = firstNbr( v ); -1 != u; u = nextNbr( v, u ) ) //考查v的每一个邻居u
          if ( UNDISCOVERED == status( u ) ) { //若u尚未被发现，则发现之
             status( u ) = DISCOVERED; Q.enqueue( u ); dTime( u ) = dClock;
             type( v, u ) = TREE; parent( u ) = v; //引入树边，拓展BFS树
          } else //若u已被发现，或者甚至已访问完毕，则
             type( v, u ) = CROSS; //将(v, u)归类于跨边
       status( v ) = VISITED; fTime( v ) = fClock++; //至此，v访问完毕
    } //for
} //BFS



template <typename Tv, typename Te> //深度优先搜索DFS算法（全图） 
void Graph<Tv, Te>::dfs( Rank s ) { // s < n 
 	reset(); Rank clock = 0; int v = s;
	do //全图复位  
 		if ( UNDISCOVERED == status( v ) ) //一旦遇到尚未发现者 
 			DFS( v , clock ); //即从它出发启动一次DFS 
 	while (s != ( v = (++v %  n) ) );
 } 
 
 template <typename Tv, typename Te> //深度优先搜索DFS算法（单个连通域） 
 void Graph<Tv, Te>::DFS( Rank v, Rank& clock ) { // v < n 
 	dTime( v ) = clock++; status( v ) = DISCOVERED; //发现当前顶点v 
 	for ( Rank u = firstNbr( v ); - 1 != u; u = nextNbr( v, u ) ) //考查v的每一个邻居u 
 		switch ( status( u ) ) { //并视其状态分别处理 
 			case UNDISCOVERED : // u尚未发现，意味着支撑树可在此拓展 
 				type( v, u ) = TREE; parent( u ) = v; DFS( u, clock ); break; 
 			case DISCOVERED : // u已被发现但尚未访问完毕，应属被后代指向的祖先 
 				type( v, u ) = BACKWARD; break; 
 			default : // u已访问完毕（VISITED，有向图），则视承袭关系分为前向边或跨边 
 				type( v, u ) = ( dTime( v ) < dTime( u ) ) ? FORWARD : CROSS; break; 
 		} 
 	status( v ) = VISITED; fTime( v ) = clock++; //至此，当前顶点v方告访问完毕 
}





