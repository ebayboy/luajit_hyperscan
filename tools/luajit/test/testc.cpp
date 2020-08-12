/****************************************************************************
@file:test_myclib.cpp 
@author:ebayboy@163.com 
@date:2020-08-12 10:40 
@version: 1.0  
@description: cpp file 
@Copyright (c)  all right reserved 
**************************************************************************/

#include <iostream>
#include <string>
#include <numeric>
#include <vector>
#include <algorithm>

using namespace std;

#define DEBUG 

#ifdef __cplusplus
extern "C"
{
#endif
int fast(int *a,int m,int n);

int main(int argc, char **argv)
{
	int a[5] = {3,1,5,4,2};

	cout << "init a:" << endl;
	for(int i =0; i< sizeof(a)/sizeof(int);i++)
	{
		cout << a[i] << " ";
	}
	cout << endl;

	fast(a, 0, 5);

	cout << "sorted:" << endl;
	for(int i =0; i< sizeof(a)/sizeof(int);i++)
	{
		cout << a[i] << " ";
	}

	cout << endl;

	return 0;
}
#ifdef __cplusplus
}
#endif
