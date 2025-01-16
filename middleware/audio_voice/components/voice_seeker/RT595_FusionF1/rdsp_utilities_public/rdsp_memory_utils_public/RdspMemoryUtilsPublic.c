/*
 * Copyright 2021 by Retune DSP
 * Copyright 2022-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
*/
#include "RdspMemoryUtilsPublic.h"

#ifdef _WIN32
#define RDSP_MEMORY_UTILS_USES_STDLIB 1
#else
#define RDSP_MEMORY_UTILS_USES_STDLIB 0
#endif

#if RDSP_MEMORY_UTILS_USES_STDLIB
#include <stdlib.h>
#if _WIN32
#define ALIGNED_MALLOC(size, align) _aligned_malloc(size, align);
#else
#define ALIGNED_MALLOC(size, align) aligned_malloc(align, size);
#endif
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
	// Pointer to the start of the aligned memory section
	void* pRet = NULL;
	size_t align = (size_t)Aalign; // Must cast enum to 32/64 bit int type

#if RDSP_ENABLE_MEMCHECK
	pRet = memcheck_malloc_align(Asize, align, NULL, 0, NULL);
	return pRet;
#else


	// Ensure next pointer is aligned in case previous malloc had a different alignment
	uintptr_t mem_next = (uintptr_t)ext_mem_nextptr; // Use uintptr_t for pointer arithmetic
	uintptr_t mem_start_align = (mem_next + (align - 1)) & ~(align - 1);

	// Compute memory size requested including added alignment
	size_t aligned_size = (Asize + (align - 1)) & ~(align - 1);
	uintptr_t mem_end_align = mem_start_align + aligned_size;

	// Compute total memory size including aligned start/end
	size_t mem_size = mem_end_align - mem_next;

	// For heap memory analysis
	if (extmem_analysis_mode_flag == 1) {
		ext_mem_bytes_used += mem_size;
		return (void*)0xFFFF;
	}

	// Only allocate if we have enough memory
	if (ext_mem_bytes_left >= mem_size) {
#if RDSP_MEMORY_UTILS_USES_STDLIB
		mem_start_align = (uintptr_t)ALIGNED_MALLOC(mem_size, align);
#endif // RDSP_MEMORY_UTILS_USES_STDLIB==1

		// Check the start is aligned
		if ((mem_start_align % align) != 0) {
			pRet = NULL; // Not aligned
#if RDSP_ENABLE_PRINTF
			(void)printf("The requested memory is not aligned.\n");
#endif
		}
		else {
			// Cast aligned uintptr_t t to void* after pointer arithmetic
			pRet = (void*)mem_start_align;
			ext_mem_nextptr = (void*)mem_end_align;
			ext_mem_bytes_left -= mem_size;
			ext_mem_bytes_used += mem_size;
		}
	}
	else {
		pRet = NULL;
#if RDSP_ENABLE_PRINTF
		(void)printf("Not enough memory available! Requested %zu bytes, but only %zu bytes available.\n", Asize, ext_mem_bytes_left);
#endif
	}
	return pRet;
#endif // RDSP_ENABLE_MEMCHECK
}

void rdsp_plugin_free(void* Aptr) {
#if RDSP_MEMORY_UTILS_USES_STDLIB
#if _WIN32
	_aligned_free(Aptr);
#else
	free(Aptr);
#endif
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
	// Pointer to the start of the aligned memory section
	void* pRet = NULL;
	size_t align = (size_t)Aalign; // Must cast enum to 32/64 bit int type

	// Ensure next pointer is aligned in case previous malloc had a different alignment
	uintptr_t mem_next = (uintptr_t)ext_mem_scratch_nextptr; // Use uintptr_t for pointer arithmetic
	uintptr_t mem_start_align = (mem_next + (align - 1)) & ~(align - 1);

	// Compute memory size requested including added alignment
	size_t aligned_size = (Asize + (align - 1)) & ~(align - 1);
	uintptr_t mem_end_align = mem_start_align + aligned_size;

	// Compute total memory size including aligned start/end
	size_t mem_size = mem_end_align - mem_next;

	// For heap memory analysis
	if (extmem_analysis_mode_flag == 1) {
		ext_mem_scratch_bytes_used += mem_size;

		// Check if max scratch memory usage has increased
		if (ext_mem_scratch_bytes_used > ext_mem_scratch_max_bytes_used) {
			ext_mem_scratch_max_bytes_used = ext_mem_scratch_bytes_used;
		}

		return (void*)0xFFFF;
	}

	// Only allocate if we have enough memory
	if (ext_mem_scratch_bytes_left >= mem_size) {
#if RDSP_MEMORY_UTILS_USES_STDLIB
		mem_start_align = (uintptr_t)ALIGNED_MALLOC(Asize, align);
#endif // RDSP_MEMORY_UTILS_USES_STDLIB

		// Check the start is aligned
		if ((mem_start_align % align) != 0) {
			pRet = NULL; // Not aligned
#if RDSP_ENABLE_PRINTF
			(void)printf("The requested memory is not aligned.\n");
#endif
		}
		else {
			// Cast aligned uintptr_t t to void* after pointer arithmetic
			pRet = (void*)mem_start_align;
			ext_mem_scratch_nextptr = (void*)mem_end_align;
			ext_mem_scratch_bytes_left -= mem_size;
			ext_mem_scratch_bytes_used += mem_size;

			// Check if max scratch memory usage has increased
			if (ext_mem_scratch_bytes_used > ext_mem_scratch_max_bytes_used) {
				ext_mem_scratch_max_bytes_used = ext_mem_scratch_bytes_used;
			}
		}
	}
	else {
		pRet = NULL;
#if RDSP_ENABLE_PRINTF
		(void)printf("Not enough memory available! Requested %zu scratch bytes but only %zu scratch bytes available.\n", Asize, ext_mem_scratch_bytes_left);
#endif
	}
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
#if defined(HIFI4) || defined(FUSIONDSP)
		Aptr[i] = Aval;
#else
		Aptr[i][0] = Aval[0];
		Aptr[i][1] = Aval[1];
#endif
	}
	return Aptr;
}

void* rdsp_plugin_memcpy(void* Adest, const void* Asrc, size_t Asize) {
#if RDSP_MEMORY_UTILS_USES_STRING
	return memcpy(Adest, Asrc, Asize);
#else
	uint32_t* src32 = (uint32_t*)Asrc;
	uint32_t* dst32 = (uint32_t*)Adest;
	size_t i = 0;
	for (; i < Asize >> 2; i++) {
		*dst32++ = *src32++;
	}

	// Do the rest
	uint8_t* src8 = (uint8_t*)src32;
	uint8_t* dst8 = (uint8_t*)dst32;
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

void* rdsp_plugin_memcpy_float(rdsp_float* Adest, const rdsp_float* Asrc, size_t Alength) {
	return memcpy(Adest, Asrc, Alength * sizeof(rdsp_float));
}

void* rdsp_plugin_memcpy_complex(rdsp_complex* Adest, const rdsp_complex* Asrc, size_t Alength) {
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
	const uint32_t* x32 = (const uint32_t*)Ax;
	const uint32_t* y32 = (const uint32_t*)Ay;
	size_t i = 0;
	for (; i < Asize >> 2; i++) {
		if (*x32++ != *y32++) {
			return 1;
		}
	}

	// Do the rest
	size_t rem = Asize - (i << 2);
	const uint8_t* x8 = (const uint8_t*)x32;
	const uint8_t* y8 = (const uint8_t*)y32;
	while (rem-- > 0) {
		if (*x8++ != *y8++) {
			return 1;
		}
	}
	return 0;
#endif
}
