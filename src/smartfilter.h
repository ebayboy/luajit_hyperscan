

#ifndef __SMARTFILTER_H__
#define __SMARTFILTER_H__

#ifdef __cplusplus
extern "C" {
#endif

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

