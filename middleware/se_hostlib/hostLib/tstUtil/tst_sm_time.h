/**
 * @file tst_sm_time.h
 * @author NXP Semiconductors
 * @version 1.0
 * @par LICENSE
 * Copyright(C) NXP Semiconductors, 2016
 * All rights reserved.
 *
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * A7-series security ICs.  This software is supplied "AS IS" without any
 * warranties of any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * Permission to use, copy and modify this software is hereby granted,
 * under NXP Semiconductors' and its licensor's relevant copyrights in
 * the software, without fee, provided that it is used in conjunction with
 * NXP Semiconductors products. This copyright, permission, and disclaimer notice
 * must appear in all copies of this code.
 * @par DESCRIPTION
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

#ifdef _WIN32
#include <Windows.h>
typedef struct {
    struct timeval tStart;
    struct timeval tEnd;
} axTimeMeasurement_t;

#elif defined(FREEDOM) || defined (__USE_CMSIS)
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

#ifndef TGT_A71CH
typedef enum axExecTimeEval
{
    AX_TIME_EVAL_IGNORE   = 0x01,
    AX_TIME_EVAL_WARNING  = 0x02,
    AX_TIME_EVAL_FATAL    = 0x03
} axExecTimeEval_t;
#endif // TGT_A71CH

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

#ifndef TGT_A71CH
int evalMeasurement(char *szMessage, long measured, long lowerBound, long higherBound, axExecTimeEval_t severity);
#endif // TGT_A71CH

#ifdef __cplusplus
}
#endif
#endif // _TST_SM_TIME_H_
