#include <stdlib.h>
#define BinNodePosi(T) BinNode<T>* //节点位置
#define stature(p) ((p) ? (p)->height : -1)//节点高度(与“空树高度为-1”的约定相统一)
typedef enum { RB_RED, RB_BLACK} RBColor;//节点颜色

template <typename T> struct BinNode{//二叉树节点模板类
// 成员(为简化描述起见统一开放，读者可根据需要进一步封装)
	T data; //数值 
	BinNodePosi(T) parent; BinNodePosi(T) lc; BinNodePosi(T) rc;//父节点及左、右孩子
    int height; //高度(通用)
 	int npl; //Null Path Length(左式堆，也可直接用height代替) 
 	RBColor color;//颜色(红黑树) 
 // 构造函数
	BinNode():
		parent (NULL ),lc( NULL ),rc(NULL),height(0),npl (1), color ( RB_RED ) { } 
	BinNode(T e, BinNodePosi(T) p = NULL, BinNodePosi(T) lc = NULL, BinNodePosi(T) rc = NULL, 
			int h = 0, int l = 1, RBColor c = RB_RED ) : 
		data(e),parent(p),lc(lc ),rc(rc ), height(h ),npl ( 1 ),color ( c ){ }
// 操作接口
	int size();//统计当前节点后代总数，亦即以其为根的子树的规模
	BinNodePosi(T) insertAsLC(T const&);//作为当前节点的左孩子插入新节点
	BinNodePosi(T) insertAsRC(T const& );//作为当前节点的右孩子插入新节点 
	BinNodePosi(T)succ();//取当前节点的直接后继 
	template <typename VST> void travLevel(VST& );//子树层次遍历 
	template <typename VST> void travPre( VST& );//子树先序遍历 
	template <typename VST> void travIn( VST& );//子树中序遍历
	template <typename VST> void travPost(VST& );//子树后序遍历
// 比较器、判等器(各列其一，其余自行补充)
	bool operator<( BinNode const& bn ) { return data < bn.data; } //小于28
	bool operator==( BinNode const& bn ){ return data -= bn.data;}//等于
};


#define IsRoot(x)(!((x).parent))
#define IsLChild(x)(!IsRoot(x)&&(&(x)==(x).parent->lc))
#define IsRChild(x)(!IsRoot(x)&&(&(x)==(x).parent->rc))
#define HasParent(x)(!IsRoot(x))
#define HasLChild(x) ((x).lc )
#define HasRChild(x) ((x).rc )
#define HasChild(x)( HasLChild(x)||HasRChild(x))//至少拥有一个孩子
#define HasBothChild(x)( HasLChild(x)&&HasRChild(x))//同时拥有两个孩子
#define IsLeaf(x) ( ! HasChild(x) )
#define sibling(p)/*兄弟*/\
(IsLChild(*(p)) ? (p)->parent->rc :(p)->parent->lc )

#define uncle(x)/*叔叔*/\
(IsLChild(*((x)->parent))?(x)->parent->parent->rc :(x)->parent->parent->lc)

#define FromParentTo(x)/*来自父亲的引用*/\
(IsRoot(x)? root :( IsLChild(x) ? (x).parent->lc :(x).parent->rc ))


template <typename T> BinNodePosi(T) BinNode<T>::insertAsLC(T const& e)
{ return lc = new BinNode( e, this ); } //将e作为当前节点的左孩子插入二叉树

template <typename T> BinNodePosi(T) BinNode<T>::insertAsRC(T const& e)
{ return rc = new BinNode( e, this ); } //将e作为当前节点的右孩子插入二叉树


template <typename T> template <typename VST>//元素类型、操作器
void BinNode<T>::travIn ( VST& visit ){ //二叉树中序遍历算法统一入口
	switch( rand() % 5 ){//此处暂随机选择以做测试，共五种选择
		case 1: travIn_I1 ( this, visit ); break; //迭代版#1 
		case 2: travIn_I2 ( this, visit ); break; //迭代版#2 
		case 3: travIn_I3 ( this, visit ); break; //迭代版#3 
		case 4: travIn_I4 ( this, visit ); break; //迭代版#4 
		default: travIn_R ( this, visit ); break; //递归版
	}
}























