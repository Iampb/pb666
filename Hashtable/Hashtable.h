#include "../Dictionary/Dictionary.h"
#include "../Entry/Entry.h"
#include "../Bitmap/Bitmap.h"
#include<cstring>
#include<iostream>
using namespace std;
 
static size_t hashCode(char c) { return (size_t)c; } //字符
static size_t hashCode(int k) { return (size_t)k; } //整数以及长长整数
static size_t hashCode(long long i) { return (size_t)((i >> 32) + (int)i); }
static size_t hashCode(char s[]) { //生成字符串的循环移位散列码（cyclic shift hash code）
	int h = 0; //散列码
	for (size_t n = strlen(s), i = 0; i < n; i++) //自左向右，逐个处理每一字符
	{
		h = (h << 5) | (h >> 27); h += (int)s[i];
	} //散列码循环左移5位，再累加当前字符
	return (size_t)h; //如此所得的散列码，实际上可理解为近似的“多项式散列码”
} //对于英语单词，"循环左移5位"是实验统计得出的最佳值
 
//符合Dictionary接口的Hashtable模板类

template <typename K, typename V> class Hashtable : public Dictionary<K, V> { //key、value
	friend class UniPrint;
private:
	Entry <K, V>** ht; //桶数组，存放词条指针
	int M, N, L; //桶的总数、词条的数目、懒惰删除标记的数目（N + L <= M）
	Bitmap* removed; //懒惰删除标记
protected:
	int probe4Hit(const K& k); //沿关键码k对应的试探链，找到词条匹配的桶
	int probe4Free(const K& k); //沿关键码k对应的试探链，找到首个可用空桶
	void rehash(); //重散列算法：扩充桶数组，保证装填因子在警戒线以下
public:
	Hashtable(int c = 5); //创建一个容量不小于c的散列表（为测试暂时选用较小的默认值）
	~Hashtable(); //释放桶数组及其中各（非空）元素所指向的词条
	int size() const { return N; } // 当前的词条数目
	bool put(K, V); //插入（禁止雷同词条，故可能失败）
	V* get(K k); //读取
	bool remove(K k); //删除
	int primeNLT(int c, int n, char* file);
};
 
 
 
//释放节点值
template <typename T>void releases(T& e) {
	e = NULL;
}


// template <typename K, typename V> void Hashtable<K, V>::rehash() {
//    int oldM = M; Entry<K, V>** oldHt = ht;
//    M = primeNLT( 2 * M, 1048576, "../../_input/prime-1048576-bitmap.txt" ); //容量至少加倍（若懒惰删除很多，可能反而缩容）
//    N = 0; ht = new Entry<K, V>*[M];  memset( ht, 0, sizeof( Entry<K, V>* ) * M ); //桶数组
//    releases ( removed ); removed = new Bitmap( M ); //懒惰删除标记
//    for ( int i = 0; i < oldM; i++ ) //扫描原表
//       if ( oldHt[i] ) //将每个非空桶中的词条
//          put( oldHt[i]->key, oldHt[i]->value ); //转入新表
//    release( oldHt ); //释放——因所有词条均已转移，故只需释放桶数组本身
// }




//构造函数
template <typename K, typename V> Hashtable<K, V>::Hashtable ( int c ) { //创建散列表，容量为
 
	char text[] = "../_input/prime-1048576-bitmap.txt";
	char* ch = text;
 
 
   M = primeNLT ( c, 1048576, ch ); //不小于c的素数M
   N = 0; ht = new Entry<K, V>*[M]; //开辟桶数组（假定成功）
   memset ( ht, 0, sizeof ( Entry<K, V>* ) * M ); //初始化各桶
   removed = new Bitmap ( M ); //用Bitmap记录懒惰删除
   //printf("A bucket array has been created with capacity = %d\n\n", M);
}


//根据file文件中的记录，在[c, n)内取最小的素数
template <typename K, typename V> int Hashtable<K, V>::primeNLT(int c,int n,char* file) {
	Bitmap B(file, n); //file已经按位图格式记录了n以内的所有素数，因此只要
	while (c < n) //从c开始，逐位地
		if (B.test(c)) c++; //测试，即可
		else return c; //返回首个发现的素数
	return c; //若没有这样的素数，返回n（实用中不能如此简化处理）
}


//析构前释放桶数组及非空词条
template <typename K, typename V> Hashtable<K, V>::~Hashtable() {
	for (int i = 0; i < M; i++) //逐一检查各桶
		if (ht[i]) releases(ht[i]); //释放非空的桶
	releases(ht); //释放桶数组
	releases(removed); //释放懒惰删除标记
} //releases()负责释放复杂结构，与算法无直接关系，具体实现详见代码包


//散列表词条查找算法
template <typename K, typename V> V* Hashtable<K, V>::get(K k) 
{
	int r = probe4Hit(k); return ht[r] ? &(ht[r]->value) : NULL;
} //禁止词条的key值雷同


//散列表词条插入
template <typename K, typename V> bool Hashtable<K, V>::put(K k, V v) { 
	if (ht[probe4Hit(k)]) return false; //雷同元素不必重复插入
	int r = probe4Free(k); //为新词条找个空桶（只要装填因子控制得当，必然成功）
	ht[r] = new Entry<K, V>(k, v); ++N; //插入
	if (removed->test(r)) { removed->clear(r); --L; } //懒惰删除标记
	if ((N + L) * 2 > M) rehash(); //若装填因子高于50%，重散列
	return true;
}



//散列表词条删除算法
template <typename K, typename V> bool Hashtable<K, V>::remove(K k) { 
	int r = probe4Hit(k); if (!ht[r]) return false; //确认目标词条确实存在
	releases(ht[r]); ht[r] = NULL; --N; //清除目标词条
	removed->set(r); ++L; //更新标记、计数器
	if (3 * N < L) rehash(); //若懒惰删除标记过多，重散列
	return true;
}




 //重散列：空桶太少时对散列表重新整理：扩容，再将词条逐一移入新表
 //散列函数的定址与表长M直接相关，故不可简单地批量复制原桶数组
template <typename K, typename V> void Hashtable<K, V>::rehash() {
 
	char text[] = "../_input/prime-1048576-bitmap.txt";
	char* PRIME_TABLE = text;
	   int oldM = M; Entry<K, V>** oldHt = ht;
	   M = primeNLT(4 * N, 1048576, PRIME_TABLE); //容量至少加倍（若懒惰删除很多，可能反而缩容）
	   ht = new Entry<K, V>*[M]; N = 0; memset(ht, 0, sizeof(Entry<K, V>*) * M); //桶数组
	   releases(removed); removed = new Bitmap(M); L = 0; //懒惰删除标记
	   for (int i = 0; i < oldM; i++) //扫描原表
	       if (oldHt[i]) //将每个非空桶中的词条
	        put(oldHt[i]->key, oldHt[i]->value); //转入新表
	   releases(oldHt); //释放——因所有词条均已转移，故只需释放桶数组本身
}



//沿关键码k的试探链，找到首个可用空桶；实践中有多种试探策略可选，这里仅以线性试探为例
template <typename K, typename V> int Hashtable<K, V>::probe4Free(const K& k) {
	int r = hashCode(k) % M; //按除余法确定试探链起点
	while (ht[r]) r = (r + 1) % M; //线性试探，直到首个空桶（无论是否带有懒惰删除标记）
	return r; //只要有空桶，线性试探迟早能找到
}
 
 

 //沿关键码k的试探链，找到与之匹配的桶；实践中有多种试探策略可选，这里仅以线性试探为例
template <typename K, typename V> int Hashtable<K, V>::probe4Hit(const K& k) {
	int r = hashCode(k) % M; //按除余法确定试探链起点
	while ((ht[r] && (k != ht[r]->key)) || removed->test(r))
		r = (r + 1) % M; //线性试探（跳过带懒惰删除标记的桶）
	return r; //调用者根据ht[r]是否为空及其内容，即可判断查找是否成功
}











