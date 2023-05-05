#include<iostream>
#include "../BinNode/BinNode.h"//����������ڵ���
#include "../List/ListNode.h"
using namespace std;
template <typename T> class BinTree{ //������ģ���� 

protected:
	int _size; BinNodePosi(T)_root;//��ģ�����ڵ�
	virtual int updateHeight(BinNodePosi(T)x);//���½ڵ�x�ĸ߶�
	void updateHeightAbove(BinNodePosi(T)x);//���½ڵ�x�������ȵĸ߶� 
public:
	BinTree():_size( 0 ),_root(NULL ){} //���캯��
	~BinTree(){ if( 0 < _size) remove( _root ); } //�������� 
	int size() const { return _size; } //��ģ 
	bool empty() const{ return !_root; }//�п�
	BinNodePosi(T) root() const{ return _root; } //����
	BinNodePosi(T) insertAsRoot(T const&e);//������ڵ�
	BinNodePosi(T) insertAsLC( BinNodePosi(T) x, T const& e ); //e��Ϊx������(ԭ��)���� 
	BinNodePosi(T) insertAsRC( BinNodePosi(T) x, T const& e ); //e��Ϊx���Һ���(ԭ��)����
	BinNodePosi(T) attachAsLC( BinNodePosi(T) x, BinTree<T>* & U );//U��Ϊx���������� 
	BinNodePosi(T) attachAsRC( BinNodePosi(T) x, BinTree<T>* & U );//U��Ϊx���������� 
	int remove(BinNodePosi( T ) x );//ɾ����λ��x���ڵ�Ϊ�������������ظ�����ԭ�ȵĹ�ģ
	BinTree<T>* secede( BinNodePosi( T ) x);//������x�ӵ�ǰ����ժ����������ת��Ϊһ�ö�������
	template <typename VST> void travPre(BinNodePosi(T) x, VST& visit) { if (_root) _root->travPre(visit); }//�������
    template <typename VST> void travPost(const VST& visit) { if (_root) _root->travPost(visit); }//�������
	template <typename VST> void travLevel(const VST& visit) { if (_root) _root->travLevel(visit); }//��α���
	template <typename VST> void travIn( const VST& visit) { if (_root) _root->travIn(visit); }//�������
	
	
	template <typename VST> //������
	void travLevel(VST& visit ){ if(_root)_root->travLevel( visit ); } //��α��� 
	template <typename VST> //������
	void travPre(VST& visit ){ if(_root)_root->travPre( visit ); } //�������
	template <typename VST> //������
	void travIn ( VST& visit ){ if(_root ) root->travIn( visit ); } //������� 
	template <typename VST> //������
	void travPost( VST& visit ){ if(_root )_root->travPost( visit ); } //������� 
	bool operator<(BinTree<T> const&t)//�Ƚ���(�������в���)
	{ return _root && t._root && lt ( _root, t._root ); } 
	bool operator== ( BinTree<T> const&t)//�е���
	{ return _root && t._root && ( _root == t._root );}
}; //BinTree


template <typename T> int BinTree<T>::updateHeight(BinNodePosi( T ) x ) //���½ڵ�x�߶�
{ return x->height =1+ max( stature (x->lc ),stature( x->rc )); } //���������������

template <typename T> void BinTree<T>::updateHeightAbove ( BinNodePosi( T ) x )//���¸߶�
{ while ( x ) { updateHeight ( x ); x = x-> parent; }} //��x������������������,���Ż�


template <typename T> BinNodePosi(T) BinTree<T>::insertAsRoot (T const& e)
{_size = 1; return _root = new BinNode<T> ( e ); } //��e�������ڵ����յĶ�����

template <typename T> BinNodePosi(T) BinTree<T>::insertAsLC ( BinNodePosi(T) x, T const& e)
{_size++; x->insertAsLC (e); updateHeightAbove ( x ); return x->lc; } //e����Ϊx������ 

template <typename T> BinNodePosi(T) BinTree<T>::insertAsRC( BinNodePosi(T) x,T const& e)
{_size++; x->insertAsRC (e); updateHeightAbove ( x ); return x->rc; } //e����Ϊx���Һ��� 

template<typename T>//���������������㷨:��s�����ڵ�x�����������룬s�����ÿ�
BinNodePosi(T) BinTree<T>::attachAsLC( BinNodePosi(T) x, BinTree<T>* &S ) { //x->1c == NULL
	if( x->lc = S->_root ) x->lc->parent = x; //����
	_size += S->_size; updateHeightAbove( x ) ;//����ȫ����ģ��x�������ȵĸ߶�
	S->_root = NULL; S->_size = 0; releases ( S );S = NULL; return x;//�ͷ�ԭ�������ؽ���λ��
}

template <typename T>//���������������㷨:��s�����ڵ�x�����������룬s�����ÿ�
BinNodePosi(T) BinTree<T>::attachAsRC ( BinNodePosi(T) x,BinTree<T>* &S ) { //x->rc == NULL
	if ( x->rc = S->_root ) x -> rc -> parent = x; //����
	_size += S -> _size; updateHeightAbove( x );//����ȫ����ģ��x�������ȵĸ߶�
	S->_root = NULL; S -> _size = 0; releases ( S );S = NULL; return x ; //�ͷ�ԭ�������ؽ���λ��
}

template <typename T>//ɾ����������λ��x���Ľڵ㼰���������ر�ɾ���ڵ����
int BinTree<T>::remove(BinNodePosi(T)x){//assert:xΪ�������еĺϷ�λ�� 
	FromParentTo( *x ) = NULL; //�ж����Ը��ڵ��ָ��
	updateHeightAbove(x -> parent); //�������ȸ߶�
	int n = removeAt( x ); _size -= n; return n ;//ɾ������x�����¹�ģ������ɾ���ڵ�����
}
template <typename T>//ɾ����������λ��x���Ľڵ㼰���������ر�ɾ���ڵ����ֵ
static int removeAt ( BinNodePosi( T ) x ) { //assert:xΪ�������еĺϷ�λ��
	if( !x ) return 0;//�ݹ��:����
	int n = 1 + removeAt( x -> lc ) + removeAt( x -> rc); //�ݹ��ͷ���������
	release( x -> data ); release( x ); return n ;//�ͷű�ժ���ڵ㣬������ɾ���ڵ�����
}

template <typename T>//���������������㷨:������x�ӵ�ǰ����ժ���������װΪһ�ö����������� 
BinTree<T>* BinTree<T>::secede ( BinNodePosi( T ) x ) {//assert:xΪ�������еĺϷ�λ�� 
    FromParentTo(*x)= NULL; //�ж����Ը��ڵ��ָ��
	updateHeightAbove( x -> parent );//����ԭ�����������ȵĸ߶�
	BinTree<T>* S = new BinTree<T>; S -> _root = x; x -> parent = NULL; //������xΪ�� 
	S -> size = x -> size(); _size -= S -> _size; return S;//���¹�ģ�����ط������������
}


//�������
template <typename T, typename VST> void travPre(BinNodePosi(T) x, VST& visit) {
	if( !x ) return;
	//BinNodePosi(T) x = root();
	visit( x->data );
	travPre( x->lc, visit );
	travPre( x->rc, visit );

}
//�������
template <typename T, typename VST> void travPost(BinNodePosi(T) x, VST& visit) {
	if( !x ) return;
	travPost( x->lc, visit );
	travPost( x->rc, visit );
	visit( x->data );

}
//�������
template <typename T, typename VST> void travIn(BinNodePosi(T) x, VST& visit) {
	if( !x ) return;
	travIn( x->lc, visit );
	visit( x->data );
	travIn( x->rc, visit );


}








