

#ifndef __SMARTFILTER_H__
#define __SMARTFILTER_H__

#ifdef __cplusplus
extern "C" {
#endif

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

void *filter_new(const char *name, const char **patterns, unsigned int *flags, unsigned int *ids, size_t size);
void *filter_delete(void *f);
int filter_match(void *filter, const char *inputData, size_t dlen);

//for test
void hello();
void printArray(int *a, size_t size);
void printStrArray(const char **a, size_t size);

#ifdef __cplusplus
}
#endif

#endif

