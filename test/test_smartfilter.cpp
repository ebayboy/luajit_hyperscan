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
	vector<const char *>patterns;
	vector<unsigned int>flags;
	vector<unsigned int>ids;

	patterns.push_back("1001");

	//HS_FLAG_DOTALL 
	flags.push_back(2);
	ids.push_back(100);

	void *f = filter_new("@url", patterns.data(),flags.data(), ids.data(), patterns.size());
	if (f == NULL) {
		cout << "Error: filter_new!" << endl;
		return -1;
	}

	string str = "abcde1001asfasf";
	int ret = filter_match(f, str.data(), str.size());
	cout << "filter_match ret:" << ret << endl;


	filter_free(f);

	return 0;
}

