/*
Copyright 2021 by Retune DSP
Copyright 2022 NXP

NXP Confidential. This software is owned or controlled by NXP and may only be used strictly in accordance with the applicable license terms.  By expressly accepting such terms or by downloading, installing, activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef RDSP_VS_UTILITIES_H
#define RDSP_VS_UTILITIES_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#if defined(SIMULATED) || defined(HIFI3) || defined(HIFI4) || defined(FUSIONF1)
#include "stdio.h"
#endif

#ifdef HEMILITE
#include "DeltaPlatform.h"
#include "DeltaTypes.h"
#include "TIE_defs.h"
#include "TIE_include.h"
#include "hemilite_common_tie.h"
#include "hemilite_fr32_ops.h"
#include "hemilite_fr32_utils.h"
#include "hemilite_fr32_math.h"
#include "CoreDSP.h"
#include "Biquad.h"
#elif defined(HMD)
#include "hemidelta_common_tie.h"
#include "fr32_math.h"
#include "fr32_utils.h"
#include "fr32_xcc_ops.h"
#include "TIE_defs.h"
#include "TIE_include.h"
#include "CoreLib.h"
#elif defined(HMD1A)
#include "hmd1a_common_tie.h"
#include "fr32_math.h"
#include "fr32_utils.h"
#include "fr32_xcc_ops.h"
#include "TIE_defs.h"
#include "TIE_include.h"
#include "CoreLib.h"
#elif defined(HIFI4)
#include <xtensa/tie/xt_hifi4.h>
#include "NatureDSP_Signal.h"
#include "NatureDSP_types.h"
#elif defined(HIFI3)
#include <xtensa/tie/xt_hifi3.h>
#include "NatureDSP_Signal_math.h"
#include "NatureDSP_Signal.h"
#include "NatureDSP_types.h"
#elif defined(FUSIONF1)
#include <xtensa/tie/xt_fusion.h>
#include "NatureDSP_Signal.h"
#include "NatureDSP_types.h"
#elif defined(LPDSP32)
#include "lpdsp32_math.h"
#endif


#if defined(__cplusplus) && !defined(CPP_LINKAGE)
extern "C" {
#endif

#if defined(HEMILITE) || defined(HMD) || defined(HMD1A)

extern const int32_t mag2db_const;
extern const int32_t pow2db_const;

#define ALGO_EX1_ID             (0xAE)   // Retune ID
#ifdef HEMILITE
typedef enum BAFMemAlign_s {
    BAF_MEM_ALIGN_1 = 1, /*!< Align 1 byte */
    BAF_MEM_ALIGN_2 = 2, /*!< Align 2 byte */
    BAF_MEM_ALIGN_4 = 4, /*!< Align 4 byte */
    BAF_MEM_ALIGN_8 = 8, /*!< Align 8 byte */
}BAFMemAlign_t;
typedef enum BAFMemType_s {
    BAF_MEM_TYPE_PERSIST = 0, /*!< Persistent Memory */
    BAF_MEM_TYPE_SCRATCH_INVALID, /*!< Scratch Memory */
    BAF_MEM_TYPE_STAGING, /*!< Staging Memory */
    BAF_MEM_TYPE_MAX /*!< Memory type max */
}BAFMemType_t;

#ifndef SIMULATED
typedef void *(*tBAFMalloc)(unsigned char algoID, unsigned int size, BAFMemAlign_t align, BAFMemType_t memType);
typedef void *(*tBAFMemSet)(unsigned char algoID, void *ptr, int32_t value, uint32_t num);
typedef void *(*tBAFMemCpy)(unsigned char algoID, void *pDst, const void *pSrc, uint32_t num);
extern tBAFMalloc BAF_pMalloc;
extern tBAFMemSet BAF_pMemSet;
extern tBAFMemCpy BAF_pMemCpy;
#endif  // SIMULATED
#elif defined(HMD) || defined(HMD1A)
//typedef enum {
//    MEM_ALIGN_1 = 1, /*!< Align 1 byte */
//    MEM_ALIGN_2 = 2, /*!< Align 2 byte */
//    MEM_ALIGN_4 = 4, /*!< Align 4 byte */
//    MEM_ALIGN_8 = 8, /*!< Align 8 byte */
//    MEM_ALIGN_16 = 16, /*!< Align 16 byte */
//}MemAlign_t;

extern void* rdsp_plugin_malloc(uint32_t Asize, int32_t Aalignment) __attribute__((weak));    // __attribute__((weak)) allows external replacement of the function
extern void* rdsp_plugin_memset(void* Aptr, uint8_t Aval, uint32_t Asize) __attribute__((weak));    // __attribute__((weak)) allows external replacement of the function
extern void* rdsp_plugin_memcpy(void* Adest, void* Asrc, uint32_t Asize) __attribute__((weak));    // __attribute__((weak)) allows external replacement of the function
#endif

extern void convert_q15_to_float(float* y, int16_t* x, int length);
extern void convert_float_to_afloat(fr32* y, float* x, int length);
extern void convert_afloat_to_float(float* y, fr32* x, int length);

#if !defined(RDSP_DISABLE_FILEIO)
#include "stdio.h"  // For file functions
typedef struct rdsp_file_s {
    FILE* rdspFile; // USB file for fopen/fwrite
}rdsp_file_t;
extern rdsp_file_t* rdsp_fopen(char* filename, char* mode);
extern int32_t rdsp_fclose(rdsp_file_t* stream);
extern int32_t rdsp_fread(void* ptr, int32_t size, int32_t count, rdsp_file_t* stream);
extern int32_t rdsp_fwrite(const void* ptr, int32_t size, int32_t count, rdsp_file_t* stream);
extern int rdsp_fseek(rdsp_file_t* stream, long int offset, int origin);
extern long int rdsp_ftell(rdsp_file_t* stream);
extern void rdsp_rewind(rdsp_file_t* stream);

extern void fwrite_hemilite(fr32* x, int length, FILE* fid);
#endif // !defined(RDSP_DISABLE_FILEIO)

extern void* rdsp_memset(void* Aaddress, uint8_t Aval, int32_t Asize) __attribute__((weak));    // __attribute__((weak)) allows external replacement of the function
extern void rdsp_memcpy(void* Adest, void* Asrc, int32_t Azize) __attribute__((weak));  // __attribute__((weak)) allows external replacement of the function
extern void* rdsp_malloc(uint32_t Asize) __attribute__((weak));  // __attribute__((weak)) allows external replacement of the function
extern void* rdsp_malloc_align(uint32_t Asize, int32_t alignment) __attribute__((weak));    // __attribute__((weak)) allows external replacement of the function
extern void rdsp_free(void* data) __attribute__((weak));    // __attribute__((weak)) allows external replacement of the function
extern void rdsp_free_align(void *data) __attribute__((weak));  // __attribute__((weak)) allows external replacement of the function

#elif defined(HIFI3) || defined(HIFI4) || defined(FUSIONF1)

#define M_PIf 3.14159265358979323846f  /* pi */
#define LOG2_to_DB_SCALE 770 //round(3.01)*2^8
#define FLOAT2INT32(x) ((int32_t)(x * MAX_INT32))
#define FLOAT2INT16(x) ((int16_t)(x * MAX_INT16))
#define FLOAT2DB(x) ((int32_t)(x * 32768))
#define IIR1LAMBDA(tau, fs) FLOAT2INT32((float) (1.0f - expf(-1.0f / (fs * tau))))

extern int32_t f32_to_dB(int32_t x);
extern int32_t recip_dB(int32_t x_dB);

extern void* rdsp_malloc(uint32_t Asize);
extern void* rdsp_malloc_align(uint32_t Asize, int32_t alignment);
extern void* rdsp_memset(void* ptr, int32_t value, int32_t num);
extern void* rdsp_memcpy(void* Adest, void* Asrc, int32_t Azize);
extern void rdsp_free(void* data);
extern void rdsp_free_align(void* data);

#if !defined(RDSP_DISABLE_FILEIO)	// File system is not supported on all platforms
#include "stdio.h"  // For file functions
extern void fwrite_f32(ae_f32* x, int length, FILE* Afid);
extern void fwrite_f64(int64_t* x, int length, FILE* Afid);
typedef struct rdsp_file_s {
    FILE* rdspFile; // USB file for fopen/fwrite
}rdsp_file_t;
extern rdsp_file_t* rdsp_fopen(char* filename, char* mode);
extern int32_t rdsp_fclose(rdsp_file_t* stream);
extern int32_t rdsp_fread(void* ptr, int32_t size, int32_t count, rdsp_file_t* stream);
extern int32_t rdsp_fwrite(void* ptr, int32_t size, int32_t count, rdsp_file_t* stream);
extern int rdsp_fseek(rdsp_file_t* stream, long int offset, int origin);
extern long int rdsp_ftell(rdsp_file_t* stream);
extern void rdsp_rewind(rdsp_file_t* stream);
#endif

#elif defined(ARM_CM4)

extern void* rdsp_malloc(size_t Asize) __attribute__((weak));	// __attribute__((weak)) allows external replacement of the function
extern void* rdsp_malloc_align(size_t Asize, int32_t alignment) __attribute__((weak));	// __attribute__((weak)) allows external replacement of the function
extern void rdsp_free(void* data) __attribute__((weak));	// __attribute__((weak)) allows external replacement of the function
extern void rdsp_free_align(void* data) __attribute__((weak));	// __attribute__((weak)) allows external replacement of the function
extern void* rdsp_memset(void* ptr, int value, size_t num) __attribute__((weak));   // __attribute__((weak)) allows external replacement of the function
extern void* rdsp_memcpy(void* Adest, void* Asrc, size_t Azize) __attribute__((weak));   // __attribute__((weak)) allows external replacement of the function
extern int rdsp_strcmp(char* str1, char* str2) __attribute__((weak));               // __attribute__((weak)) allows external replacement of the function

// File functions
#if !defined(RDSP_DISABLE_FILEIO)	// File system is not supported on all platforms
#include "fatfs.h"
typedef struct rdsp_file_s {
    FIL rdspFile; // USB file for fopen/fwrite
}rdsp_file_t;
extern rdsp_file_t* rdsp_fopen(char* filename, char* mode);
extern int32_t rdsp_fclose(rdsp_file_t* stream);
extern int32_t rdsp_fread(void* ptr, int32_t size, int32_t count, rdsp_file_t* stream);
extern int32_t rdsp_fwrite(void* ptr, int32_t size, int32_t count, rdsp_file_t* stream);
extern int rdsp_fseek(rdsp_file_t* stream, long int offset, int origin);
extern long int rdsp_ftell(rdsp_file_t* stream);
extern void rdsp_rewind(rdsp_file_t* stream);
#endif	//!defined(RDSP_DISABLE_FILEIO)

// Cycle counting
/* DWT (Data Watchpoint and Trace) registers, only exists on ARM Cortex with a DWT unit */
#define RDSP_DWT_CONTROL             (*((volatile uint32_t*)0xE0001000))  /*!< DWT Control register */
#define RDSP_DWT_CYCCNTENA_BIT       (1UL<<0)                             /*!< CYCCNTENA bit in DWT_CONTROL register */
#define RDSP_DWT_CYCCNT              (*((volatile uint32_t*)0xE0001004))  /*!< DWT Cycle Counter register */
#define RDSP_DEMCR                   (*((volatile uint32_t*)0xE000EDFC))  /*!< DEMCR: Debug Exception and Monitor Control Register */
#define RDSP_TRCENA_BIT              (1UL<<24)                            /*!< Trace enable bit in DEMCR register */

#define RDSP_InitCycleCounter() \
RDSP_DEMCR |= RDSP_TRCENA_BIT     /*!< TRCENA: Enable trace and debug block DEMCR (Debug Exception and Monitor Control Register */

#define RDSP_ResetCycleCounter() \
RDSP_DWT_CYCCNT = 0               /*!< Reset cycle counter */

#define RDSP_EnableCycleCounter() \
RDSP_DWT_CONTROL |= RDSP_DWT_CYCCNTENA_BIT    /*!< Enable cycle counter */

#define RDSP_DisableCycleCounter() \
RDSP_DWT_CONTROL &= ~RDSP_DWT_CYCCNTENA_BIT   /*!< Disable cycle counter */

#define RDSP_GetCycleCounter() \
RDSP_DWT_CYCCNT                   /*!< Read cycle counter register */

extern const float mag2db_const;
#elif defined(LPDSP32)

#define FLOAT2DB(x) ((int32_t)(x * 32768))
#define FLOAT2INT32(x) (as_int(x))

void* rdsp_plugin_malloc(uint32_t size, int32_t alignment);
void* rdsp_plugin_memset(void* Aptr, uint8_t Aval, uint32_t Asize);
void* rdsp_plugin_memcpy(void* Adest, void* Asrc, uint32_t Asize);
void rdsp_plugin_free(void* Aptr);

extern void* rdsp_malloc(size_t Asize);
extern void* rdsp_malloc_align(size_t Asize, int32_t alignment);
extern void rdsp_free(void* data);
extern void rdsp_free_align(void* data);
extern void* rdsp_memset(void* ptr, int value, size_t num);
extern void* rdsp_memcpy(void* Adest, void* Asrc, size_t Azize);
extern int rdsp_strcmp(char* str1, char* str2);

// File functions
#if !defined(RDSP_DISABLE_FILEIO)	// File system is not supported on all platforms
#include "stdio.h"  // For file functions
typedef struct rdsp_file_s {
    FILE* rdspFile; // File for fopen/fwrite
}rdsp_file_t;
extern rdsp_file_t* rdsp_fopen(const char* filename, const char* mode);
extern int32_t rdsp_fclose(rdsp_file_t* stream);
extern int32_t rdsp_fread(void* ptr, int32_t size, int32_t count, rdsp_file_t* stream);
extern int32_t rdsp_fwrite(void* ptr, int32_t size, int32_t count, rdsp_file_t* stream);
extern void fwrite_f32(int32_t* x, int32_t length, rdsp_file_t* stream);
extern int rdsp_fseek(rdsp_file_t* stream, long int offset, int origin);
extern long int rdsp_ftell(rdsp_file_t* stream);
extern void rdsp_rewind(rdsp_file_t* stream);
#endif	//!defined(RDSP_DISABLE_FILEIO)

extern const float mag2db_const;

#else

// ANSI C floating-point implementation
#ifdef ENABLE_WEAK_SYMBOLS
#define RDSP_WEAK_SYMBOLS __attribute__((weak))     // __attribute__((weak)) allows external replacement of the function, but may not be supported on all platforms, e.g. Cygwin
#else
#define RDSP_WEAK_SYMBOLS
#endif

extern void* rdsp_malloc(size_t Asize) RDSP_WEAK_SYMBOLS;	// __attribute__((weak)) allows external replacement of the function
extern void* rdsp_malloc_align(size_t Asize, int32_t alignment) RDSP_WEAK_SYMBOLS;	// __attribute__((weak)) allows external replacement of the function
extern void rdsp_free(void* data) RDSP_WEAK_SYMBOLS;	// __attribute__((weak)) allows external replacement of the function
extern void rdsp_free_align(void* data) RDSP_WEAK_SYMBOLS;	// __attribute__((weak)) allows external replacement of the function
extern void* rdsp_memset(void* ptr, int value, size_t num) RDSP_WEAK_SYMBOLS;	// __attribute__((weak)) allows external replacement of the function
extern void* rdsp_memcpy(void* Adest, void* Asrc, size_t Azize) RDSP_WEAK_SYMBOLS;	// __attribute__((weak)) allows external replacement of the function
extern int rdsp_strcmp(char* str1, char* str2) RDSP_WEAK_SYMBOLS;	// __attribute__((weak)) allows external replacement of the function*/

// File functions
#if !defined(RDSP_DISABLE_FILEIO)	// File system is not supported on all platforms
#include "stdio.h"  // For file functions
typedef struct rdsp_file_s {
	FILE* rdspFile; // File for fopen/fwrite
}rdsp_file_t;
extern rdsp_file_t* rdsp_fopen(char* filename, char* mode);
extern int32_t rdsp_fclose(rdsp_file_t* stream);
extern int32_t rdsp_fread(void* ptr, int32_t size, int32_t count, rdsp_file_t* stream);
extern int32_t rdsp_fwrite(void* ptr, int32_t size, int32_t count, rdsp_file_t* stream);
extern int rdsp_fseek(rdsp_file_t* stream, long int offset, int origin);
extern long int rdsp_ftell(rdsp_file_t* stream);
extern void rdsp_rewind(rdsp_file_t* stream);
#endif	//!defined(RDSP_DISABLE_FILEIO)

extern const float mag2db_const;

#endif  // defined(HIFI3) || defined(HIFI4) || defined(FUSIONF1)
// Cycle counting function definitions
extern void rdsp_init_cycle_counter();
extern uint32_t rdsp_read_cycle_counter();
#if defined(__cplusplus) && !defined(CPP_LINKAGE)
    }
#endif

#endif // RDSP_VS_UTILITIES_H
