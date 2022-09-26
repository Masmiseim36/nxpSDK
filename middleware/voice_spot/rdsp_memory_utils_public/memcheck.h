#ifndef MEMCHECK_H
#define MEMCHECK_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif


void* memcheck_malloc_align(size_t size, int alignment, const char* file, int line, const char* function);
void* memcheck_malloc(size_t size, const char* file, int line, const char* function);

void memcheck_free_align(void *data);
void memcheck_free(void* addr);

void memcheck_check(void);


#ifdef __cplusplus
}
#endif
#endif // MEMCHECK_H
