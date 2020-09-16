/****************************************************************************
@file:test-util.cpp 
@author:ebayboy@163.com 
@date:2020-08-20 16:22 
@version: 1.0  
@description: cpp file 
@Copyright (c)  all right reserved 
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <fstream>
#include <memory>

#include <gtest/gtest.h>

using namespace std;

#include "hpsfilter.hpp"

//#include unit test header

int test_hps_init1()
{
	vector<const char *> exprs {"1000", "1001", "1002"}; 
	vector<unsigned int> ids {1000, 1001, 1002};
	vector<unsigned int> flags {FLAGS_FAST, FLAGS_FAST, FLAGS_LEFTMOST}; 

	string_view data("===1000=======1001======1002====");
	auto filter = make_shared<HPSFilter>(exprs, ids, flags);

	int ret = filter->Init();

	vector<tuple<unsigned int, unsigned long long , unsigned long long >> vec_res;
	int err = filter->Match(data, vec_res);
	if (err != 0) {
		cout << "error match!" << endl;
		return -1;
	}

	cout << "ret:" << ret << endl;

	for ( auto && v : vec_res ) {
		std::cout << "id:" << std::get<0>(v) << " ";
		std::cout << "from:" << std::get<1>(v) << " ";
		std::cout << "to:" << std::get<2>(v) << " " << std::endl;
	}

	return ret;
}

int test_hps_init2()
{

}

int test_hps_dumpdb()
{

}

TEST(HPSFilter, test_hps_init1)
{
	ASSERT_TRUE(test_hps_init1() == 0);
}

