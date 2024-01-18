/*Copyright 2021 Retune DSP 
* Copyright 2022 NXP 
*
* NXP Confidential. This software is owned or controlled by NXP 
* and may only be used strictly in accordance with the applicable license terms.  
* By expressly accepting such terms or by downloading, installing, 
* activating and/or otherwise using the software, you are agreeing that you have read, 
* and that you agree to comply with and are bound by, such license terms.  
* If you do not agree to be bound by the applicable license terms, 
* then you may not retain, install, activate or otherwise use the software.
*
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
