#include<iostream>
#include"../Stack/Stack.h"
#include"../Queue2/Queue2.h"
using namespace std;

typedef enum { UNDISCOVERED, DISCOVERED, VISITED } VStatus; //����״̬
typedef enum { UNDETERMINED, TREE, CROSS, FORWARD, BACKWARD } EType; //���ڱ�����������������

template <typename Tv, typename Te> //�������͡�������
class Graph { //ͼGraphģ����
private:
	void reset() { //���ж��㡢�ߵĸ�����Ϣ��λ
       for ( Rank v = 0; v < n; v++ ) { //���ж����
          status( v ) = UNDISCOVERED; dTime( v ) = fTime( v ) = -1; //״̬��ʱ���ǩ
          parent( v ) = -1; priority( v ) = INT_MAX; //���ڱ������еģ����ڵ㣬���ȼ���
          for ( Rank u = 0; u < n; u++ ) //���бߵ�
             if ( exists( v, u ) ) type( v, u ) = UNDETERMINED; //����
       }
    }
    void BFS( int, int& ); //����ͨ�򣩹�����������㷨
    void DFS( int, int& ); //����ͨ��������������㷨
    void BCC( int, int&, Stack<int>& ); //����ͨ�򣩻���DFS��˫��ͨ�����ֽ��㷨
    bool TSort( int, int&, Stack<Tv>* ); //����ͨ�򣩻���DFS�����������㷨
    template <typename PU> void PFS( int, PU ); //����ͨ�����ȼ��������
public:
// ����
    int n; //��������
    virtual int insert( Tv const& ) = 0; //���붥�㣬���ر��
    virtual Tv remove( int ) = 0; //ɾ�����㼰������ߣ����ظö�����Ϣ
    virtual Tv& vertex( int ) = 0; //��������ݣ��ö����ȷ���ڣ�
    virtual int inDegree( int ) = 0; //�������ȣ��ö����ȷ���ڣ�
    virtual int outDegree( int ) = 0; //����ĳ��ȣ��ö����ȷ���ڣ�
	virtual int firstNbr( int ) = 0; //������׸��ڽӶ���
    virtual int nextNbr( int, int ) = 0; //���㣨��Ե�ǰ�ھӵģ���һ�ھ�
    virtual VStatus& status( int ) = 0; //�����״̬
    virtual int& dTime( int ) = 0; //�����ʱ���ǩdTime
    virtual int& fTime( int ) = 0; //�����ʱ���ǩfTime
    virtual int& parent( int ) = 0; //�����ڱ������еĸ���
    virtual int& priority( int ) = 0; //�����ڱ������е����ȼ���
// �ߣ�����Լ��������߾�ͳһת��Ϊ�������һ������ߣ��Ӷ�������ͼ��������ͼ������
    int e; //������
    virtual bool exists( int, int ) = 0; //��(v, u)�Ƿ����
    virtual void insert( Te const&, int, int, int ) = 0; //����������֮�����ָ��Ȩ�صı�
    virtual Te remove( int, int ) = 0; //ɾ��һ�Զ���֮��ıߣ����ظñ���Ϣ
    virtual EType& type( int, int ) = 0; //�ߵ�����
	virtual Te& edge( int, int ) = 0; //�ߵ����ݣ��ñߵ�ȷ���ڣ�
    virtual int& weight( int, int ) = 0; //��(v, u)��Ȩ��
// �㷨
	void bfs( int ); //������������㷨
    void dfs( int ); //������������㷨
    void bcc( int ); //����DFS��˫��ͨ�����ֽ��㷨
    Stack<Tv>* tSort( int ); //����DFS�����������㷨
    void prim( int ); //��С֧����Prim�㷨
    void dijkstra( int ); //���·��Dijkstra�㷨
    template <typename PU> void pfs( int, PU ); //���ȼ��������
};


template <typename Tv, typename Te> //�����������BFS�㷨��ȫͼ��
void Graph<Tv, Te>::bfs( Rank s ) { // s < n
    reset(); Rank dClock = 0; //ȫͼ��λ
    for ( Rank v = s; v < s + n; v++ ) //��s��˳�μ�����ж���
       if ( UNDISCOVERED == status( v % n ) ) //һ��������δ������
          BFS( v % n, dClock ); //��������������һ��BFS
 } //��˿���������ȫͼ�������帴�Ӷ���Ȼ����ΪO(n+e)



template <typename Tv, typename Te> //�����������BFS�㷨��������ͨ��
void Graph<Tv, Te>::BFS( Rank v, Rank& dClock ) { // v < n
    Queue < Rank > Q; status( v ) = DISCOVERED; Q.enqueue( v ); dTime( v ) = dClock++; //������
    for ( Rank fClock = 0; !Q.empty(); ) { //��Q���֮ǰ��������
       if ( dTime( v ) < dTime( Q.front() ) ) //dTime�����ӣ���ζ�ſ����µ�һ�������
          dClock++, fClock = 0; //dTime������fTime��λ
       v = Q.dequeue(); //ȡ���׶���v����
      for ( Rank u = firstNbr( v ); -1 != u; u = nextNbr( v, u ) ) //����v��ÿһ���ھ�u
          if ( UNDISCOVERED == status( u ) ) { //��u��δ�����֣�����֮
             status( u ) = DISCOVERED; Q.enqueue( u ); dTime( u ) = dClock;
             type( v, u ) = TREE; parent( u ) = v; //�������ߣ���չBFS��
          } else //��u�ѱ����֣����������ѷ�����ϣ���
             type( v, u ) = CROSS; //��(v, u)�����ڿ��
       status( v ) = VISITED; fTime( v ) = fClock++; //���ˣ�v�������
    } //for
} //BFS



template <typename Tv, typename Te> //�����������DFS�㷨��ȫͼ�� 
void Graph<Tv, Te>::dfs( Rank s ) { // s < n 
 	reset(); Rank clock = 0; int v = s;
	do //ȫͼ��λ  
 		if ( UNDISCOVERED == status( v ) ) //һ��������δ������ 
 			DFS( v , clock ); //��������������һ��DFS 
 	while (s != ( v = (++v %  n) ) );
 } 
 
 template <typename Tv, typename Te> //�����������DFS�㷨��������ͨ�� 
 void Graph<Tv, Te>::DFS( Rank v, Rank& clock ) { // v < n 
 	dTime( v ) = clock++; status( v ) = DISCOVERED; //���ֵ�ǰ����v 
 	for ( Rank u = firstNbr( v ); - 1 != u; u = nextNbr( v, u ) ) //����v��ÿһ���ھ�u 
 		switch ( status( u ) ) { //������״̬�ֱ��� 
 			case UNDISCOVERED : // u��δ���֣���ζ��֧�������ڴ���չ 
 				type( v, u ) = TREE; parent( u ) = v; DFS( u, clock ); break; 
 			case DISCOVERED : // u�ѱ����ֵ���δ������ϣ�Ӧ�������ָ������� 
 				type( v, u ) = BACKWARD; break; 
 			default : // u�ѷ�����ϣ�VISITED������ͼ�������ӳ�Ϯ��ϵ��Ϊǰ��߻��� 
 				type( v, u ) = ( dTime( v ) < dTime( u ) ) ? FORWARD : CROSS; break; 
 		} 
 	status( v ) = VISITED; fTime( v ) = clock++; //���ˣ���ǰ����v���������� 
}





