#ifndef BITMAP
#define BITMAP 
#include <cstring>
#include <iostream>
using namespace std;

class Bitmap{ //位图Bitmap类
private:
	char*M;
	int N, _sz;//位图空间M[]，N*sizeof(char)*8个比特中含_sz个有效位
protected:
	void init( int n )
	{M = new char[N = (n+7)/8]; memset(M,0,N);_sz = 0;}
public:
	Bitmap( int n = 8 ) {init ( n ) ; }//按指定容量创建位图（为测试暂时选用较小的默认值）
	Bitmap( char* file, int n = 8){//按指定或默认规模，从指定文件中读取位图
		init( n );
		FILE* fp = fopen(file,"r"); fread( M, sizeof(char),N,fp); fclose( fp ); 
		for(int k = 0, _sz = 0; k < n; k++)_sz+=test(k);
	}
	~Bitmap(){delete[] M; M = NULL; _sz = 0;}//析构时释放位图空间
	int size(){ return _sz ;} 
	void set( int k){expand(k); _sz++; M[k>>3]|=(0x80>>(k&0x07));}
	void clear( int k){expand(k);_sz--; M[k>>3] &=~(0x80>>(k&0x07));}
	bool test(int k){expand(k); return M[k>>3]&(0x80>>(k&0x07));}
	void dump(char*file)//将位图整体导出至指定的文件，以便对此后的新位图批量初始化
	{FILE*fp = fopen( file,"w");fwrite(M,sizeof(char),N,fp);fclose(fp);}
	char*bits2string(int n){//将前n位转换为字符串——
		expand(n-1);//此时可能被访问的最高位为bitmap[n-1]
		char*s = new char[n+1]; s[n]='\0';//字符串所占空间，由上层调用者负责释放
		for(int i = 0; i < n; i++) s[i] = test(i)?'1':'0';
		return s;//返回字符串位置
	}
	void expand(int k){//若被访问的Bitmap[k]已出界，则需扩容
		if(k < 8*N) return;//仍在界内，无需扩容
		int oldN = N ; char* oldM = M; 
		init (2*k);//与向量类似，加倍策略
		strcpy ( oldM, M );
		delete[] oldM;//原数据转移至新空间
	}
};
#endif
