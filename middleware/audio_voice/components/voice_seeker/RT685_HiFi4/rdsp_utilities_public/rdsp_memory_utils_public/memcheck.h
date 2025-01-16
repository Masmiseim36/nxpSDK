/*
 * Copyright 2021 by Retune DSP
 * Copyright 2022-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
*/
#ifndef MEMCHECK_H
#define MEMCHECK_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif


void* memcheck_malloc_align(size_t size, size_t alignment, const char* file, int line, const char* function);
void* memcheck_malloc(size_t size, const char* file, int line, const char* function);

void memcheck_free_align(void *data);
void memcheck_free(void* addr);

void memcheck_check(void);


#ifdef __cplusplus
}
#endif
#endif // MEMCHECK_H
