#define N_CHAR  (0x80 - 0x20)//���Կɴ�ӡ�ַ�Ϊ��
struct HuffChar { //Huffman(��)�ַ�
	char ch; int weight;//�ַ���Ƶ��
	HuffChar (char c = '^',int w = 0):ch( c ), weight( w ) {};
//�Ƚ������е���(������һ���������в���)
	bool operator<  ( HuffChar const& hc ) { return weight > hc.weight;}//�˴������С�ߵ� 
	bool operator== ( HuffChar const& hc ) { return weight == hc.weight; }
};

#include"../../BinTree/BinTree.h"
#define HuffTree BinTree<HuffChar> //Huffman������BinTree�������ڵ�����ΪHuffChar
#include"../../List/ListNode.h"
#include"../../List/List.h"
typedef List<HuffTree*> HuffForest;
#include "../../Bitmap/Bitmap.h"
typedef Bitmap HuffCode;
#include"../../Hashtable/Hashtable.h"
typedef Hashtable<char, char*> HuffTable;
 
#include "../../List/ListNode.h"
#include "../../List/List.h" //��Listʵ��
//using HuffTree = BinTree<HuffChar>;
//using HuffForest = List<HuffTree*>; //Huffmanɭ��


int* statistics( char* dream ){ //ͳ���ַ�����Ƶ��
	int* freq = new int[N_CHAR]; //����ͳ����������ʣ����������¼���ַ����ִ��� 
	memset ( freq, 0 , sizeof( int ) * N_CHAR ); //����
	FILE* fp = fopen ( dream,"r"); //assert:�ļ������ҿ���ȷ�� 
	for ( char ch; 0 < fscanf ( fp, "%c", &ch );)//���ɨ�������ļ��е�ÿ���ַ�
		if ( ch >= 0x20 ) freq[ch - 0x20]++;//�ۼƶ�Ӧ�ĳ��ִ��� 
	fclose ( fp ); return freq;
}

HuffForest* initForest( int* freq ){//����Ƶ��ͳ�Ʊ�Ϊÿ���ַ�����һ����
	HuffForest* forest = new HuffForest;//��Listʵ�ֵ�Huffmanɭ�� 
	for ( int i = 0; i < N_CHAR; i++ ){ //Ϊÿ���ַ�
		forest -> insertAsLast( new HuffTree );//����һ�����������ַ�����Ƶ��
		forest -> last() -> data -> insertAsRoot ( HuffChar ( 0x20 + i,freq[i]));//��������
	}
	return forest;
}


HuffTree* minHChar( HuffForest* forest ) { //��Huffmanɭ�����ҳ�Ȩ����С��(��)�ַ�
	ListNodePosi( HuffTree* ) p = forest -> first();//���׽ڵ�������� 
	ListNodePosi( HuffTree* ) minChar = p; //��СHuffman�����ڵĽڵ�λ��
	int minWeight = p -> data -> root() -> data.weight;//Ŀǰ����СȨ�� 
	while( forest -> valid( p = p -> succ) )//�������нڵ�
		if( minWeight > p->data->root()->data.weight)//����ǰ�ڵ���������С���� 
			{  minWeight=p->data->root()->data.weight; minChar = p;} //���¼�¼
	return forest -> remove( minChar );//����ѡ����Huffman����ɭ����ժ���������� 
}

HuffTree* generateTree ( HuffForest* forest){ //Huffman�����㷨 
	while( 1 < forest -> size() ) {
		HuffTree* T1 = minHChar ( forest ); HuffTree* T2 = minHChar ( forest ); 
		HuffTree* S = new HuffTree();
		S->insertAsRoot ( HuffChar ( '^', T1->root()->data.weight + T2->root()->data.weight)); 
		S->attachAsLC( S->root(),T1 ); S->attachAsRC( S->root(),T2 ); 
		forest->insertAsLast( S );
	}//assert:ѭ������ʱ��ɭ����Ψһ(�б��׽ڵ���)���ǿ�����Huffman������ 
	return forest->first()->data; 
}


static void //ͨ��������ȡ���ַ��ı���
generateCT ( Bitmap* code, int length, HuffTable* table, BinNodePosi ( HuffChar )v ){
	if( IsLeaf (*v) ) //����Ҷ�ڵ�(���ж��ַ��������ж�)
		{ table->put (v->data.ch, code->bits2string( length ) ); return; } 
	if( HasLChild(*v)) //Left = 0
		{code->clear(length ); generateCT( code, length + 1, table, v->lc ); } 
	if(HasRChild(*v)) //Right=1
		{ code->set( length ); generateCT ( code, length + 1, table, v->rc ); }
}

HuffTable* generateTable( HuffTree* tree ){//�����ַ�����ͳһ������ɢ�б�ʵ�ֵı������
	HuffTable* table = new HuffTable; Bitmap* code = new Bitmap;
	generateCT( code, 0 , table, tree->root() ); releases( code ); return table;
};



int encode ( HuffTable* table, Bitmap* codeString, char*s){ //���ձ�����B1tmap������
	int n = 0;//�����صı��봮�ܳ�n
	for ( size_t m = strlen ( s ), i = 0;i < m; i++ ){ //���������е�ÿ���ַ�
		char** pCharCode = table->get( s[i] );//ȡ�����Ӧ�ı��봮
		if(!pCharCode) pCharCode=table->get( s[i] +'A'-'a');//Сд��ĸתΪ��д 
		if(!pCharCode) pCharCode=table->get( ' ' ); //�޷�ʶ����ַ�ͳһ�����ո� 
		printf ("%s", *pCharCode );//�����ǰ�ַ��ı���
		for(size_t m=strlen( *pCharCode ), j = 0; j <m;j++ )//����ǰ�ַ��ı��������봮
			'1' == *( *pCharCode + j) ? codeString->set(n++): codeString->clear(n++);
	}
	printf ("\n"); return n;
}//�����Ʊ��봮��¼��λͼcodeString��



void decode ( HuffTree* tree, Bitmap* code, int n){ //���ݱ������Գ�Ϊn��Bitmap������
	BinNodePosi ( HuffChar ) x = tree->root(); 
	for ( int i = 0; i <n; i++ ) {
		x = code->test(i) ? x->rc :x->lc;
		if(IsLeaf ( *x ) ) { printf ( "%c", x->data.ch );x = tree->root(); } 
	}
}//��������룬�ڴ�ֱ�Ӵ�ӡ���;ʵ���пɸ�Ϊ������Ҫ�����ϲ������


int main(int argc, char* argv[]) { //Huffman�����㷨ͳһ����
    char text[] = "dream";
    char* ch = text;
    char s[] = "dream.txt";
   //������ļ�����ϸ�뿴ע��
 
 
    int* freq = statistics(s); //���������ļ���ͳ�Ƹ��ַ��ĳ���Ƶ��
    HuffForest* forest = initForest(freq); //����Huffmanɭ��
    releases(freq); 
    HuffTree* tree = generateTree(forest);  //����Huffman������
    releases(forest);
    //print(tree); //���������
    HuffTable* table = generateTable(tree); //��Huffman������ת��Ϊ�����
    for (int i = 0; i < N_CHAR; i++) //��������
        printf(" %c: %s\n", i + 0x20, *(table->get(i + 0x20)));
    
    for (int i = 2; i < 3; i++) { //���������д����ÿһ���Ĵ�
        printf("\nEncoding: %s\n", ch); //���²��Ա���
        Bitmap* codeString = new Bitmap; //�����Ʊ��봮
        int n = encode(table, codeString, text); //�����ݱ�������ɣ�����Ϊn��
        releases(codeString);
    }
    system("pause");
    releases(table); 
    releases(tree);
    return 0; //�ͷű����������
} 
