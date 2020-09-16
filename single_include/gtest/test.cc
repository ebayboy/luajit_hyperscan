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

#include <gtest/gtest.h>

using namespace std;

#include "hpsfilter.hpp"

//#include unit test header

int test_hps_init1()
{
	vector<const char *> exprs {"1000", "1001", "1002"}; 
	vector<unsigned int> ids {1000, 1001, 1002};
	vector<unsigned int> flags {FLAGS_FAST, FLAGS_FAST, FLAGS_LEFTMOST}; 

}

int test_hps_init2()
{

}

int test_hps_dumpdb()
{

}

#if 0
TEST(protobuf, test_write)
{
	ASSERT_TRUE(test_write() == 0);
}
#endif

