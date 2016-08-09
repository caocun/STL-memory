#include<iostream>
using namespace std;
#include <typeinfo>
#include"mylist.h"
#include"myiterator.h"
#include<vector>

int main()
{
	int arr[] = {12, 23, 34, 45, 56, 78};
	int len = sizeof(arr)/sizeof(arr[0]);
	caocun::list<int> mylist;
	mylist.push_back(12);
	return 0;
}
