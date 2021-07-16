/**
 * @file tst_sm_time_kinetis.c
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 *
 * Copyright 2017,2018 NXP
 * SPDX-License-Identifier: Apache-2.0
 *
 * @par Description
 * Utility functions for measurement of test execution time
 *
 */

#include "tst_sm_time.h"
#include <stdio.h>

extern volatile uint32_t gtimer_kinetis_msticks; // counter for 1ms SysTicks

long getMeasurement(
    axTimeMeasurement_t *mPair)
{
    if (mPair->tEnd >= mPair->tStart)
        return mPair->tEnd - mPair->tStart;
    else
        return 0;
}

void initMeasurement(
    axTimeMeasurement_t *mPair)
{
    mPair->tEnd = mPair->tStart = gtimer_kinetis_msticks;
}

void concludeMeasurement(
    axTimeMeasurement_t *mPair)
{
    mPair->tEnd = gtimer_kinetis_msticks;
}


void axSummarizeMeasurement(
    void *fHandle_NotUsed, char *szMessage, long *msArray, int nMeasurement, int reportMode)
{
    int i;
    long averaged = 0;
    long minValue = 0;
    long maxValue = 0;
    //int fEchoStdout = 0;
    //int fReportVerbose = 0;
    //int nOut;

    //fEchoStdout = ((reportMode & AX_MEASURE_ECHO_MASK) == AX_MEASURE_ECHO_STDOUT);
    //fReportVerbose = ((reportMode & AX_MEASURE_REPORT_MASK) == AX_MEASURE_REPORT_VERBOSE);

    if (nMeasurement > 0)
    {
        minValue = msArray[0];
        maxValue = msArray[0];
    }
    else
    {
        printf("%s: No valid amount of measurements (%d)\n", szMessage, nMeasurement);
        return;
    }

    for (i=0; i<nMeasurement; i++)
    {
        //printf("%s: %ld ms\n", szMessage, msArray[i]);
        averaged += msArray[i];
        minValue = (msArray[i] < minValue) ? msArray[i] : minValue;
        maxValue = (msArray[i] > maxValue) ? msArray[i] : maxValue;
    }
    averaged /= nMeasurement;

    printf("Exec Time: %s:\n\tAverage (%d measurements): %ld ms\n", szMessage, nMeasurement, averaged);
    printf("\tMinimum: %ld ms\n", minValue);
    printf("\tMaximum: %ld ms\n", maxValue);
    return;
}
