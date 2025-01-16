/*
 * Copyright 2021 by Retune DSP
 * Copyright 2022-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
*/
#ifndef RDSP_MEMORY_UTILS_PUBLIC
#define RDSP_MEMORY_UTILS_PUBLIC

#include "RdspTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    MEM_ALIGN_1 = 1,	/*!< Align 1 byte */
    MEM_ALIGN_2 = 2,	/*!< Align 2 byte */
    MEM_ALIGN_4 = 4,	/*!< Align 4 byte */
    MEM_ALIGN_8 = 8,	/*!< Align 8 byte */
    MEM_ALIGN_16 = 16,	/*!< Align 16 byte */
} MemAlign_t;

#if defined(RDSP_128BIT_PLATFORM) || defined(__ARM_NEON) || defined(_WIN64)
#define RDSP_ALIGN_SIZE MEM_ALIGN_16
#else
#define RDSP_ALIGN_SIZE MEM_ALIGN_8
#endif

#ifdef _WIN32
#define RDSP_MEM_ALIGN
#elif defined(HIFI4) || defined(FUSIONF1)
#define RDSP_MEM_ALIGN __attribute__((aligned(8)))
#else
#define RDSP_MEM_ALIGN __attribute__((aligned(RDSP_ALIGN_SIZE)))
#endif

#define RDSP_PLUGIN_MALLOC_ALIGN(type, n) (type*)rdsp_plugin_malloc(sizeof(type) * n, RDSP_ALIGN_SIZE)
#define RDSP_PLUGIN_SCRATCH_MALLOC_ALIGN(type, n) (type*)rdsp_plugin_scratch_malloc(sizeof(type) * n, RDSP_ALIGN_SIZE)

extern uint32_t rdsp_plugin_get_heapmem_analysis_flag(void);
extern void rdsp_plugin_set_heapmem_analysis_flag(uint32_t Aanalysis_mode_flag);

extern void rdsp_plugin_malloc_init(void* Aextmem_baseptr, void* Aextmem_nextptr, size_t AextmemSize);
extern size_t rdsp_plugin_malloc_GetAllocatedBytes(void);
extern void* rdsp_plugin_malloc(size_t Asize, MemAlign_t Aalign);

extern void rdsp_plugin_scratch_init(void* Aext_scratch_mem_baseptr, void* Aext_scratch_mem_nextptr, size_t Aext_scratch_memSize);
extern size_t rdsp_plugin_scratch_GetAllocatedBytes(void);
extern void rdsp_plugin_scratch_reset(void);
extern void* rdsp_plugin_scratch_malloc(size_t Asize, MemAlign_t Aalign);

extern void* rdsp_plugin_memset(void* Aptr, uint8_t Aval, size_t Asize);
extern void* rdsp_plugin_memset_32b(void* Aptr, uint32_t Aval, size_t Alength);
extern void* rdsp_plugin_memset_64b(void* Aptr, uint64_t Aval, size_t Alength);
extern void* rdsp_plugin_memset_float(rdsp_float* Aptr, rdsp_float Aval, size_t Alength);
extern void* rdsp_plugin_memset_complex(rdsp_complex* Aptr, rdsp_complex Aval, size_t Alength);

extern void* rdsp_plugin_memcpy(void* Adest, const void* Asrc, size_t Asize);
extern void* rdsp_plugin_memcpy_32b(void* Adest, const void* Asrc, size_t Asize);
extern void* rdsp_plugin_memcpy_64b(void* Adest, const void* Asrc, size_t Asize);
extern void* rdsp_plugin_memcpy_float(rdsp_float* Adest, const rdsp_float* Asrc, size_t Alength);
extern void* rdsp_plugin_memcpy_complex(rdsp_complex* Adest, const rdsp_complex* Asrc, size_t Alength);

extern void* rdsp_plugin_memmove(void* Adest, const void* Asrc, size_t Asize);
extern int32_t rdsp_plugin_memcompare(void* Ax, void* Ay, size_t Asize);
extern void rdsp_plugin_free(void* Aptr);

#ifdef __cplusplus
}
#endif

#endif /* RDSP_MEMORY_UTILS_PUBLIC */
