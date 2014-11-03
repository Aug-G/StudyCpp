


#include <iostream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;


void insertionSort(int *head, int n)
{
	for(int j=1;j < n;j++)
	{
		int key = head[j];
		int i = j - 1;
		while ( i >= 0 && head[i] > key)
		{
			head[i+1] = head[i];
			i --;
		}
		head[i+1] = key;
	}
}

void printList(int *head, int n)
{
	cout << "--------start--------" << endl;

	for(int i=0;i < n;i++)
	{
		cout << head[i] << endl;
	}

	cout << "--------end----------" << endl;
}




int main(int argc, char** argv)
{
	int n = 10;
	int *a = new int[10];
	for(int i = 0; i < n ; i++)
	{
		a[i] = rand()%n +1 ;
	}

	printList(a,n);

	insertionSort(a,n);
	
	printList(a, n);

	cin.get();

	delete[] a;

	return 0;
}



