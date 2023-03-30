# include"complex.h"
# include"../../vector/vector.h"
# include<iostream>
using namespace std;
	Complex A[10];
int main(){
	cout<<"生成无序随机向量";
	for( int i = 0; i < 10; i++ ){
		A[i].read_Real(rand() % 20 + 10);
		A[i].read_Imaginary(rand() % 20 + 10);
		A[i].read_modulus();
	}
	cout<<endl;
	Vector<Complex> B(A,10);
	for( int i = 0; i < 10; i++){
		B._elem[i].display();
	}
	cout<<endl;
	B.mergeSort(0,10);
	cout<<"排序后"<<endl;
	for( int i = 0; i < 10; i++)
		B._elem[i].display();
	cout<<endl;
	
	//区间查找 
	double C[10];
	for(int i = 0; i < 10; i++){
		C[i] = B._elem[i].display2();
	}
	Vector<Complex> D(A,10);
	for( int i = 0; i < 10; i++ ){
		D[i].read_Real(C[i]);
		D[i].read_Imaginary(0);
		D[i].read_modulus();
	}
	int sum = D.find2(10,30,0,10);
	cout<<"个数为："<<sum<<endl; 
	for(int i = 0; i < sum; i++)
		B._elem[i].display();
		
		
		
}
