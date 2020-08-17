

#ifndef __SMARTFILTER_H__
#define __SMARTFILTER_H__

#ifdef __cplusplus
extern "C" {
#endif

const static unsigned int RESULT_SET_MAX = 8;

typedef enum {
	F_HS_FLAG_CASELESS = 1,
	F_HS_FLAG_DOTALL  = 2,
	F_HS_FLAG_SINGLEMATCH = 8,
	F_HS_FLAG_SOM_LEFTMOST = 256
} F_HS_ENUM_T;

const static unsigned int RULES_HS_FLAGS = F_HS_FLAG_CASELESS | F_HS_FLAG_SINGLEMATCH | F_HS_FLAG_DOTALL;
const static unsigned int RULES_HS_FLAGS_LEFTMOST = F_HS_FLAG_CASELESS | F_HS_FLAG_DOTALL | F_HS_FLAG_SOM_LEFTMOST;

typedef struct {
	unsigned int id;
	unsigned long long from;
	unsigned long long to;
} result_t;

typedef struct {
	unsigned int cursor;
	result_t results[RESULT_SET_MAX]; //new
} result_set_t;

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

