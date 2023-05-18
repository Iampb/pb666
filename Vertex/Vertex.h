#include "../vector/Vector.h" //��������
#include "../Graph/Graph.h" //����ͼADT
#include<iostream>
using namespace std;

template <typename Tv> struct Vertex { //�������Ϊ���������δ�ϸ��װ��
	Tv data; int inDegree, outDegree; VStatus status; //���ݡ����������״̬
    int dTime, fTime; //ʱ���ǩ
    Rank parent; int priority; //�ڱ������еĸ��ڵ㡢���ȼ���
    Vertex ( Tv const& d = ( Tv ) 0 ) : //�����¶���
        data ( d ), inDegree ( 0 ), outDegree ( 0 ), status ( UNDISCOVERED ),
        dTime ( -1 ), fTime ( -1 ), parent ( -1 ), priority ( INT_MAX ) {} //�ݲ�����Ȩ�����
};

template <typename Te> struct Edge { //�߶���Ϊ���������δ�ϸ��װ��
    Te data; int weight; EType type; //���ݡ�Ȩ�ء�����
    Edge ( Te const& d, int w ) : data ( d ), weight ( w ), type ( UNDETERMINED ) {} //����
};

template <typename Tv, typename Te> //�������͡�������
class GraphMatrix : public Graph<Tv, Te> { //�������������ڽӾ�����ʽʵ�ֵ�ͼ
private:
   Vector< Vertex< Tv > > V; //���㼯��������
   Vector< Vector< Edge< Te > * > > E; //�߼����ڽӾ���
public:
   GraphMatrix() { int n ; int e = 0; } //����
   ~GraphMatrix() { //����
   		int n;
      for ( Rank v = 0; v <  n; v++ ) //���ж�̬������
         for ( Rank u = 0; u < n; u++ ) //�߼�¼
            delete E[v][u]; //�������
   }
   
   
// ����Ļ�����������ѯ��v�����㣨0 <= v < n��
   virtual Tv& vertex ( Rank v ) { return V[v].data; } //����
   virtual int inDegree ( Rank v ) { return V[v].inDegree; } //���
   virtual int outDegree ( Rank v ) { return V[v].outDegree; } //����
   virtual Rank firstNbr ( Rank v ) { int n ;return nextNbr ( v,  n ); } //�׸��ڽӶ���
   virtual Rank nextNbr ( Rank v, Rank u ) //����ڶ���u����һ�ڽӶ��㣨�����ڽӱ�����Ч�ʣ�
   { while ( ( -1 < u ) && ( !exists ( v, --u ) ) ); return u; } //����������̽
   virtual VStatus& status ( Rank v ) { return V[v].status; } //״̬
   virtual int& dTime ( Rank v ) { return V[v].dTime; } //ʱ���ǩdTime
   virtual int& fTime ( Rank v ) { return V[v].fTime; } //ʱ���ǩfTime
   virtual Rank& parent ( Rank v ) { return V[v].parent; } //�ڱ������еĸ���
   virtual int& priority ( Rank v ) { return V[v].priority; } //�ڱ������е����ȼ���
   
   
// ����Ķ�̬����
   virtual Rank insert ( Tv const& vertex ) { //���붥�㣬���ر��
      int n; 
      for ( Rank u = 0; u < n; u++ ) E[u].insert ( NULL ); n++; //������Ԥ��һ��Ǳ�ڵĹ�����
      E.insert ( Vector<Edge<Te>*> ( n, n, ( Edge<Te>* ) NULL ) ); //�����¶����Ӧ�ı�����
      return V.insert ( Vertex<Tv> ( vertex ) ); //������������һ������
   }
   virtual Tv remove ( Rank v ) { //ɾ����v�����㼰������ߣ�0 <= v < n��
      int n ;
      for ( Rank u = 0; u < n; u++ ) //���г���
         if ( exists ( v, u ) ) { delete E[v][u]; V[u].inDegree--; } //����ɾ��
      E.remove ( v ); n--; //ɾ����v��
      Tv vBak = vertex ( v ); V.remove ( v ); //ɾ������v
      for ( Rank u = 0; u < n; u++ ) //�������
         if ( Edge<Te> * x = E[u].remove ( v ) ) { delete x; V[u].outDegree--; } //����ɾ��
      return vBak; //���ر�ɾ���������Ϣ
   }
   
   
// �ߵ�ȷ�ϲ���
   virtual bool exists ( Rank v, Rank u ) //��(v, u)�Ƿ����
   { int n; return ( v < n ) && ( u < n ) && E[v][u] != NULL; }
   
   
// �ߵĻ�����������ѯ����v��u֮������ߣ�0 <= v, u < n��exists(v, u)��
   virtual EType & type ( Rank v, Rank u ) { return E[v][u]->type; } //��(v, u)������
   virtual Te& edge ( Rank v, Rank u ) { return E[v][u]->data; } //��(v, u)������
   virtual int& weight ( Rank v, Rank u ) { return E[v][u]->weight; } //��(v, u)��Ȩ��
   
   
// �ߵĶ�̬����
   virtual void insert ( Te const& edge, int w, Rank v, Rank u, int e ) { //����Ȩ��Ϊw�ı�(v, u)
      if ( exists ( v, u ) ) return; //ȷ���ñ��в�����
      E[v][u] = new Edge<Te> ( edge, w ); //�����±�
      e++; V[v].outDegree++; V[u].inDegree++; //���±߼������������Ķ���
   }
   
   
   
   virtual Te remove ( Rank v, Rank u , int e ) { //ɾ������v��u֮������ߣ�exists(v, u)��
      Te eBak = edge ( v, u ); delete E[v][u]; E[v][u] = NULL; //���ݺ�ɾ���߼�¼
      e--; V[v].outDegree--; V[u].inDegree--; //���±߼������������Ķ���
      return eBak; //���ر�ɾ���ߵ���Ϣ
   }
};
