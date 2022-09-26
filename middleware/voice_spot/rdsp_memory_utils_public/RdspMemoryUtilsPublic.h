/*
 * Copyright (c) 2021 by Retune DSP. This code is the confidential and
 * proprietary property of Retune DSP and the possession or use of this
 * file requires a written license from Retune DSP.
 *
 * Contact information: info@retune-dsp.com
 *                      www.retune-dsp.com
 */

#ifndef RDSP_MEMORY_UTILS_PUBLIC
#define RDSP_MEMORY_UTILS_PUBLIC

#include <stddef.h>
#include <stdint.h>
#include "memcheck.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef enum {
    MEM_ALIGN_1 = 1,	/*!< Align 1 byte */
    MEM_ALIGN_2 = 2,	/*!< Align 2 byte */
    MEM_ALIGN_4 = 4,	/*!< Align 4 byte */
    MEM_ALIGN_8 = 8,	/*!< Align 8 byte */
    MEM_ALIGN_16 = 16,	/*!< Align 16 byte */
    //MEM_ALIGN_32 = 32,	/*!< Align 32 byte */
} MemAlign_t;

extern void rdsp_plugin_malloc_init(void* Aextmem_baseptr, void* Aextmem_nextptr, uint32_t AextmemSize);
extern uint32_t rdsp_plugin_malloc_GetAllocatedBytes();
extern void* rdsp_plugin_malloc(size_t Asize, int32_t align);
extern void* rdsp_plugin_malloc_func(uint32_t size, int32_t align);

extern void rdsp_plugin_scratch_init(void* Aext_scratch_mem_baseptr, void* Aext_scratch_mem_nextptr, uint32_t Aext_scratch_memSize);
extern uint32_t rdsp_plugin_scratch_GetAllocatedBytes();
extern void rdsp_plugin_scratch_reset();
extern void* rdsp_plugin_scratch_malloc(uint32_t Asize, int32_t Aalign);

extern void* rdsp_plugin_memset(void* Aptr, uint8_t Aval, uint32_t Asize);
extern void* rdsp_plugin_memcpy(void* Adest, void* Asrc, uint32_t Asize);
extern void* rdsp_plugin_memmove(void* Adest, const void* Asrc, uint32_t Asize);
extern uint32_t rdsp_plugin_memcompare(void* Ax, void* Ay, uint32_t Asize);
extern void rdsp_plugin_free(void* Aptr);

#ifdef RDSP_128BIT_PLATFORM
#define RDSP_PLUGIN_MALLOC_ALIGN(type, n) (type*)rdsp_plugin_malloc(sizeof(type)*n, MEM_ALIGN_16)
#else
#define RDSP_PLUGIN_MALLOC_ALIGN(type, n) (type*)rdsp_plugin_malloc(sizeof(type)*n, MEM_ALIGN_8)
#endif

#ifdef __cplusplus
}
#endif

#endif /* RDSP_MEMORY_UTILS_PUBLIC */
