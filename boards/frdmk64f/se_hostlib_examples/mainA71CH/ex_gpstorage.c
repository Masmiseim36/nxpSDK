/**
 * @file ex_gpstorage.c
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
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
 * @par Description
 * Example invocations of general purpose storage related functionality of the A71CH
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// #include "a70cm_configuration.h"
#include "a71ch_ex.h"
#include "ax_util.h"
#include "a71_debug.h"
#include "sm_types.h"
#include "sm_apdu.h"
#include "tst_sm_util.h"
#include "tst_a71ch_util.h"

#define EX_RND_DATA         0x00 //!< Bit position 0 decides on random or incrementing data payload
#define EX_INC_DATA         0x01 //!< Bit position 0 decides on random or incrementing data payload
#define EX_ALL_PACKET_SIZES 0x02 //!< Bit position 1 decides on all or a selection of packetsizes

/******************************************************************************
* test
*****************************************************************************/
static U8 exGpStoragePacketSize(U8 initMode, U8 tstMode);
static U8 exGpStorageFreeze(U8 initMode, U8 tstMode);
static U8 exMonotonicCounter(U8 initMode);

/**
 * Demonstrate general purpose storage functionality
 * - ::exGpStoragePacketSize
 * - ::exGpStorageFreeze
 *
 * Demonstrate monotonic counter usage
 * - ::exMonotonicCounter
 */
U8 exGPStorage(U8 tstMode)
{
    U8 result = 1;
    printf( "\r\n-----------\r\nStart exGPStorage(%s)\r\n------------\r\n",
        ((tstMode & EXTENDED_TEST) == EXTENDED_TEST) ? "Extended Test" : "Fast Test");

    // No channel encryption
    DEV_ClearChannelState();

    result &= exMonotonicCounter(INIT_MODE_RESET);

    result &= exGpStoragePacketSize(INIT_MODE_RESET, EX_RND_DATA);

    result &= exGpStoragePacketSize(INIT_MODE_NO_RESET, EX_INC_DATA);

    if ((tstMode & EXTENDED_TEST) == EXTENDED_TEST)
    {
        result &= exGpStoragePacketSize(INIT_MODE_NO_RESET, EX_RND_DATA|EX_ALL_PACKET_SIZES);
    }

    result &= exGpStorageFreeze(INIT_MODE_RESET, EX_INC_DATA);

    // Use channel encryption
    result &= exMonotonicCounter(INIT_MODE_RESET_DO_SCP03);
    result &= exGpStoragePacketSize(INIT_MODE_NO_RESET, EX_RND_DATA);

    printf( "\r\n-----------\r\nEnd exGPStorage(%s), result = %s\r\n------------\r\n",
        ((tstMode & EXTENDED_TEST) == EXTENDED_TEST) ? "Extended Test" : "Fast Test",
        ((result == 1)? "OK": "FAILED"));

    return result;
}

/**
 * Demonstrate monotonic counter usage.
 *
 * @param[in] initMode Visit the documentation of ::a71chInitModule for more information on this parameter
 */
static U8 exMonotonicCounter(U8 initMode)
{
    U8 result = 1;
    U8 index = 0;
    U16 err;

    U32 tgtValue[] = {0x00000004, 0x00000014, 0x00000024, 0x00000034};
    U32 readValue = 0;

    printf("\r\n-----------\r\nStart exMonotonicCounter(%s)\r\n------------\r\n",
        getInitModeAsString(initMode));

    // Initialize the A71CH (Debug mode restrictions may apply)
    result &= a71chInitModule(initMode);
    assert(result);

    // Check all counters default to value 0
    for (index=0; index<A71CH_COUNTER_MAX; index++)
    {
        printf("\r\nA71_GetCounter(index=0x%02x)\r\n", index);
        err = A71_GetCounter(index, &readValue);
        result &= AX_CHECK_SW(err, SW_OK, "Failed to retrieve counter value");
        if (err == SW_OK)
        {
            if (readValue != 0)
            {
                printf("Failed to retrieve expected counter value (index=0x%02x): %ld != 0\r\n", index, readValue);
                result &= 0;
            }
        }
    }

    // Set all counters to a counter specific target value
    for (index=0; index<A71CH_COUNTER_MAX; index++)
    {
        printf("\r\nA71_SetCounter(index=0x%02x, %ld)\r\n", index, tgtValue[index]);
        err = A71_SetCounter(index, tgtValue[index]);
        result &= AX_CHECK_SW(err, SW_OK, "Failed to set counter value");
    }

    // Verify all counters were set to target value
    for (index=0; index<A71CH_COUNTER_MAX; index++)
    {
        printf("\r\nA71_GetCounter(index=0x%02x)\r\n", index);
        err = A71_GetCounter(index, &readValue);
        result &= AX_CHECK_SW(err, SW_OK, "Failed to retrieve counter value");
        if (err == SW_OK)
        {
            if (readValue != tgtValue[index])
            {
                printf("Failed to retrieve expected counter value (index=0x%02x): %ld != %ld\r\n", index, readValue, tgtValue[index]);
                result &= 0;
            }
        }
    }

    // Increment all counters with one
    for (index=0; index<A71CH_COUNTER_MAX; index++)
    {
        printf("\r\nA71_IncrementCounter(index=0x%02x)\r\n", index);
        err = A71_IncrementCounter(index);
        result &= AX_CHECK_SW(err, SW_OK, "Failed to increment counter value");
    }

    // Verify all counters have target value
    for (index=0; index<A71CH_COUNTER_MAX; index++)
    {
        printf("\r\nA71_GetCounter(index=0x%02x)\r\n", index);
        err = A71_GetCounter(index, &readValue);
        result &= AX_CHECK_SW(err, SW_OK, "Failed to retrieve counter value");
        if (err == SW_OK)
        {
            if (readValue != (tgtValue[index]+1))
            {
                printf("Failed to retrieve expected counter value (index=0x%02x): %ld != %ld\r\n", index, readValue, (tgtValue[index]+1));
                result &= 0;
            }
        }
    }

    // Now erase all counters and check whether they are back to default value 0
    for (index=0; index<A71CH_COUNTER_MAX; index++)
    {
        printf("\r\nA71_DbgEraseCounter(index=0x%02x)\r\n", index);
        err = A71_DbgEraseCounter(index);
        result &= AX_CHECK_SW(err, SW_OK, "Failed to erase counter value");
    }
    for (index=0; index<A71CH_COUNTER_MAX; index++)
    {
        printf("\r\nA71_GetCounter(index=0x%02x)\r\n", index);
        err = A71_GetCounter(index, &readValue);
        result &= AX_CHECK_SW(err, SW_OK, "Failed to retrieve counter value");
        if (err == SW_OK)
        {
            if (readValue != 0)
            {
                printf("Failed to retrieve expected counter value (index=0x%02x): %ld != 0\r\n", index, readValue);
                result &= 0;
            }
        }
    }

    // Set all counters to a counter specific target value & increment the value & freeze the counter
    for (index=0; index<A71CH_COUNTER_MAX; index++)
    {
        printf("\r\nA71_SetCounter(index=0x%02x, %ld)\r\n", index, tgtValue[index]);
        err = A71_SetCounter(index, tgtValue[index]);
        result &= AX_CHECK_SW(err, SW_OK, "Failed to set counter value");
        printf("\r\nA71_IncrementCounter(index=0x%02x)\r\n", index);
        err = A71_IncrementCounter(index);
        result &= AX_CHECK_SW(err, SW_OK, "Failed to increment counter value");
    }

    // Check value just set
    for (index=0; index<A71CH_COUNTER_MAX; index++)
    {
        printf("\r\nA71_GetCounter(index=0x%02x)\r\n", index);
        err = A71_GetCounter(index, &readValue);
        result &= AX_CHECK_SW(err, SW_OK, "Failed to retrieve counter value");
        if (err == SW_OK)
        {
            if (readValue != (tgtValue[index]+1))
            {
                printf("Failed to retrieve expected counter value (index=0x%02x): %ld != %ld\r\n", index, readValue, (tgtValue[index]+1));
                result &= 0;
            }
        }

        // printf("\r\nA71_IncrementCounter(index=0x%02x) - negative test\r\n", index);
        // err = A71_IncrementCounter(index);
        // result &= AX_CHECK_SW(err, SW_COMMAND_NOT_ALLOWED, "Increment frozen counter must fail");
    }

    printf("\r\n-----------\r\nEnd exMonotonicCounter(%s), result = %s\r\n------------\r\n", getInitModeAsString(initMode),
        ((result == 1)? "OK": "FAILED"));

    return result;
}

/**
 * Demonstrate reading and writing to GP Storage.
 *
 * @param[in] initMode Visit the documentation of ::a71chInitModule for more information on this parameter
 * @param[in] tstMode Influence test execution through this parameter
 */
static U8 exGpStoragePacketSize(U8 initMode, U8 tstMode)
{
    U16 testSizes[] = {60, 200, 300, 400, 500, 600, 700, 800, 900, 1000, A71CH_GP_STORAGE_SIZE};
    U8 gpStorageRef[A71CH_GP_STORAGE_SIZE];
    U8 gpStorage[A71CH_GP_STORAGE_SIZE];
    int i = 0;
    U16 packetSize = 0;
    int maxIter = sizeof(testSizes)/sizeof(U16);
    U8 result = 1;
    U16 err;

    printf( "\r\n-----------\r\nStart exGpStoragePacketSize(%s)\r\n------------\r\n",
        getInitModeAsString(initMode));

    if ((tstMode & EX_INC_DATA) == EX_INC_DATA)
    {
        for (i=0; i<A71CH_GP_STORAGE_SIZE; i++)
        {
            gpStorageRef[i] = (U8)i;
        }
    }
    else
    {
        // Create reference array containing random values
        srand(0);
        for (i=0; i<A71CH_GP_STORAGE_SIZE; i++)
        {
            gpStorageRef[i] = (U8)rand();
        }
    }

    // Initialize the A71CH (Debug mode restrictions may apply)
    result &= a71chInitModule(initMode);
    assert(result);

    if ((tstMode & EX_ALL_PACKET_SIZES) == EX_ALL_PACKET_SIZES)
    {
        // Test all packetsizes in read and write mode
        for (packetSize = 1; packetSize <= A71CH_GP_STORAGE_SIZE; packetSize++)
        {
            if ((tstMode & EX_INC_DATA) == EX_RND_DATA)
            {
                // In case of random data, recreate the reference array
                srand(0);
                for (i=0; i<packetSize; i++)
                {
                    gpStorageRef[i] = (U8)rand();
                }
            }
            printf( "\r\nA71_SetGpData(0, %d, ...)\r\n", packetSize);
            err = A71_SetGpData(0, gpStorageRef, packetSize);
            result &= AX_CHECK_SW(err, SW_OK, "A71_SetGpData fails");

            printf( "\r\nA71_GetGpData(0, %d, ...)\r\n", packetSize);
            err = A71_GetGpData(0, gpStorage, packetSize);
            result &= AX_CHECK_SW(err, SW_OK, "A71_GetGpData fails");

            result &= AX_COMPARE_BYTE_ARRAY( "gpStorageRef", gpStorageRef, packetSize, "gpStorage", gpStorage, packetSize, AX_COLON_32);
        }
    }
    else
    {
        // Always start from offset 0
        for (i = 0; i < maxIter; i++)
        {
            printf( "\r\nA71_SetGpData(0, %d, ...)\r\n", testSizes[i]);
            err = A71_SetGpData(0, gpStorageRef, testSizes[i]);
            result &= AX_CHECK_SW(err, SW_OK, "A71_SetGpData fails");

            printf( "\r\nA71_GetGpData(0, %d, ...)\r\n", testSizes[i]);
            err = A71_GetGpData(0, gpStorage, testSizes[i]);
            result &= AX_CHECK_SW(err, SW_OK, "A71_GetGpData fails");

            result &= AX_COMPARE_BYTE_ARRAY("gpStorageRef", gpStorageRef, testSizes[i], "gpStorage", gpStorage, testSizes[i], AX_COLON_32);
        }
    }

    printf( "\r\n-----------\r\nEnd exGpStoragePacketSize(%s), result = %s\r\n------------\r\n",
        getInitModeAsString(initMode), ((result == 1)? "OK": "FAILED"));

    return result;
}

/**
 * Demonstrate freezing of GP Storage data chunks.
 * GP Storage data can be frozen with a granularity of ::A71CH_GP_STORAGE_GRANULARITY  byte
 *
 * @param[in] initMode Visit the documentation of ::a71chInitModule for more information on this parameter
 * @param[in] tstMode Influence test execution through this parameter
 */
static U8 exGpStorageFreeze(U8 initMode, U8 tstMode)
{
#if (A71CH_GP_STORAGE_SIZE == A71CH_GP_STORAGE_SIZE_A)
    U16 testSizes[] = {60, 200, 300, 400, 500, 600, 700, 800, 900, 1000, A71CH_GP_STORAGE_SIZE};
#else
    U16 testSizes[] = {60, 200, 300, 400, 500, 600, 1000, 1500, 2000, 3000, A71CH_GP_STORAGE_SIZE};
#endif
    U8 gpStorageRef[A71CH_GP_STORAGE_SIZE];
    U8 gpStorageNew[A71CH_GP_STORAGE_SIZE];
    U8 gpStorage[A71CH_GP_STORAGE_SIZE];
    U8 gpStorageExpected[A71CH_GP_STORAGE_SIZE];
    int i = 0;
    U16 packetSize = 0;
    U16 offset = 0;
    int maxIter = sizeof(testSizes)/sizeof(U16);
    U8 result = 1;
    U16 err;

    printf( "\r\n-----------\r\nStart exGpStorageFreeze(%s, A71CH_GP_STORAGE_SIZE=%d)\r\n------------\r\n",
        getInitModeAsString(initMode), A71CH_GP_STORAGE_SIZE);

    if ((tstMode & EX_INC_DATA) == EX_INC_DATA)
    {
        for (i=0; i<A71CH_GP_STORAGE_SIZE; i++)
        {
            gpStorageRef[i] = (U8)i;
        }
    }
    else
    {
        // Create reference array containing random values
        srand(0);
        for (i=0; i<A71CH_GP_STORAGE_SIZE; i++)
        {
            gpStorageRef[i] = (U8)rand();
        }
    }

    for (i=0; i<A71CH_GP_STORAGE_SIZE; i++)
    {
        gpStorageNew[i] = 0xAA;
    }

    // Initialize the A71CH (Debug mode restrictions may apply)
    result &= a71chInitModule(initMode);
    assert(result);

    if ((tstMode & EX_ALL_PACKET_SIZES) == EX_ALL_PACKET_SIZES)
    {
        // Test all packetsizes in read and write mode
        for (packetSize = 1; packetSize <= A71CH_GP_STORAGE_SIZE; packetSize++)
        {
            if ((tstMode & EX_INC_DATA) == EX_RND_DATA)
            {
                // In case of random data, recreate the reference array
                srand(0);
                for (i=0; i<packetSize; i++)
                {
                    gpStorageRef[i] = (U8)rand();
                }
            }
            printf( "\r\nA71_SetGpData(0, %d, ...)\r\n", packetSize);
            err = A71_SetGpData(0, gpStorageRef, packetSize);
            result &= AX_CHECK_SW(err, SW_OK, "A71_SetGpData fails");

            printf( "\r\nA71_GetGpData(0, %d, ...)\r\n", packetSize);
            err = A71_GetGpData(0, gpStorage, packetSize);
            result &= AX_CHECK_SW(err, SW_OK, "A71_GetGpData fails");

            result &= AX_COMPARE_BYTE_ARRAY("gpStorageRef", gpStorageRef, packetSize, "gpStorage", gpStorage, packetSize, AX_COLON_32);
        }
    }
    else
    {
        // Always start from offset 0
        for (i = 0; i < maxIter; i++)
        {
            printf( "\r\nA71_SetGpData(0, %d, ...)\r\n", testSizes[i]);
            err = A71_SetGpData(0, gpStorageRef, testSizes[i]);
            result &= AX_CHECK_SW(err, SW_OK, "A71_SetGpData fails");

            printf( "\r\nA71_GetGpData(0, %d, ...)\r\n", testSizes[i]);
            err = A71_GetGpData(0, gpStorage, testSizes[i]);
            result &= AX_CHECK_SW(err, SW_OK, "A71_GetGpData fails");

            result &= AX_COMPARE_BYTE_ARRAY("gpStorageRef", gpStorageRef, testSizes[i], "gpStorage", gpStorage, testSizes[i], AX_COLON_32);
        }
    }

    // Just fill up the full GpStorage with reference data
    packetSize = A71CH_GP_STORAGE_SIZE;
    printf( "\r\nA71_SetGpData(0, %d, ...)\r\n", packetSize);
    err = A71_SetGpData(0, gpStorageRef, packetSize);
    result &= AX_CHECK_SW(err, SW_OK, "A71_SetGpData fails");

    // Check whether data was written successfully
    printf( "\r\nA71_GetGpData(0, %d, ...)\r\n", packetSize);
    err = A71_GetGpData(0, gpStorage, packetSize);
    result &= AX_CHECK_SW(err, SW_OK, "A71_GetGpData fails");
    result &= AX_COMPARE_BYTE_ARRAY("gpStorageRef", gpStorageRef, packetSize,
        "gpStorage", gpStorage, packetSize, AX_COLON_32);

    // Lock the first half of GP storage
    printf( "\r\nA71_FreezeGpData(offset=%d, dataLen=%d, ...)\r\n", 0, A71CH_GP_STORAGE_SIZE>>1);
    err = A71_FreezeGpData(0, A71CH_GP_STORAGE_SIZE>>1);
    result &= AX_CHECK_SW(err, SW_OK, "A71_FreezeGpData fails");

    // Attempt to write in the frozen area
    packetSize = 16;
    offset = 0;
    printf( "\r\nA71_SetGpData(%d, %d, ...)\r\n", offset, packetSize);
    err = A71_SetGpData(offset, gpStorageNew, packetSize);
    result &= AX_CHECK_SW(err, SW_COMMAND_NOT_ALLOWED, "A71_SetGpData was expected to fail (because area has just been locked)");

    // Write to open area
    offset = A71CH_GP_STORAGE_SIZE>>1;
    printf( "\r\nA71_SetGpData(%d, %d, ...)\r\n", offset, packetSize);
    err = A71_SetGpData(offset, gpStorageNew, packetSize);
    result &= AX_CHECK_SW(err, SW_OK, "A71_SetGpData fails");

    // Create expected data pattern
    memcpy(gpStorageExpected, gpStorageRef, A71CH_GP_STORAGE_SIZE);
    memcpy(gpStorageExpected+(A71CH_GP_STORAGE_SIZE>>1), gpStorageNew, 16);

    // Retrieve data and compare with expected data pattern
    packetSize = A71CH_GP_STORAGE_SIZE;
    printf( "\r\nA71_GetGpData(0, %d, ...)\r\n", packetSize);
    err = A71_GetGpData(0, gpStorage, packetSize);
    result &= AX_CHECK_SW(err, SW_OK, "A71_GetGpData fails");
    result &= AX_COMPARE_BYTE_ARRAY("gpStorageExpected", gpStorageExpected, A71CH_GP_STORAGE_SIZE,
        "gpStorage", gpStorage, A71CH_GP_STORAGE_SIZE, AX_COLON_32);

    printf( "\r\n-----------\r\nEnd exGpStorageFreeze(%s), result = %s\r\n------------\r\n",
        getInitModeAsString(initMode), ((result == 1)? "OK": "FAILED"));

    return result;
}
