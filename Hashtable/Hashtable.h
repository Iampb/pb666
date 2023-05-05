#include "../Dictionary/Dictionary.h"
#include "../Entry/Entry.h"
#include "../Bitmap/Bitmap.h"
#include<cstring>
#include<iostream>
using namespace std;
 
static size_t hashCode(char c) { return (size_t)c; } //�ַ�
static size_t hashCode(int k) { return (size_t)k; } //�����Լ���������
static size_t hashCode(long long i) { return (size_t)((i >> 32) + (int)i); }
static size_t hashCode(char s[]) { //�����ַ�����ѭ����λɢ���루cyclic shift hash code��
	int h = 0; //ɢ����
	for (size_t n = strlen(s), i = 0; i < n; i++) //�������ң��������ÿһ�ַ�
	{
		h = (h << 5) | (h >> 27); h += (int)s[i];
	} //ɢ����ѭ������5λ�����ۼӵ�ǰ�ַ�
	return (size_t)h; //������õ�ɢ���룬ʵ���Ͽ����Ϊ���Ƶġ�����ʽɢ���롱
} //����Ӣ�ﵥ�ʣ�"ѭ������5λ"��ʵ��ͳ�Ƶó������ֵ
 
//����Dictionary�ӿڵ�Hashtableģ����

template <typename K, typename V> class Hashtable : public Dictionary<K, V> { //key��value
	friend class UniPrint;
private:
	Entry <K, V>** ht; //Ͱ���飬��Ŵ���ָ��
	int M, N, L; //Ͱ����������������Ŀ������ɾ����ǵ���Ŀ��N + L <= M��
	Bitmap* removed; //����ɾ�����
protected:
	int probe4Hit(const K& k); //�عؼ���k��Ӧ����̽�����ҵ�����ƥ���Ͱ
	int probe4Free(const K& k); //�عؼ���k��Ӧ����̽�����ҵ��׸����ÿ�Ͱ
	void rehash(); //��ɢ���㷨������Ͱ���飬��֤װ�������ھ���������
public:
	Hashtable(int c = 5); //����һ��������С��c��ɢ�б�Ϊ������ʱѡ�ý�С��Ĭ��ֵ��
	~Hashtable(); //�ͷ�Ͱ���鼰���и����ǿգ�Ԫ����ָ��Ĵ���
	int size() const { return N; } // ��ǰ�Ĵ�����Ŀ
	bool put(K, V); //���루��ֹ��ͬ�������ʿ���ʧ�ܣ�
	V* get(K k); //��ȡ
	bool remove(K k); //ɾ��
	int primeNLT(int c, int n, char* file);
};
 
 
 
//�ͷŽڵ�ֵ
template <typename T>void releases(T& e) {
	e = NULL;
}


// template <typename K, typename V> void Hashtable<K, V>::rehash() {
//    int oldM = M; Entry<K, V>** oldHt = ht;
//    M = primeNLT( 2 * M, 1048576, "../../_input/prime-1048576-bitmap.txt" ); //�������ټӱ���������ɾ���ܶ࣬���ܷ������ݣ�
//    N = 0; ht = new Entry<K, V>*[M];  memset( ht, 0, sizeof( Entry<K, V>* ) * M ); //Ͱ����
//    releases ( removed ); removed = new Bitmap( M ); //����ɾ�����
//    for ( int i = 0; i < oldM; i++ ) //ɨ��ԭ��
//       if ( oldHt[i] ) //��ÿ���ǿ�Ͱ�еĴ���
//          put( oldHt[i]->key, oldHt[i]->value ); //ת���±�
//    release( oldHt ); //�ͷš��������д�������ת�ƣ���ֻ���ͷ�Ͱ���鱾��
// }




//���캯��
template <typename K, typename V> Hashtable<K, V>::Hashtable ( int c ) { //����ɢ�б�����Ϊ
 
	char text[] = "../_input/prime-1048576-bitmap.txt";
	char* ch = text;
 
 
   M = primeNLT ( c, 1048576, ch ); //��С��c������M
   N = 0; ht = new Entry<K, V>*[M]; //����Ͱ���飨�ٶ��ɹ���
   memset ( ht, 0, sizeof ( Entry<K, V>* ) * M ); //��ʼ����Ͱ
   removed = new Bitmap ( M ); //��Bitmap��¼����ɾ��
   //printf("A bucket array has been created with capacity = %d\n\n", M);
}


//����file�ļ��еļ�¼����[c, n)��ȡ��С������
template <typename K, typename V> int Hashtable<K, V>::primeNLT(int c,int n,char* file) {
	Bitmap B(file, n); //file�Ѿ���λͼ��ʽ��¼��n���ڵ��������������ֻҪ
	while (c < n) //��c��ʼ����λ��
		if (B.test(c)) c++; //���ԣ�����
		else return c; //�����׸����ֵ�����
	return c; //��û������������������n��ʵ���в�����˼򻯴���
}


//����ǰ�ͷ�Ͱ���鼰�ǿմ���
template <typename K, typename V> Hashtable<K, V>::~Hashtable() {
	for (int i = 0; i < M; i++) //��һ����Ͱ
		if (ht[i]) releases(ht[i]); //�ͷŷǿյ�Ͱ
	releases(ht); //�ͷ�Ͱ����
	releases(removed); //�ͷ�����ɾ�����
} //releases()�����ͷŸ��ӽṹ�����㷨��ֱ�ӹ�ϵ������ʵ����������


//ɢ�б���������㷨
template <typename K, typename V> V* Hashtable<K, V>::get(K k) 
{
	int r = probe4Hit(k); return ht[r] ? &(ht[r]->value) : NULL;
} //��ֹ������keyֵ��ͬ


//ɢ�б��������
template <typename K, typename V> bool Hashtable<K, V>::put(K k, V v) { 
	if (ht[probe4Hit(k)]) return false; //��ͬԪ�ز����ظ�����
	int r = probe4Free(k); //Ϊ�´����Ҹ���Ͱ��ֻҪװ�����ӿ��Ƶõ�����Ȼ�ɹ���
	ht[r] = new Entry<K, V>(k, v); ++N; //����
	if (removed->test(r)) { removed->clear(r); --L; } //����ɾ�����
	if ((N + L) * 2 > M) rehash(); //��װ�����Ӹ���50%����ɢ��
	return true;
}



//ɢ�б����ɾ���㷨
template <typename K, typename V> bool Hashtable<K, V>::remove(K k) { 
	int r = probe4Hit(k); if (!ht[r]) return false; //ȷ��Ŀ�����ȷʵ����
	releases(ht[r]); ht[r] = NULL; --N; //���Ŀ�����
	removed->set(r); ++L; //���±�ǡ�������
	if (3 * N < L) rehash(); //������ɾ����ǹ��࣬��ɢ��
	return true;
}




 //��ɢ�У���Ͱ̫��ʱ��ɢ�б������������ݣ��ٽ�������һ�����±�
 //ɢ�к����Ķ�ַ���Mֱ����أ��ʲ��ɼ򵥵���������ԭͰ����
template <typename K, typename V> void Hashtable<K, V>::rehash() {
 
	char text[] = "../_input/prime-1048576-bitmap.txt";
	char* PRIME_TABLE = text;
	   int oldM = M; Entry<K, V>** oldHt = ht;
	   M = primeNLT(4 * N, 1048576, PRIME_TABLE); //�������ټӱ���������ɾ���ܶ࣬���ܷ������ݣ�
	   ht = new Entry<K, V>*[M]; N = 0; memset(ht, 0, sizeof(Entry<K, V>*) * M); //Ͱ����
	   releases(removed); removed = new Bitmap(M); L = 0; //����ɾ�����
	   for (int i = 0; i < oldM; i++) //ɨ��ԭ��
	       if (oldHt[i]) //��ÿ���ǿ�Ͱ�еĴ���
	        put(oldHt[i]->key, oldHt[i]->value); //ת���±�
	   releases(oldHt); //�ͷš��������д�������ת�ƣ���ֻ���ͷ�Ͱ���鱾��
}



//�عؼ���k����̽�����ҵ��׸����ÿ�Ͱ��ʵ�����ж�����̽���Կ�ѡ���������������̽Ϊ��
template <typename K, typename V> int Hashtable<K, V>::probe4Free(const K& k) {
	int r = hashCode(k) % M; //�����෨ȷ����̽�����
	while (ht[r]) r = (r + 1) % M; //������̽��ֱ���׸���Ͱ�������Ƿ��������ɾ����ǣ�
	return r; //ֻҪ�п�Ͱ��������̽�������ҵ�
}
 
 

 //�عؼ���k����̽�����ҵ���֮ƥ���Ͱ��ʵ�����ж�����̽���Կ�ѡ���������������̽Ϊ��
template <typename K, typename V> int Hashtable<K, V>::probe4Hit(const K& k) {
	int r = hashCode(k) % M; //�����෨ȷ����̽�����
	while ((ht[r] && (k != ht[r]->key)) || removed->test(r))
		r = (r + 1) % M; //������̽������������ɾ����ǵ�Ͱ��
	return r; //�����߸���ht[r]�Ƿ�Ϊ�ռ������ݣ������жϲ����Ƿ�ɹ�
}











