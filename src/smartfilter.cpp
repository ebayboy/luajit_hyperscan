#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hs.h>
#include <unistd.h>

#include <vector>

#include "smartfilter.h"

using namespace std;

//shared_ptr

#ifdef __cplusplus
	extern "C" {
#endif

#ifdef DEBUG
#define DD(fmt, args...) printf(fmt, ##args)
#else
#define DD(fmt, args...)
#endif

struct filter_t {
	char name[512];
	hs_database_t *database;
	hs_scratch_t *scratch;

	vector<const char *> patterns; //need free
	vector<unsigned int> flags; vector<unsigned int> ids;

	filter_t():database(NULL), scratch(NULL), flags(0) {};
	~filter_t() {};
};

struct context_t {
	filter_t *filter;
	const char *data;
	size_t dlen;
	results_t *results;

	context_t (filter_t *f, const char *data, size_t dlen): filter(f), data(data), dlen(dlen), results(NULL) {};
	~context_t () {};
};

static int on_match_cnt(unsigned int id, unsigned long long from, unsigned long long to, unsigned int flags, void *ctx) {
	int *cnt = (int *)ctx;
	(*cnt)++;
    DD("Hit id:%u Match for pattern \"%s\" at offset %llu-%llu cnt:%u\n", id, f->name, from, to, cnt);
	return 0;
}

static int on_match(unsigned int id, unsigned long long from, unsigned long long to, unsigned int flags, void *context) {

	context_t *ctx = (context_t *)context;
	//filter_t *f = ctx->filter;
	unsigned int cursor = 0;
	result_t *r = NULL;

	if (ctx->results == NULL){
		ctx->results = new results_t();
		if (ctx->results == NULL) {
			DD("Error: new results()");
			return -1;
		}
	} 

	cursor = ctx->results->cursor;
	if (cursor >= RESULT_SET_MAX) {
		DD("WARN: too max result ! continue! rule->cursor:%u\n", cursor);
		return 0;
	}

	r = &ctx->results->results[cursor];

	r->id = id;
	r->from = from;
	r->to= to;

	ctx->results->cursor++;
    DD("Hit id:%u Match for pattern \"%s\" at offset %llu-%llu cursor:%u\n", id, f->name, from, to, cursor);

    return 0;
}

void filter_delete(void *filter)
{
	filter_t *f = (filter_t *)filter;
	if (f) {
		if (f->scratch) {
			fprintf(stderr, "free scratch %p\n", f->scratch);
			hs_free_scratch(f->scratch);
		}
		if (f->database) {
			fprintf(stderr, "free database %p\n", f->database);
			hs_free_database(f->database);
		}

		fprintf(stderr, "free filter %p\n", f);
		delete f;
	}
}

void *filter_new(const char *name, const char **patterns, unsigned int *flags, unsigned int *ids, size_t size)
{
	filter_t *filter = NULL;
	hs_compile_error_t *compile_err = NULL;
	size_t i;
	hs_error_t err;

	filter = new filter_t();
	memset((void *)filter, 0, sizeof(filter_t));
	if (filter == NULL) {
		fprintf(stderr, "ERROR: Unable to alloc filter!!!\n");
		goto error;
	}
	fprintf(stderr, "malloc filter %p\n", filter);

	strncpy(filter->name, name, sizeof(filter->name) - 1);
	for (i = 0; i < size; i++) {
		filter->patterns.push_back(patterns[i]);
		filter->flags.push_back(flags[i]);
		filter->ids.push_back(ids[i]);
	}

	//build database
	err = hs_compile_multi(filter->patterns.data(), 
			filter->flags.data(),
			filter->ids.data(),
			filter->patterns.size(),
			HS_MODE_BLOCK, 
			NULL,  //platform
			&filter->database, 
			&compile_err);
	if (err != HS_SUCCESS) {
		if (compile_err->expression < 0) {
			fprintf(stderr, "Error:%s\n", compile_err->message);
		} else {
			fprintf(stderr, "ERROR: Pattern %s' failed with error:%s\n" ,patterns[compile_err->expression], compile_err->message);
		}
		goto error;
	}

	fprintf(stderr, "alloc database:%p\n", filter->database);

	//alloc scratch
	if (hs_alloc_scratch(filter->database, &filter->scratch) != HS_SUCCESS) {
		fprintf(stderr, "ERROR: Unable to allocate scratch space. Exiting.\n");
		goto error;
	}
	fprintf(stderr, "alloc scratch:%p\n", filter->scratch);

	return filter;

error:
	if (compile_err) {
		hs_free_compile_error(compile_err);
	}

	filter_delete(filter);

	return NULL;
};

int filter_match_cnt(void *filter, const char *inputData, size_t dlen)
{
	filter_t *f = (filter_t *)filter;
	int cnt  = 0;

	if (filter == NULL || inputData == NULL || dlen == 0) {
		return -1;
	}

	if (hs_scan(f->database, inputData, dlen, 0, f->scratch, on_match_cnt, &cnt) != HS_SUCCESS) {
		fprintf(stderr, "ERROR: Unable to scan input buffer. Exiting.\n");
		goto error;
	}

	DD("cnt:%d", cnt);

	return cnt;

error:

	return -1;
}

/* RETURN:  error: -1;  succcess:  match times */
results_t * filter_match(void *filter, const char *inputData, size_t dlen)
{
	filter_t *f = (filter_t *)filter;
	context_t *ctx = NULL;
	results_t *rset = NULL;

	if (filter == NULL || inputData == NULL || dlen == 0) {
		return NULL;
	}

	if ((ctx = new context_t(f, inputData, 0)) == NULL) {
		fprintf(stderr, "Error: new context_t!!!");
		goto error;
	}

	if (hs_scan(f->database, inputData, dlen, 0, f->scratch, on_match, ctx) != HS_SUCCESS) {
		fprintf(stderr, "ERROR: Unable to scan input buffer. Exiting.\n");
		goto error;
	}

	if (ctx) {
		if (ctx->results) {
			rset = ctx->results;
		}
		delete ctx;
	}

	if (rset) {
		DD("rset->cursor:%u", rset->cursor);
	}

	return rset;

error:
	if (ctx) {
		if (ctx->results) {
			delete ctx->results;
			ctx->results = NULL;
		}
		delete ctx;
	}

	return NULL;
}

void filter_results_delete(results_t *results)
{
	if(results) {
		delete results;
	}
}

void hello()
{
	printf("%s:%d hello!\n", __func__, __LINE__);
}

void printArray(int *a, size_t size)
{
	size_t i;
	for (i=0; i <size; i++)
	{
		printf("a[%lu]:%d ", i, a[i]);
	}
	printf("\n");
}

void printStrArray(const char **a, size_t size)
{
	size_t i;
	for (i=0; i <size; i++)
	{
		printf("a[%lu]:%s ", i, a[i]);
	}
	printf("\n");
}

#ifdef __cplusplus
}
#endif


