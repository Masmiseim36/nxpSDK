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
#include "RdspMemoryUtilsPublic.h"

#define RDSP_MEMORY_UTILS_USES_STDLIB 0
#if RDSP_MEMORY_UTILS_USES_STDLIB
#include <stdlib.h>
#endif

#define RDSP_MEMORY_UTILS_USES_STRING 1
#if RDSP_MEMORY_UTILS_USES_STRING
#include <string.h>
#endif

#define RDSP_ENABLE_MEMCHECK 0
#if RDSP_ENABLE_MEMCHECK
#include "memcheck.h"
#endif

#if RDSP_ENABLE_PRINTF
#include <stdio.h>
#endif

static uint32_t extmem_analysis_mode_flag = 0;

static void* ext_mem_baseptr;
static void* ext_mem_nextptr;
static size_t ext_mem_size_bytes;
static size_t ext_mem_bytes_left;
static size_t ext_mem_bytes_used;

static void* ext_mem_scratch_baseptr;
static void* ext_mem_scratch_nextptr;
static size_t ext_mem_scratch_size_bytes;
static size_t ext_mem_scratch_bytes_left;
static size_t ext_mem_scratch_bytes_used;
static size_t ext_mem_scratch_max_bytes_used;

uint32_t rdsp_plugin_get_heapmem_analysis_flag(void) {
	return extmem_analysis_mode_flag;
}

void rdsp_plugin_set_heapmem_analysis_flag(uint32_t Aanalysis_mode_flag) {
	extmem_analysis_mode_flag = Aanalysis_mode_flag;
}

/*
 * Memory allocation 
 */

void rdsp_plugin_malloc_init(void* Aextmem_baseptr, void* Aextmem_nextptr, size_t AextmemSize) {
	ext_mem_baseptr = Aextmem_baseptr;
	ext_mem_nextptr = Aextmem_nextptr;
	ext_mem_size_bytes = AextmemSize;
	ext_mem_bytes_left = AextmemSize;
	ext_mem_bytes_used = 0;
}

size_t rdsp_plugin_malloc_GetAllocatedBytes(void) {
	return ext_mem_bytes_used;
}

void* rdsp_plugin_malloc(size_t Asize, MemAlign_t Aalign) {
#if RDSP_ENABLE_MEMCHECK
	void* p = memcheck_malloc_align(Asize, align, NULL, 0, NULL);
	return p;
#else
	void* pRet = NULL;
	size_t aligned_size = (Asize + (size_t)Aalign - 1) & ~((size_t)Aalign - 1);

	/* For heap memory analysis */
	if (extmem_analysis_mode_flag == 1) {
		ext_mem_bytes_used += aligned_size;
		return (void*)0xFFFF;
	}

#if RDSP_MEMORY_UTILS_USES_STDLIB
	pRet = malloc(aligned_size);
#else
	int32_t failed = 0;
	// Only allocate if we have enough memory
	if (ext_mem_bytes_left >= aligned_size) {
		void* tmp = ext_mem_nextptr;
		pRet = (void*)((uintptr_t)(tmp + ((size_t)Aalign - 1)) & ~(uintptr_t)((size_t)Aalign - 1));

		ext_mem_nextptr = pRet + aligned_size;
		ext_mem_bytes_left -= aligned_size;
		ext_mem_bytes_used += aligned_size;

		// Check for alignment
		if (((uintptr_t)pRet % (size_t)Aalign) != 0) {
			failed = 1;
		}
	}
	else {
		failed = 1;
	}

	if (failed == 1) {
		pRet = NULL;
#ifdef RDSP_ENABLE_PRINTF
		(void)printf("Not enough memory available or requested alignment not supported!\n");
#endif
	}
#endif // RDSP_MEMORY_UTILS_USES_STDLIB==1

	return pRet;
#endif
}

void rdsp_plugin_free(void* Aptr) {
#if RDSP_MEMORY_UTILS_USES_STDLIB
	return free(Aptr);
#else
	// extmem should be free'd outside
#endif
}

/*
 * Scratch memory
 */

void rdsp_plugin_scratch_init(void* Aext_scratch_mem_baseptr, void* Aext_scratch_mem_nextptr, size_t Aext_scratch_memSize) {
	ext_mem_scratch_baseptr = Aext_scratch_mem_baseptr;
	ext_mem_scratch_nextptr = Aext_scratch_mem_nextptr;

	ext_mem_scratch_size_bytes = Aext_scratch_memSize;
	ext_mem_scratch_bytes_left = Aext_scratch_memSize;
	ext_mem_scratch_bytes_used = 0;
	ext_mem_scratch_max_bytes_used = 0;
}

size_t rdsp_plugin_scratch_GetAllocatedBytes(void) {
	return ext_mem_scratch_max_bytes_used;
}

void rdsp_plugin_scratch_reset(void) {
	ext_mem_scratch_nextptr = ext_mem_scratch_baseptr;
	ext_mem_scratch_bytes_left = ext_mem_scratch_size_bytes;
	ext_mem_scratch_bytes_used = 0;
}

void* rdsp_plugin_scratch_malloc(size_t Asize, MemAlign_t Aalign) {

	void *pRet = NULL;
	size_t aligned_size = (Asize + (size_t)Aalign - 1) & ~((size_t)Aalign - 1);

	/* For heap memory analysis */
	if (extmem_analysis_mode_flag == 1) {
		ext_mem_scratch_bytes_used += aligned_size;
		if (ext_mem_scratch_bytes_used > ext_mem_scratch_max_bytes_used) {
			ext_mem_scratch_max_bytes_used = ext_mem_scratch_bytes_used;
		}
		return (void*) 0xFFFF;
	}

#if RDSP_MEMORY_UTILS_USES_STDLIB
	pRet = malloc(aligned_size);
#else
	// Only allocate if we have enough memory
	if (ext_mem_scratch_bytes_left >= aligned_size) {
		void* tmp = ext_mem_scratch_nextptr;
		pRet = (void*)((uintptr_t)(tmp + ((size_t)Aalign - 1)) & ~(uintptr_t)((size_t)Aalign - 1));
		ext_mem_scratch_nextptr = pRet + aligned_size;
		ext_mem_scratch_bytes_left -= aligned_size;
		ext_mem_scratch_bytes_used += aligned_size;
		if (ext_mem_scratch_bytes_used > ext_mem_scratch_max_bytes_used) {
			ext_mem_scratch_max_bytes_used = ext_mem_scratch_bytes_used;
		}
	}
	else {
#ifdef RDSP_ENABLE_PRINTF
		(void)printf("Not enough memory available! Requested %z scratch bytes but only %z scratch bytes available.\n", Asize, ext_mem_scratch_bytes_left);
#endif
	}
#endif // RDSP_MEMORY_UTILS_USES_STDLIB
	return pRet;
}

/*
 * Memory utilities
 */

void* rdsp_plugin_memset(void* Aptr, uint8_t Aval, size_t Asize) {
#if RDSP_MEMORY_UTILS_USES_STRING
	return memset(Aptr, Aval, Asize);
#else
    // Replicate val from 8b to 32b
    uint32_t val32;
    uint8_t* val8 = (uint8_t*)&val32;
	size_t i;
    for (i = 0; i < 4; i++) {
        val8[i] = Aval;
    }

    // Set 32b chunks
    i = 0;
    size_t* ptr32 = (uint32_t*)Aptr;
    for (; i < Asize >> 2; i++) {
        *ptr32++ = val32;
    }

    // Do the rest
    uint8_t* ptr8 = (uint8_t*)ptr32;
	size_t rem = Asize - (i << 2);
    while (rem-- > 0) {
        *ptr8++ = Aval;
    }

    return Aptr;
#endif
}

void* rdsp_plugin_memset_32b(void* Aptr, uint32_t Aval, size_t Asize) {
	uint32_t* ptr32 = (uint32_t*)Aptr;
	for (size_t i = 0; i < Asize; i++) {
		ptr32[i] = Aval;
	}
	return Aptr;
}

void* rdsp_plugin_memset_64b(void* Aptr, uint64_t Aval, size_t Asize) {
	uint64_t* ptr64 = (uint64_t*)Aptr;
	for (size_t i = 0; i < Asize; i++) {
		ptr64[i] = Aval;
	}
	return Aptr;
}

void* rdsp_plugin_memset_float(rdsp_float* Aptr, rdsp_float Aval, size_t Alength) {
	for (size_t i = 0; i < Alength; i++) {
		Aptr[i] = Aval;
	}
	return Aptr;
}

void* rdsp_plugin_memset_complex(rdsp_complex* Aptr, rdsp_complex Aval, size_t Alength) {
	for (size_t i = 0; i < Alength; i++) {
#ifdef RDSP_USES_AFLOAT_CODE
		Aptr[i] = Aval;
#elif defined(HIFI4) || defined(FUSIONDSP)
		Aptr[i] = Aval;
#else
		Aptr[i][0] = Aval[0];
		Aptr[i][1] = Aval[1];
#endif
	}
	return Aptr;
}

void* rdsp_plugin_memcpy(void* Adest, void* Asrc, size_t Asize) {
#if RDSP_MEMORY_UTILS_USES_STRING
	return memcpy(Adest, Asrc, Asize);
#else
	uint32_t* src32 = (uint32_t*) Asrc;
	uint32_t* dst32 = (uint32_t*) Adest;
	size_t i = 0;
	for(; i < Asize >> 2; i++) {
		*dst32++ = *src32++;
	}

	// Do the rest
	uint8_t *src8 = (uint8_t *) src32;
	uint8_t *dst8 = (uint8_t *) dst32;
	size_t rem = Asize - (i << 2);
	while (rem-- > 0) {
		*dst8++ = *src8++;
	}

    return Adest;
#endif
}

void* rdsp_plugin_memcpy_32b(void* Adest, const void* Asrc, size_t Alength) {
#if RDSP_MEMORY_UTILS_USES_STRING
	return memcpy(Adest, Asrc, sizeof(uint32_t) * Alength);
#else
	uint32_t* pdest32 = (uint32_t*)Adest;
	uint32_t* psrc32 = (const uint32_t*)Asrc;
	for (size_t i = 0; i < Alength; i++) {
		pdest32[i] = psrc32[i];
	}
	return Adest;
#endif
}

void* rdsp_plugin_memcpy_64b(void* Adest, const void* Asrc, size_t Alength) {
#if RDSP_MEMORY_UTILS_USES_STRING
	return memcpy(Adest, Asrc, sizeof(uint64_t) * Alength);
#else
	uint64_t* pdest64 = (const uint64_t*)Adest;
	uint64_t* psrc64 = (uint64_t*)Asrc;
	for (size_t i = 0; i < Alength; i++) {
		pdest64[i] = psrc64[i];
	}
	return Adest;
#endif
}

void* rdsp_plugin_memcpy_float(rdsp_float* Adest, rdsp_float* Asrc, size_t Alength) {
	return memcpy(Adest, Asrc, Alength * sizeof(rdsp_float));
}

void* rdsp_plugin_memcpy_complex(rdsp_complex* Adest, rdsp_complex* Asrc, size_t Alength) {
	return memcpy(Adest, Asrc, Alength * sizeof(rdsp_complex));
}

void* rdsp_plugin_memmove(void* Adest, const void* Asrc, size_t Asize) {
#if RDSP_MEMORY_UTILS_USES_STRING
	return memmove(Adest, Asrc, Asize);
#else
	// FIXME: rdsp_plugin_memmove requires RDSP_MEMORY_UTILS_USES_STRING==1
	return memmove(Adest, Asrc, Asize);
#endif
}

int32_t rdsp_plugin_memcompare(void* Ax, void* Ay, size_t Asize) {
#if RDSP_MEMORY_UTILS_USES_STRING
	return memcmp(Ax, Ay, Asize);
#else
	const uint32_t *x32 = (const uint32_t *) Ax;
	const uint32_t *y32 = (const uint32_t *) Ay;
	size_t i = 0;
	for (; i < Asize >> 2; i++) {
		if (*x32++ != *y32++) {
			return 1;
		}
	}

	// Do the rest
	size_t rem = Asize - (i << 2);
	const uint8_t *x8 = (const uint8_t *) x32;
	const uint8_t *y8 = (const uint8_t *) y32;
	while (rem-- > 0) {
		if (*x8++ != *y8++) {
			return 1;
		}
	}
	return 0;
#endif
}
