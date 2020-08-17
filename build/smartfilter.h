

#ifndef __SMARTFILTER_H__
#define __SMARTFILTER_H__

#ifdef __cplusplus
extern "C" {
#endif

#define RESULT_SET_MAX			8

#define HS_FLAG_CASELESS        1
#define HS_FLAG_DOTALL          2
#define HS_FLAG_SINGLEMATCH     8
#define HS_FLAG_SOM_LEFTMOST    256

#define RULES_HS_FLAGS   (HS_FLAG_CASELESS    | \
		HS_FLAG_SINGLEMATCH | \
		HS_FLAG_DOTALL)

#define RULES_HS_FLAGS_LEFTMOST        (HS_FLAG_CASELESS    | \
		HS_FLAG_DOTALL      | \
		HS_FLAG_SOM_LEFTMOST)

struct result_t {
	unsigned int id;
	unsigned long long from;
	unsigned long long to;
	result_t():id(0), from(0), to(0) {};
	~result_t() {};
};

struct result_set_t {
	unsigned int cursor;
	result_t results[RESULT_SET_MAX]; //new
	result_set_t(): cursor(0) {};
	~result_set_t() {};
};

void *filter_new(const char *name, const char **patterns, unsigned int *flags, unsigned int *ids, size_t size);
void filter_delete(void *f);
result_set_t * filter_match(void *filter, const char *inputData, size_t dlen);
void filter_result_set_delete(result_set_t *result_set);

//for test
void hello();
void printArray(int *a, size_t size);
void printStrArray(const char **a, size_t size);

#ifdef __cplusplus
}
#endif

#endif

