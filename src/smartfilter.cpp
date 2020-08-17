#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hs.h>
#include <unistd.h>

#include <vector>

using namespace std;

//shared_ptr

#ifdef __cplusplus
	extern "C" {
#endif

#define DEBUG

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
	vector<unsigned int> flags;
	vector<unsigned int> ids;
};

struct match_t {
	unsigned int id;
	unsigned long long from;
	unsigned long long to;
};

struct context_t {
	filter_t *filter;
	const char *data;
	size_t dlen;
};

/*
 * 
*/
static int on_match(unsigned int id, unsigned long long from, unsigned long long to, unsigned int flags, void *context) {

	context_t *ctx = (context_t *)context;
	filter_t *f = ctx->filter;

	//export hit payload
	//hit expr id

    printf("Hit id:%u Match for pattern \"%s\" at offset %llu-%llu flags:%u\n", id, f->name, from, to, flags);

	char buff[50];
	memcpy(buff, ctx->data + from, to - from);
	DD("hit_payload:[%s]\n", buff);

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

	strncpy(filter->name, name, strlen(name));
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

int filter_match(void *filter, const char *inputData, size_t dlen)
{
	filter_t *f = (filter_t *)filter;

	if (filter == NULL || inputData == NULL || dlen == 0) {
		return -1;
	}

	context_t ctx = { f, inputData, dlen };

	if (hs_scan(f->database, inputData, dlen, 0, f->scratch, on_match, &ctx) != HS_SUCCESS) {
		fprintf(stderr, "ERROR: Unable to scan input buffer. Exiting.\n");
		return -1;
	}

	return 0;
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


