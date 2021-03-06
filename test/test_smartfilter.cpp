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

int test_c()
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

	size_t count = 100000;
	double cost = 0;

	for (size_t k = 0; k < 10; k++ ) {
		for (size_t j = 0; j <count; j++) {
			clock_t end, start = clock();
			results_t *rset = filter_match(f, str.data(), str.size());
			end = clock();
			double use = end - start;
			cost = cost + use;
			filter_results_delete(rset);
		}
	}
	double avg = cost / 10;
	cout << "avg:" << avg / CLOCKS_PER_SEC << "s" << endl;

	filter_delete(f);

	return 0;
}

int test_single_cnt()
{
	vector<const char *>patterns;
	vector<unsigned int>flags;
	vector<unsigned int>ids;

	patterns.push_back( "this is"  );
	patterns.push_back( "pattern"  );
	patterns.push_back( "google!"  );
	patterns.push_back( "anber"  );


	//HS_FLAG_DOTALL 
	//unsigned int flag  = RULES_HS_FLAGS_LEFTMOST;
	unsigned int flag  = RULES_HS_FLAGS;
	flags.push_back(flag);
	flags.push_back(flag);
	flags.push_back(flag);
	flags.push_back(flag);

	ids.push_back(1000);
	ids.push_back(1001);
	ids.push_back(1002);
	ids.push_back(1003);

	void *f = filter_new("TestPerformance", patterns.data(),flags.data(), ids.data(), patterns.size());
	if (f == NULL) {
		cout << "Error: filter_new!" << endl;
		return -1;
	}

	std::string str = "This is some text I made up.  This will be testing\n" 
		"multi-pattern matching from Wu/Manber's paper called\n"
		"'A Fast Algorithm for Multi-Pattern Searching'. Manber is\n";

	int cnt= filter_match_cnt(f, str.data(), str.size());
	cout << "hit: "  << cnt << endl;

	filter_delete(f);

	return 0;
}

int test_single()
{
	vector<const char *>patterns;
	vector<unsigned int>flags;
	vector<unsigned int>ids;

	patterns.push_back( "this is"  );
	patterns.push_back( "pattern"  );
	patterns.push_back( "google!"  );
	patterns.push_back( "anber"  );


	//HS_FLAG_DOTALL 
	//unsigned int flag  = RULES_HS_FLAGS_LEFTMOST;
	unsigned int flag  = RULES_HS_FLAGS;
	flags.push_back(flag);
	flags.push_back(flag);
	flags.push_back(flag);
	flags.push_back(flag);

	ids.push_back(1000);
	ids.push_back(1001);
	ids.push_back(1002);
	ids.push_back(1003);

	void *f = filter_new("TestPerformance", patterns.data(),flags.data(), ids.data(), patterns.size());
	if (f == NULL) {
		cout << "Error: filter_new!" << endl;
		return -1;
	}

	std::string str = "This is some text I made up.  This will be testing\n" 
		"multi-pattern matching from Wu/Manber's paper called\n"
		"'A Fast Algorithm for Multi-Pattern Searching'. Manber is\n";

	results_t *rset = filter_match(f, str.data(), str.size());
	cout << "hit: "  << rset->cursor << endl;

	for (size_t i = 0; i < rset->cursor; i++) {
		result_t *r = &rset->results[i];
		printf("id:%u from:%llu to:%llu\n", r->id, r->from, r->to);
	}

	filter_results_delete(rset);

	filter_delete(f);

	return 0;
}

int test_perf_kmp_cnt()
{
	vector<const char *>patterns;
	vector<unsigned int>flags;
	vector<unsigned int>ids;

	patterns.push_back( "this is"  );
	patterns.push_back( "pattern"  );
	patterns.push_back( "google!"  );
	patterns.push_back( "anber"  );

	//HS_FLAG_DOTALL 
	flags.push_back(RULES_HS_FLAGS);
	flags.push_back(RULES_HS_FLAGS);
	flags.push_back(RULES_HS_FLAGS);
	flags.push_back(RULES_HS_FLAGS);

	ids.push_back(1000);
	ids.push_back(1001);
	ids.push_back(1002);
	ids.push_back(1003);

	void *f = filter_new("TestPerformance", patterns.data(),flags.data(), ids.data(), patterns.size());
	if (f == NULL) {
		cout << "Error: filter_new!" << endl;
		return -1;
	}

	std::string str = "This is some text I made up.  This will be testing\n" 
		"multi-pattern matching from Wu/Manber's paper called\n"
		"'A Fast Algorithm for Multi-Pattern Searching'. Manber is\n";

	size_t count = 10000000;
	double cost = 0;

	for (size_t k = 0; k < 3; k++ ) {

		int cnt  = 0;
		clock_t end, start = clock();
		for (size_t j = 0; j <count; j++) {
			cnt = filter_match_cnt(f, str.data(), str.size());
		}

		end = clock();
		double use = end - start;
		cost = cost + use;
		cout << "cost: " << use/CLOCKS_PER_SEC << "s" <<  " cnt:" << cnt << endl;
	}

	double avg = cost /3;

	cout << "avg:" << avg / CLOCKS_PER_SEC << "s" << endl;
	filter_delete(f);

	return 0;
}


int test_perf_kmp()
{
	vector<const char *>patterns;
	vector<unsigned int>flags;
	vector<unsigned int>ids;

	patterns.push_back( "pattern"  );


	//HS_FLAG_DOTALL 
	flags.push_back(RULES_HS_FLAGS);

	ids.push_back(1000);

	void *f = filter_new("TestPerformance", patterns.data(),flags.data(), ids.data(), patterns.size());
	if (f == NULL) {
		cout << "Error: filter_new!" << endl;
		return -1;
	}

	std::string str = "This is some text I made up.  This will be testing\n" 
		"multi-pattern matching from Wu/Manber's paper called\n"
		"'A Fast Algorithm for Multi-Pattern Searching'. Manber is\n";

	size_t count = 10000000;
	double cost = 0;

	for (size_t k = 0; k < 10; k++ ) {
		clock_t end, start = clock();
		for (size_t j = 0; j <count; j++) {
			results_t *rset = filter_match(f, str.data(), str.size());
			filter_results_delete(rset);
		}

		end = clock();
		double use = end - start;
		cost = cost + use;
		cout << "cost: " << use/CLOCKS_PER_SEC << "s" << endl;
	}

	double avg = cost /10;

	cout << "avg:" << avg / CLOCKS_PER_SEC << "s" << endl;
	filter_delete(f);

	return 0;
}


//compare with wm , hyperscan performance  > WuManber
int test_perf_wm()
{
	vector<const char *>patterns;
	vector<unsigned int>flags;
	vector<unsigned int>ids;

	patterns.push_back( "this is"  );
	patterns.push_back( "pattern"  );
	patterns.push_back( "google!"  );
	patterns.push_back( "anber"  );

	//HS_FLAG_DOTALL 
	flags.push_back(RULES_HS_FLAGS);
	flags.push_back(RULES_HS_FLAGS);
	flags.push_back(RULES_HS_FLAGS);
	flags.push_back(RULES_HS_FLAGS);

	ids.push_back(1000);
	ids.push_back(1001);
	ids.push_back(1002);
	ids.push_back(1003);

	void *f = filter_new("TestPerformance", patterns.data(),flags.data(), ids.data(), patterns.size());
	if (f == NULL) {
		cout << "Error: filter_new!" << endl;
		return -1;
	}

	std::string str = "This is some text I made up.  This will be testing\n" 
		"multi-pattern matching from Wu/Manber's paper called\n"
		"'A Fast Algorithm for Multi-Pattern Searching'. Manber is\n";

	size_t count = 10000000;
	double cost = 0;

	size_t loops = 3;
	for (size_t k = 0; k < loops; k++ ) {

		clock_t end, start = clock();
		for (size_t j = 0; j <count; j++) {
			results_t *rset = filter_match(f, str.data(), str.size());
			filter_results_delete(rset);
		}

		end = clock();
		double use = end - start;
		cost = cost + use;
		cout << "cost: " << use/CLOCKS_PER_SEC << "s" << endl;
	}

	double avg = cost / loops;

	cout << "avg:" << avg / CLOCKS_PER_SEC << "s" << endl;
	filter_delete(f);

	return 0;
}



int main(int argc, char **argv)
{
	//	test_c ();
	//test_perf_wm();
	//test_perf_kmp();

	//test_single();
	//test_single_cnt();
	test_perf_kmp_cnt();

	return 0;
}

