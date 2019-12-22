/**
 * @file tst_sm_time.h
 * @author NXP Semiconductors
 * @version 1.0
 * @par LICENSE
 * Copyright 2016 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 *
 * @par Description
 * (APDU) Execution time measurement utility API
 *
 *****************************************************************************/
#ifndef _TST_SM_TIME_H_
#define _TST_SM_TIME_H_

#include <time.h>
#include "sm_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32) && !defined(__MINGW64__)
#include <Windows.h>
#if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_APP)
#include <WinSock2.h>
#endif


typedef struct {
    struct timeval tStart;
    struct timeval tEnd;
} axTimeMeasurement_t;

#elif AX_EMBEDDED
/**
 * Utility structure enabling delta time measurement.
 */
typedef struct {
    long long tStart; //!< To contain start of time measurement
    long long tEnd;   //!< To contain end of time measurement
} axTimeMeasurement_t;
#else
/**
 * Utility structure enabling delta time measurement.
 */
typedef struct {
    struct timespec tStart; //!< To contain start of time measurement
    struct timespec tEnd;   //!< To contain end of time measurement
} axTimeMeasurement_t;
#endif

#if !defined(TGT_A71CH) && !defined(TGT_A71CL)
typedef enum axExecTimeEval
{
    AX_TIME_EVAL_IGNORE   = 0x01,
    AX_TIME_EVAL_WARNING  = 0x02,
    AX_TIME_EVAL_FATAL    = 0x03
} axExecTimeEval_t;
#endif // !defined(TGT_A71CH) && !defined(TGT_A71CL)

#define AX_MEASURE_REPORT_VERBOSE     0x01 //!< Report on every measurement
#define AX_MEASURE_ECHO_STDOUT        0x02 //!< Also echo to stdout

/// @cond
#define AX_MEASURE_REPORT_MASK        0x01
#define AX_MEASURE_ECHO_MASK          0x02
/// @endcond

void initMeasurement(axTimeMeasurement_t *mPair);
void concludeMeasurement(axTimeMeasurement_t *mPair);
long getMeasurement(axTimeMeasurement_t *mPair);
#ifdef FTR_FILE_SYSTEM
void axSummarizeMeasurement(FILE *fHandle, char *szMessage, long *msArray, int nMeasurement, int reportMode);
#else
void axSummarizeMeasurement(void *fHandle, char *szMessage, long *msArray, int nMeasurement, int reportMode);
#endif

#if !defined(TGT_A71CH) && !defined(TGT_A71CL)
int evalMeasurement(char *szMessage, long measured, long lowerBound, long higherBound, axExecTimeEval_t severity);
#endif // !defined(TGT_A71CH) && !defined(TGT_A71CL)

#ifdef __cplusplus
}
#endif
#endif // _TST_SM_TIME_H_
