#include <stdlib.h>
#define BinNodePosi(T) BinNode<T>* //�ڵ�λ��
#define stature(p) ((p) ? (p)->height : -1)//�ڵ�߶�(�롰�����߶�Ϊ-1����Լ����ͳһ)
typedef enum { RB_RED, RB_BLACK} RBColor;//�ڵ���ɫ

template <typename T> struct BinNode{//�������ڵ�ģ����
// ��Ա(Ϊ���������ͳһ���ţ����߿ɸ�����Ҫ��һ����װ)
	T data; //��ֵ 
	BinNodePosi(T) parent; BinNodePosi(T) lc; BinNodePosi(T) rc;//���ڵ㼰���Һ���
    int height; //�߶�(ͨ��)
 	int npl; //Null Path Length(��ʽ�ѣ�Ҳ��ֱ����height����) 
 	RBColor color;//��ɫ(�����) 
 // ���캯��
	BinNode():
		parent (NULL ),lc( NULL ),rc(NULL),height(0),npl (1), color ( RB_RED ) { } 
	BinNode(T e, BinNodePosi(T) p = NULL, BinNodePosi(T) lc = NULL, BinNodePosi(T) rc = NULL, 
			int h = 0, int l = 1, RBColor c = RB_RED ) : 
		data(e),parent(p),lc(lc ),rc(rc ), height(h ),npl ( 1 ),color ( c ){ }
// �����ӿ�
	int size();//ͳ�Ƶ�ǰ�ڵ����������༴����Ϊ���������Ĺ�ģ
	BinNodePosi(T) insertAsLC(T const&);//��Ϊ��ǰ�ڵ�����Ӳ����½ڵ�
	BinNodePosi(T) insertAsRC(T const& );//��Ϊ��ǰ�ڵ���Һ��Ӳ����½ڵ� 
	BinNodePosi(T)succ();//ȡ��ǰ�ڵ��ֱ�Ӻ�� 
	template <typename VST> void travLevel(VST& );//������α��� 
	template <typename VST> void travPre( VST& );//����������� 
	template <typename VST> void travIn( VST& );//�����������
	template <typename VST> void travPost(VST& );//�����������
// �Ƚ������е���(������һ���������в���)
	bool operator<( BinNode const& bn ) { return data < bn.data; } //С��28
	bool operator==( BinNode const& bn ){ return data -= bn.data;}//����
};


#define IsRoot(x)(!((x).parent))
#define IsLChild(x)(!IsRoot(x)&&(&(x)==(x).parent->lc))
#define IsRChild(x)(!IsRoot(x)&&(&(x)==(x).parent->rc))
#define HasParent(x)(!IsRoot(x))
#define HasLChild(x) ((x).lc )
#define HasRChild(x) ((x).rc )
#define HasChild(x)( HasLChild(x)||HasRChild(x))//����ӵ��һ������
#define HasBothChild(x)( HasLChild(x)&&HasRChild(x))//ͬʱӵ����������
#define IsLeaf(x) ( ! HasChild(x) )
#define sibling(p)/*�ֵ�*/\
(IsLChild(*(p)) ? (p)->parent->rc :(p)->parent->lc )

#define uncle(x)/*����*/\
(IsLChild(*((x)->parent))?(x)->parent->parent->rc :(x)->parent->parent->lc)

#define FromParentTo(x)/*���Ը��׵�����*/\
(IsRoot(x)? root :( IsLChild(x) ? (x).parent->lc :(x).parent->rc ))


template <typename T> BinNodePosi(T) BinNode<T>::insertAsLC(T const& e)
{ return lc = new BinNode( e, this ); } //��e��Ϊ��ǰ�ڵ�����Ӳ��������

template <typename T> BinNodePosi(T) BinNode<T>::insertAsRC(T const& e)
{ return rc = new BinNode( e, this ); } //��e��Ϊ��ǰ�ڵ���Һ��Ӳ��������


template <typename T> template <typename VST>//Ԫ�����͡�������
void BinNode<T>::travIn ( VST& visit ){ //��������������㷨ͳһ���
	switch( rand() % 5 ){//�˴������ѡ���������ԣ�������ѡ��
		case 1: travIn_I1 ( this, visit ); break; //������#1 
		case 2: travIn_I2 ( this, visit ); break; //������#2 
		case 3: travIn_I3 ( this, visit ); break; //������#3 
		case 4: travIn_I4 ( this, visit ); break; //������#4 
		default: travIn_R ( this, visit ); break; //�ݹ��
	}
}























