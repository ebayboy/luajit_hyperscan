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
	cout << "========Test init with DB================" << endl;
	// read db 
	ifstream is{"./dumpdb.dat", std::ios::binary | std::ios::ate};
	if (is.bad()) {
		cerr << "Error open file!" << endl;
		return -1;
	}

	auto size = is.tellg();
	std::string db(size, '\0'); // construct string to stream size

	cout << "size:" << size << endl;

	is.seekg(0);
	if(is.read(&db[0], size))
		std::cout << "db:" << db << '\n';
	is.close();


	// init
	vector<const char *> exprs {"1000", "1001", "1002"}; 
	vector<unsigned int> ids {1000, 1001, 1002};
	vector<unsigned int> flags {FLAGS_FAST, FLAGS_FAST, FLAGS_LEFTMOST}; 

	string_view data("===1000=======1001======1002====");
	auto filter = make_shared<HPSFilter>(exprs, ids, flags);

	int ret = filter->Init(db.data(), size);
	if (ret != 0) {
		std::cerr << "Error: init " << std::endl;
	}

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

	return 0;
}

int test_hps_dumpdb()
{
	vector<const char *> exprs {"1000", "1001", "1002"}; 
	vector<unsigned int> ids {1000, 1001, 1002};
	vector<unsigned int> flags {FLAGS_FAST, FLAGS_FAST, FLAGS_LEFTMOST}; 

	auto filter = make_shared<HPSFilter>(exprs, ids, flags);
	int ret = filter->Init();

	size_t length = 0;
	std::shared_ptr<char> sp_bytes;

	ret = filter->DumpDB(sp_bytes, length);
	if (ret != 0) {
		cerr << "error:ret:%d" << ret << endl;
		return -1;
	}

	cout << "db:" << std::addressof(sp_bytes) << endl;
	cout << "length:" << length << endl;

	ofstream of("./dumpdb.dat", ios::binary);
	if (of.bad()) {
		cerr << "Error open file!" << endl;
		return -1;
	}

#if 1
	of.write(sp_bytes.get(), length);
#else
	of.write("hello", 5);
#endif

	of.close();

	return 0;
}

TEST(HPSFilter, test_hps_init1)
{
	//test init with exprs
	ASSERT_TRUE(test_hps_init1() == 0);

	//test dump db 
	ASSERT_TRUE(test_hps_dumpdb() == 0);

	//test init with db
	ASSERT_TRUE(test_hps_init2() == 0);
}

