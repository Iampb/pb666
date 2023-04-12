#include "../../Queue/Queue.h"
#include <iostream>
#include <stdlib.h>
using namespace std;

queue A; 
 
void cul(int* height,int n)
{
	int maxarea = 0;
	height[n+1]=-1;
    for(int i=1; i <= n+1; i++)
    {
        if(A.empty() || height[ A.getlast() ] <= height[i]) {A.push(i);}
        else
        {   int suc;
            while(A.getLength() && (height[i] < height[A.getlast()] ) ) //计算 
            {
                suc = A.getlast(); 
                maxarea = max(height[A.getlast()]*(i-A.getlast()), maxarea);
                A.out();
            }
        A.push(suc);
        height[suc] = height[i];
        }
    }
    cout<<endl<<"最大矩形面积为"<<maxarea<<endl;
}
int main()
{
	const int N = 105;
	int height[1000];
	for(int j = 1; j<=N; j++)
    	{height[j] = rand()%100+1;
    	cout<<height[j]<<" ";}
    cul(height, N);
    
	int n;
	cout<<endl<<"接下来可以手动输入"<<endl; 
	cout<<"手动输入矩形个数:"<<endl;
	cin>>n;
	cout<<"手动输入各矩形高度:"<<endl;
	for(int i=1;i<=n;i++)
	{
		cin>>height[i];
	}
		cul(height,n);
}
