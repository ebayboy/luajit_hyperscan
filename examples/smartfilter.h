

#ifndef __SMARTFILTER_H__
#define __SMARTFILTER_H__

#ifdef __cplusplus
extern "C" {
#endif

void *filter_new(const char *name, const char **patterns, unsigned int *flags, unsigned int *ids, size_t size);
void *filter_free(void *f);
int filter_match(void *filter, const char *inputData, size_t dlen);

#ifdef __cplusplus
}
#endif

#endif

