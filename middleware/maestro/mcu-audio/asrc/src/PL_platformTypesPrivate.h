/*
 * Copyright 2023 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
@file
Header file for the application layer interface of the VIT module.
This files includes all definitions, types, structures and function prototypes
required by the calling layer. All other types, structures and functions are
private.
*/

#ifndef PLATFORMTYPESPRIVATE_H_
#define PLATFORMTYPESPRIVATE_H_

/****************************************************************************************/
/*                                                                                      */
/*  Includes                                                                            */
/*                                                                                      */
/****************************************************************************************/
#include "PL_platformTypes.h"
#include <float.h>
#include <stddef.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>

/****************************************************************************************/
/*                                                                                      */
/*  Definitions                                                                         */
/*                                                                                      */
/****************************************************************************************/

#if defined(PLATFORM_RT1060_CORTEXM7) || defined(PLATFORM_RT1160_CORTEXM7) || defined(PLATFORM_RT1170_CORTEXM7)

#define PLATFORM_CORTEXM7

#define PL_INPUT_IS_RAW_TEMPORAL // input data are raw pcm input

#ifndef ACOUSTIC_MODEL_TF
#define AFE_IS_ON // AFE integrated in Lib
#define USE_NN_FROM_GLOW
#endif
// #define USE_QUANTIZATION
// #define NON_LINEAR_QUANTIZATION
#define SUBSAMPLING

// ARM functions
#define __ARM_FEATURE_DSP 1
// see arm_math.h
// #if (defined (__ARM_FEATURE_DSP) && (__ARM_FEATURE_DSP == 1))
//      #define ARM_MATH_DSP

// Used to check whether Device is the correct one
#define SI_VALIDATION

// RT1060 : SAI1 version ID (VERID) register
#define RT1060_SI_TEST_ADDR   0x40384000
#define RT1060_EXPECTED_VALUE 0x03000000

// RT1160 : SAI1 version ID (VERID) register
#define RT1160_SI_TEST_ADDR   0x40404000
#define RT1160_EXPECTED_VALUE 0x03010000

// RT1170 : SAI version ID (VERID) register
#define RT1170_SI_TEST_ADDR   0x40404000
#define RT1170_EXPECTED_VALUE 0x03010000

#define SI_VALIDATION_VALUE_MASK 0xFFFFFFFF

typedef int32_t queue_int_t;

#elif PLATFORM_CORTEXM7_NOAFE
#define PLATFORM_CORTEXM7

#define PL_INPUT_IS_RAW_TEMPORAL // input data are raw pcm input

// ARM functions
#define __ARM_FEATURE_DSP        1
#define SI_VALIDATION

// RT1040 : SAI1 version ID (VERID) register
#define RT1040_SI_TEST_ADDR   0x40384000
#define RT1040_EXPECTED_VALUE 0x03000000

// RT1050 : SAI1 version ID (VERID) register
// NOTE: TO BE TESTED on HW, when available
#define RT1050_SI_TEST_ADDR   0x40384000
#define RT1050_EXPECTED_VALUE 0x03000000

// RT1060 : SAI1 version ID (VERID) register
#define RT1060_SI_TEST_ADDR   0x40384000
#define RT1060_EXPECTED_VALUE 0x03000000

// RT1160 : SAI1 version ID (VERID) register
#define RT1160_SI_TEST_ADDR   0x40404000
#define RT1160_EXPECTED_VALUE 0x03010000

// RT1170 : SAI version ID (VERID) register
#define RT1170_SI_TEST_ADDR   0x40404000
#define RT1170_EXPECTED_VALUE 0x03010000

typedef int32_t queue_int_t;

#define SI_VALIDATION_VALUE_MASK 0xFFFFFFFF

#elif PLATFORM_CORTEXM33
#define PL_INPUT_IS_RAW_TEMPORAL // input data are raw pcm input

// ARM functions
#define __ARM_FEATURE_DSP        1
typedef int32_t queue_int_t;

#elif defined PLATFORM_HIFI4

#define PL_INPUT_IS_RAW_TEMPORAL // input data are raw pcm input
#define HIFI4_OPTIMIZED

#define USE_NN_FROM_GLOW
// #define USE_QUANTIZATION
// #define NON_LINEAR_QUANTIZATION
#ifndef SIMULATION_HIFI
// Used to check whether Device is the correct one
#define SI_VALIDATION
#endif

#define RT600_SI_TEST_ADDR   0x40002060 // PRODUCT_ID
#define RT600_EXPECTED_VALUE 0x00006850

#define SI_VALIDATION_VALUE_MASK 0xFFF0FFFE

typedef int queue_int_t;

#elif defined PLATFORM_FUSIONF1

#define PL_INPUT_IS_RAW_TEMPORAL // input data are raw pcm input

// Used to check whether Device is correct one
#define SI_VALIDATION

#define RT500_SI_TEST_ADDR   0x40002060 // PRODUCT_ID
#define RT500_EXPECTED_VALUE 0x00005950

#define SI_VALIDATION_VALUE_MASK 0xFFFFFFFE

typedef int queue_int_t;

#elif defined PLATFORM_WINDOWS
// all MACRO defined in PL_platformTypes_windows.h file

#elif defined PLATFORM_LINUX
// all MACRO defined in PL_platformTypes_linux.h file

#if UINTPTR_MAX == 0xffffffff
/* 32-bit */
typedef int32_t queue_int_t;

#elif UINTPTR_MAX == 0xffffffffffffffff
/* 64-bit */
typedef int64_t queue_int_t;
#endif
#elif defined PLATFORM_AARCH64
// all MACRO defined in PL_platformTypes_linux.h file
#define SI_VALIDATION
#define IMX8A53_SI_TEST_ADDR        "/sys/devices/soc0/soc_id"
#define IMX8MINIA53_EXPECTED_VALUE  "i.MX8MM"
#define IMX8MPLUSA53_EXPECTED_VALUE "i.MX8MP"
#define IMX8QMA53_EXPECTED_VALUE    "i.MX8QM"
#define IMX9A55_SI_TEST_ADDR        IMX8A53_SI_TEST_ADDR
#define IMX93A55_EXPECTED_VALUE     "i.MX93"

#define PL_INPUT_IS_RAW_TEMPORAL // input data are raw pcm input
#if UINTPTR_MAX == 0xffffffff
/* 32-bit */
typedef int32_t queue_int_t;

#elif UINTPTR_MAX == 0xffffffffffffffff
/* 64-bit */
typedef int64_t queue_int_t;
#endif
#elif defined PLATFORM_IMX8MINI_CORTEXM4
// all MACRO defined in PL_platformTypes_linux.h file
#define PL_INPUT_IS_RAW_TEMPORAL // input data are raw pcm input
// Used to check whether Device is correct one
#define SI_VALIDATION

// Check DIGPROG register
// DIGPROG_MAJOR_UPPER : Bit[7:4] is 0x8, stands for “i.MX8” Bit[3:0] is 0x2,
// stands for ”M”  #define IMX8MINIM4_SI_TEST_ADDR     0x30360800
#define IMX8MINIM4_SI_TEST_ADDR   0x30360800
#define IMX8MINIM4_EXPECTED_VALUE 0x00820000

#define SI_VALIDATION_VALUE_MASK 0x00FF0000

typedef int32_t queue_int_t;
#elif defined PLATFORM_IMX8PLUS_CORTEXM7
// all MACRO defined in PL_platformTypes_linux.h file
#define PL_INPUT_IS_RAW_TEMPORAL // input data are raw pcm input
// Used to check whether Device is correct one

typedef int32_t queue_int_t;

#define SI_VALIDATION

// Check DIGPROG register
// DIGPROG_MAJOR_UPPER : Bit[7:4] is 0x8, stands for “i.MX8” Bit[3:0] is 0x2,
// stands for ”M”  #define IMX8MINIM4_SI_TEST_ADDR     0x30360800
#define IMX8PLUSM7_SI_TEST_ADDR   0x30360800
#define IMX8PLUSM7_EXPECTED_VALUE 0x00820000

#define SI_VALIDATION_VALUE_MASK 0x00FF0000

#else
#error "NO PLATFORM selected (between WINDOWS, LINUX, HIFI4 or CORTEXM7)"
#endif

// Put here to ensure "#define PL_ITST_FUNC" will have an impact
#if !defined(VIT_MODEL_GENERATION)
#include "PL_ITST.h"
#endif

#define PL_MAXINT_8   127         ///< Maximum positive integer size
#define PL_MAXINT_16  32767       ///< Maximum signed int 16 bits number
#define PL_MAXUINT_16 65535U      ///< Maximum un-signed int 16 bits number
#define PL_MAXINT_32  2147483647  ///< Maximum signed int 32 bits number
#define PL_MAXUINT_32 4294967295U ///< Maximum un-signed int 32 bits number
#define PL_MININT_32  0x80000000  ///< Minimum signed int 32 bit number in 2's complement form
#define PL_EPSFLT     FLT_MIN
#define PL_MAXFLOAT   FLT_MAX

typedef enum
{
    PL_MODE_OFF,
    PL_MODE_ON,
    PL_MODE_en_DUMMY = PL_MAXENUM
} PL_MODE_en;

// Memory  access
#define PL_POINTER_SIZE sizeof(PL_UINTPTR)

#ifdef AFE_IS_ON
#define VIT_MAX_NUMBER_OF_CHANNEL_PRIVATE _3CHAN
#else
#ifndef PL_DEBUG_FILE_FFT_DUMP
#define VIT_MAX_NUMBER_OF_CHANNEL_PRIVATE _1CHAN
#else
// Required to dump FFT output
#define VIT_MAX_NUMBER_OF_CHANNEL_PRIVATE _3CHAN
#endif
#endif

// VCE Scheduler
enum
{
    VCE_SCHEDULE_PART0 = 0,
    VCE_SCHEDULE_PART1,
    VCE_SCHEDULE_PART2,
    VCE_SCHEDULE_PART3,
    VCE_SCHEDULE_PART0_PART1,
    VCE_SCHEDULE_PART0_PART2,
    VCE_SCHEDULE_ALL
};

/*
This enum type specifies the different error codes returned by the API functions
For the exact meaning see the individual function descriptions
*/
typedef enum
{
    PL_SUCCESS = 0, ///< Successful return from a routine      //--- WARNING MUST BE THE SAME THAN VIT SUCCESS ---
    PL_INVALID_BUFFER_MEMORY_ALIGNMENT = 1,  ///< Memory alignment error
    PL_INVALID_NULLADDRESS             = 2,  ///< NULL allocation address
    PL_INVALID_ARGUMENT                = 3,  ///< Error in function arguments
    PL_INVALID_PARAMETER_OUTOFRANGE    = 4,  ///< Out of range parameter
    PL_INVALID_SAMPLE_RATE             = 5,  ///< Sample rate not supported
    PL_INVALID_FRAME_SIZE              = 6,  ///< Frame size not supported
    PL_INVALID_MICROPHONE_NUMBER       = 7,  ///< Microphone number not supported
    PL_INVALID_MICROPHONE_TOPOLOGY     = 8,  ///< Microphone topology not supported
    PL_INVALID_ALGORITHM_CONFIGURATION = 9,  ///< Mutually exclusive algorithms are on
    PL_INVALID_MODEL                   = 10, ///< Model Group is not supported
    PL_INVALID_MODEL_GROUP             = 11, ///< Model Group is not supported
    PL_INVALID_MODEL_NAME              = 12, ///< Model Name is not supported
    PL_INVALID_STATE                   = 13, ///< State machine error
    PL_CLASSIFIER_NOT_RUN              = 14, ///< Classifier not run
    PL_ERROR                           = 15, ///< Other errors
    PL_HCLG_NOT_ENOUGH_TOKEN           = 16,
    PL_HCLG_NOT_ENOUGH_FWDLNK          = 17,
    PL_HCLG_NOT_ENOUGH_SCRATCHARRAY    = 18,
    PL_HCLG_NOT_ENOUGH_ACTIVETOKS      = 19,
    PL_INVALID_DEVICE                  = 20,
    PL_INVALID_API_VERSION             = 21,
    PL_DUMMY_ERROR                     = PL_MAXENUM
} PL_ReturnStatus_en;

// Keep like this - Not enum
#define ERROR_LANGUAGE 0
#define ENGLISH        1
#define MANDARIN       2
#define TURKISH        3
#define GERMAN         4
#define SPANISH        5
#define JAPANESE       6
#define KOREAN         7
#define ITALIAN        8
#define FRENCH         9
#define DUMMY_LANGUAGE PL_MAXENUM

typedef struct
{
    PL_UINT16 numRows; /**< number of rows of the matrix.     */
    PL_UINT16 numCols; /**< number of columns of the matrix.  */
    PL_FLOAT *pData;   /**< points to the data of the matrix. */
} Matrix_instance_f32;

typedef struct
{
    PL_UINT16 numRows; /**< number of rows of the matrix.     */
    PL_UINT16 numCols; /**< number of columns of the matrix.  */
    PL_INT16 *pData;   /**< points to the data of the matrix. */
} Matrix_instance_i16;

typedef struct
{
    PL_UINT16 numRows; /**< number of rows of the matrix.     */
    PL_UINT16 numCols; /**< number of columns of the matrix.  */
    PL_INT32 *pData;   /**< points to the data of the matrix. */
} Matrix_instance_i32;

#ifdef MODEL_16BIT
#define MATRIX_INSTANCE Matrix_instance_i16
#else
#define MATRIX_INSTANCE Matrix_instance_f32
#endif

// useful in case of Model layer allocation is controlled by user
#define MEMORY_LOCATION_FLEXIBLE (PL_MEMREGION_ROM + 1)

/*   UseFul MACRO */
#define FIELD_SIZEOF_IN_BYTE(struct_name, field)  (sizeof(((struct_name *)0)->field))
#define FIELD_SIZEOF_IN_FLOAT(struct_name, field) ((sizeof(((struct_name *)0)->field)) / sizeof(PL_FLOAT))
#define IS_ALIGNED(POINTER, BYTE_COUNT)           (((PL_UINTPTR)(const void *)(POINTER)) % (BYTE_COUNT) == 0)

#define INT16_TO_FLOAT(x) ((PL_FLOAT)x)
#define INT32_TO_FLOAT(x) ((PL_FLOAT)x)
#define INT64_TO_FLOAT(x) ((PL_FLOAT)x)

#define FLOAT_TO_INT16(x) ((PL_INT16)x)
#define FLOAT_TO_INT32(x) ((PL_INT32)x)
#define FLOAT_TO_INT64(x) ((PL_INT64)x)

/*   Useful Functions */
#if !defined(VIT_MODEL_GENERATION)
#include "PL_MEM_Copy.h"
#endif
PL_ReturnStatus_en PL_MatxVec_Bias(const Matrix_instance_f32 *Mat,
                                   const Matrix_instance_f32 *Vect,
                                   const Matrix_instance_f32 *Bias,
                                   Matrix_instance_f32 *y);

PL_ReturnStatus_en PL_MatxVec_Bias_FXP(const Matrix_instance_i16 *Mat,
                                       const Matrix_instance_i16 *Vect,
                                       const Matrix_instance_i32 *Bias,
                                       Matrix_instance_i32 *y,
                                       PL_UINT16 AccShift);

#ifdef MODEL_16BIT
#define MATxVEC_BIAS PL_MatxVec_Bias_FXP
#else
#define MATxVEC_BIAS PL_MatxVec_Bias
#endif

PL_ReturnStatus_en PL_Mat_Trans(const Matrix_instance_f32 *in, Matrix_instance_f32 *out);

#endif /* PLATFORMTYPESPRIVATE_H_ */

// check define condition error
#if (defined PL_INPUT_IS_RAW_TEMPORAL) && (defined PL_INPUT_IS_RAW_SPECTRUM)
#error "input selection error: PL_INPUT_IS_RAW_TEMPORAL and PL_INPUT_IS_RAW_SPECTRUM can't be defined at same time"
#endif
#if (defined PL_INPUT_IS_RAW_TEMPORAL) && (defined PL_INPUT_IS_AFE_SPECTRUM)
#error "input selection error: PL_INPUT_IS_RAW_TEMPORAL and PL_INPUT_IS_AFE_SPECTRUM can't be defined at same time"
#endif
#if (defined PL_INPUT_IS_RAW_SPECTRUM) && (defined PL_INPUT_IS_AFE_SPECTRUM)
#error "input selection error: PL_INPUT_IS_RAW_SPECTRUM and PL_INPUT_IS_AFE_SPECTRUM can't be defined at same time"
#endif
#if (defined PL_PROCESS_KALDI_COMPATIBLE) && (!(defined PL_INPUT_IS_RAW_TEMPORAL))
#error "input selection error: PL_PROCESS_KALDI_COMPATIBLE requires PL_INPUT_IS_RAW_TEMPORAL definition"
#endif
#if (defined PL_ITST_FUNC) && !((defined PL_DEBUG_FILE) || (defined PL_DEBUG_FILE_REDUCE))
// #error "input selection error: PL_ITST_FUNC requires PL_DEBUG_FILE or PL_DEBUG_FILE_REDUCE definition"
#endif
#if (defined PL_ITST_MONITOR) && !(defined PL_ITST_FUNC)
#error "input selection error: PL_ITST_NORM requires PL_ITST_FUNC definition"
#endif

/* End of file */
