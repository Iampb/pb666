#include<iostream>
#include<time.h>
#include<ctime>
#include<stdlib.h>
using namespace std;
#define MAX 20000
int a[MAX];

//ð������
void bubble(int a[], int size)
{
	int i, temp, work;
	for (int pass = 0; pass < size; pass++)
	{
		work = 1;
		for (i = 0; i < size - pass; i++)

			if (a[i] > a[i + 1])
			{
				temp = a[i];
				a[i] = a[i + 1];
				a[i + 1] = temp;
				work = 0;
			}
		if (work) break;

	}
}


	//��������
void insert_sort(int a[], int length)
{
	int t;
	for (int i = 1; i < length; i++)
	{
		for (int j = i ; j >= 0 && a[j + 1] < a[j]; j--)
		{
			t = a[j];
			a[j] = a[j + 1];
			a[j + 1] = t;
		}
	}
}


//ѡ������
void selection_sort(int a[],int N)
{
	int i, j, t, k;
	for (i = 0; i < N - 1; i++)
	{
		k = a[i];
		for (j = i + 1; j < N; j++)
			if (a[k] > a[j])
				k = a[j];
		if (k != i)
		{
			t = a[i]; 
			a[i] = a[k];
			a[k] = t;
		}
	}
}



//�鲢���� 
void merge(int arr[], int L, int M, int R)
{
	int Left_Size = M - L;
	int Right_Size = R - M + 1;
	int left[MAX];
	int right[MAX];
	int i, j, k;

	for (i = L; i < M; i++)
	{
		left[i - L] = arr[i];
	}
	for (i = M; i <= R; i++)
	{
		right[i - M] = arr[i];
	}

	i = 0;
	j = 0;
	k = L;
	while (i < Left_Size && j < Right_Size)
	{
		if (left[i] < right[j])
		{
			arr[k] = left[i];
			i++;
			k++;
		}
		else
		{
			arr[k] = right[j];
			j++;
			k++;
		}
	}
	while (i < Left_Size)
	{
		arr[k] = left[i];
		i++;
		k++;
	}
	while (j < Right_Size)
	{
		arr[k] = right[j];
		j++;
		k++;
	}
}

void mergeSort(int arr[], int L, int R)
{
	if (L == R)
		return;
	else
	{
		int M = (L + R) / 2;
		mergeSort(arr, L, M);
		mergeSort(arr, M + 1, R);
		merge(arr, L, M + 1, R);
	}
}



//��������
void quick_sort(int array[], int low, int high) {
    int i = low; 
    int j = high;
    if(i >= j) {
        return;
    }
 
    int temp = array[low];
    while(i != j) {
        while(array[j] >= temp && i < j) {
            j--;
        }
	while(array[i] <= temp && i < j) {
            i++;
        }
	if(i < j) {
            swap(array[i], array[j]);
        }
    }
 
    //����׼temp�����Լ���λ�ã�����i��λ�ã�
    swap(array[low], array[i]);
    quick_sort(array, low, i - 1);
    quick_sort(array, i + 1, high);
}

 
	
int main()
{
	//����� 
	srand((int)time(0));
	for (int i = 0; i < MAX; i++)
		a[i] = rand () % 100;

	//���� 
	clock_t startTime, endTime;
	startTime = clock();
	bubble(a, MAX);
	endTime = clock();
	cout << "���������Ϊ: " << MAX << endl;
	cout << "����ð���������õ�ʱ��Ϊ: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	
	//˳�� 
	startTime = clock();
	bubble(a, MAX);
	endTime = clock();
	cout << "˳��ð���������õ�ʱ��Ϊ: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	
	//���� 
	int b[MAX];
	for (int i = MAX; i >= 0; i--) {
		b[MAX-i] = a[i];
	}
	startTime = clock();
	bubble(b, MAX);
	endTime = clock();
	cout << "����ð���������õ�ʱ��Ϊ: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	cout <<endl; 
	
	//���� 
	int c[MAX]; 
	srand((int)time(0));
	for (int i = 0; i < MAX; i++)
		c[i] = rand() % 100;

	//���� 
	startTime = clock();
	insert_sort(c, MAX);
	endTime = clock();
	cout << "��������������õ�ʱ��Ϊ: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	
	//˳�� 
	startTime = clock();
	insert_sort(c, MAX);
	endTime = clock();
	cout << "˳������������õ�ʱ��Ϊ: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	
	//���� 
	int d[MAX];
	for (int i = MAX; i >= 0; i--) {
		d[MAX-i] = c[i];
	}
	startTime = clock();
	insert_sort(d, MAX);
	endTime = clock();
	cout << "��������������õ�ʱ��Ϊ: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	cout <<endl; 
	
	//ѡ�� 
	int e[MAX]; 
	srand((int)time(0));
	for (int i = 0; i < MAX; i++)
		e[i] = rand() % 100;

	//���� 
	startTime = clock();
	selection_sort(e, MAX);
	endTime = clock();
	cout << "����ѡ���������õ�ʱ��Ϊ: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl; 
	
	//˳��
	startTime = clock();
	selection_sort(e, MAX);
	endTime = clock();
	cout << "˳��ѡ���������õ�ʱ��Ϊ: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	
	//����  
	int f[MAX];
	for (int i = MAX; i >= 0; i--) {
		f[MAX-i] = e[i];
	}
	startTime = clock();
	selection_sort(f, MAX);
	endTime = clock();
	cout << "��������������õ�ʱ��Ϊ: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	cout <<endl; 
	
	//�鲢 
	int g[MAX]; 
	srand((int)time(0));
	for (int i = 0; i < MAX; i++)
		g[i] = rand() % 100;

	//���� 
	startTime = clock();
	mergeSort(g, 0, MAX-1);
	endTime = clock();
	cout << "����鲢�������õ�ʱ��Ϊ: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	
	
	
	//˳��
	startTime = clock();
	mergeSort(g, 0, MAX-1);
	endTime = clock();
	cout << "˳��鲢�������õ�ʱ��Ϊ: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	
	//���� 
	int h[MAX];
	for (int i = MAX; i >= 0; i--) {
		h[MAX-i] = g[i];
	}
	startTime = clock();
	mergeSort(h, 0,MAX-1);
	endTime = clock();
	cout << "����鲢�������õ�ʱ��Ϊ: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	cout <<endl; 
	
	//���� 
	int j[MAX]; 
	srand((int)time(0));
	for (int i = 0; i < MAX; i++)
		j[i] = rand() % 100;

	//���� 
	startTime = clock();
	quick_sort(j, 0, MAX);
	endTime = clock();
	cout << "��������������õ�ʱ��Ϊ: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	
	
	//˳��
	startTime = clock();
	quick_sort(j, 0, MAX);
	endTime = clock();
	cout << "˳������������õ�ʱ��Ϊ: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	
	//����
	int k[MAX];
	for (int i = MAX; i >= 0; i--) {
		k[MAX-i] = j[i];
	}
	startTime = clock();
	quick_sort(k, 0, MAX);
	endTime = clock();
	cout << "��������������õ�ʱ��Ϊ: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	cout <<endl;  
}

		
		
		
		
		
		
		
		
		
		
		
		

