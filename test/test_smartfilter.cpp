/****************************************************************************
@file:testfilter.cpp 
@author:ebayboy@163.com 
@date:2020-08-17 10:42 
@version: 1.0  
@description: cpp file 
@Copyright (c)  all right reserved 
**************************************************************************/

#include <iostream>
#include <string>
#include <numeric>
#include <vector>
#include <algorithm>

#include <vector>
#include <string>

#include "smartfilter.h"

using namespace std;

#define DEBUG 

int main(int argc, char **argv)
{
	hello();

	int a[5]= {1,2,3,4,5};
	printArray(a, 5);

	const char *stra[4];

	char *str1= (char *)"my";
	char *str2= (char *)"name";
	char *str3= (char *)"is";
	char *str4= (char *)"fanpf";

	stra[0] = str1;
	stra[1] = str2;
	stra[2] = str3;
	stra[3] = str4;

	printStrArray(stra, 4);

	vector<const char *>patterns;
	vector<unsigned int>flags;
	vector<unsigned int>ids;

	patterns.push_back("1001");
	patterns.push_back("1002");

	//HS_FLAG_DOTALL 
	flags.push_back(RULES_HS_FLAGS);
	flags.push_back(RULES_HS_FLAGS_LEFTMOST);

	ids.push_back(100);
	ids.push_back(101);

	void *f = filter_new("@url", patterns.data(),flags.data(), ids.data(), patterns.size());
	if (f == NULL) {
		cout << "Error: filter_new!" << endl;
		return -1;
	}

	string str = "abcde1001as1002fasf";
	int ret = filter_match(f, str.data(), str.size());
	cout << "filter_match ret:" << ret << endl;

	filter_delete(f);

	return 0;
}

