
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

#include <hs.h>

static int eventHandler(unsigned int id, unsigned long long from,
		unsigned long long to, unsigned int flags, void *ctx) 
{
	int *cnt = (int *)ctx;
	(*cnt)++;
	//  printf("id:%u from:%lu to:%lu\n", id, from, to);
	return 0;
}

const static unsigned int RULES_HS_FLAGS = HS_FLAG_CASELESS | HS_FLAG_SINGLEMATCH | HS_FLAG_DOTALL;
const static unsigned int RULES_HS_FLAGS_LEFTMOST = HS_FLAG_CASELESS | HS_FLAG_DOTALL | HS_FLAG_SOM_LEFTMOST;

int test_single()
{
	const char *str = "This is some text I made up.  This will be testing\n" 
		"multi-pattern matching from Wu/Manber's paper called\n"
		"'A Fast Algorithm for Multi-Pattern Searching'. Manber is\n";

	hs_database_t *database;
	hs_compile_error_t *compile_err;

	unsigned int flags[] = {RULES_HS_FLAGS, RULES_HS_FLAGS, RULES_HS_FLAGS, RULES_HS_FLAGS};
	unsigned int ids[] = {1000, 1001, 1002, 1003};
	const char * patterns[] = { "this is", "pattern", "google!", "anber"};
	hs_scratch_t *scratch = NULL;

	if (hs_compile_multi( patterns,
				flags,
				ids,
				4,
				HS_MODE_BLOCK, 
				NULL,  //platform
				&database, 
				&compile_err) != HS_SUCCESS) {
		hs_free_compile_error(compile_err);
		if (compile_err->expression < 0) {
			fprintf(stderr, "Error:%s\n", compile_err->message);
		} else {
			fprintf(stderr, "ERROR: Pattern %s' failed with error:%s\n" ,patterns[compile_err->expression], compile_err->message);
		}
	}

	if (hs_alloc_scratch(database, &scratch) != HS_SUCCESS) {
		fprintf(stderr, "ERROR: Unable to allocate scratch space. Exiting.\n");
		hs_free_database(database);
		return -1;
	}

	int cnt = 0;
	if (hs_scan(database, str, strlen(str), 0, scratch, eventHandler, &cnt) != HS_SUCCESS) 
	{
		fprintf(stderr, "ERROR: Unable to scan input buffer. Exiting.\n");
		hs_free_scratch(scratch);
		hs_free_database(database);
		return -1;
	}

	if (compile_err) {
		hs_free_compile_error(compile_err);
	}

	hs_free_scratch(scratch);
	hs_free_database(database);

	return 0;
}

int test_perf()
{
	const char *str = "This is some text I made up.  This will be testing\n" 
		"multi-pattern matching from Wu/Manber's paper called\n"
		"'A Fast Algorithm for Multi-Pattern Searching'. Manber is\n";

	hs_database_t *database;
	hs_compile_error_t *compile_err;

	unsigned int flags[] = {RULES_HS_FLAGS, RULES_HS_FLAGS, RULES_HS_FLAGS, RULES_HS_FLAGS};
	unsigned int ids[] = {1000, 1001, 1002, 1003};
	const char * patterns[] = { "this is", "pattern", "google!", "anber"};
	hs_scratch_t *scratch = NULL;

	if (hs_compile_multi( patterns,
				flags,
				ids,
				4,
				HS_MODE_BLOCK, 
				NULL,  //platform
				&database, 
				&compile_err) != HS_SUCCESS) {
		hs_free_compile_error(compile_err);
		if (compile_err->expression < 0) {
			fprintf(stderr, "Error:%s\n", compile_err->message);
		} else {
			fprintf(stderr, "ERROR: Pattern %s' failed with error:%s\n" ,patterns[compile_err->expression], compile_err->message);
		}

		hs_free_compile_error(compile_err);
		hs_free_scratch(scratch);
		hs_free_database(database);
		return -1;
	}

	if (hs_alloc_scratch(database, &scratch) != HS_SUCCESS) {
		fprintf(stderr, "ERROR: Unable to allocate scratch space. Exiting.\n");
		hs_free_database(database);
		return -1;
	}

	size_t count = 10000000;
	size_t i = 0;

	double all_cost = 0;

	for (size_t j = 0; j < 3; j++) {
		clock_t end, start = clock();
		size_t cnt = 0;
		for (i = 0; i < count; i++) {
			cnt = 0;
			if (hs_scan(database, str, strlen(str), 0, scratch, eventHandler, &cnt) != HS_SUCCESS) {
				fprintf(stderr, "ERROR: Unable to scan input buffer. Exiting.\n");
				hs_free_scratch(scratch);
				hs_free_database(database);
				return -1;
			}
		}
		end  = clock();
		double cost = end - start;
		printf("cost :%f(s) cnt:%u\n", cost/CLOCKS_PER_SEC, cnt);
		all_cost += cost;
	}

	double avg = all_cost/3;
	printf("avg:%f(s)\n", avg/CLOCKS_PER_SEC);

	if (compile_err) {
		hs_free_compile_error(compile_err);
	}

	hs_free_scratch(scratch);
	hs_free_database(database);

	return 0;
}

int main()
{
	test_perf();
	return 0;
}
