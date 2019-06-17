/**
 * @file ex_gpstorage.c
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
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
#include "nxLog_hostLib.h"

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
    LOG_I( "-----------Start exGPStorage(%s)------------",
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

    LOG_I( "-----------End exGPStorage(%s), result = %s------------",
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

    LOG_I("-----------Start exMonotonicCounter(%s)------------",
        getInitModeAsString(initMode));

    // Initialize the A71CH (Debug mode restrictions may apply)
    result &= a71chInitModule(initMode);
    assert(result);

    // Check all counters default to value 0
    for (index=0; index<A71CH_COUNTER_MAX; index++)
    {
        LOG_I("A71_GetCounter(index=0x%02x)", index);
        err = A71_GetCounter(index, &readValue);
        result &= AX_CHECK_SW(err, SW_OK, "Failed to retrieve counter value");
        if (err == SW_OK)
        {
            if (readValue != 0)
            {
                LOG_E("Failed to retrieve expected counter value (index=0x%02x): %ld != 0", index, readValue);
                result &= 0;
            }
        }
    }

    // Set all counters to a counter specific target value
    for (index=0; index<A71CH_COUNTER_MAX; index++)
    {
        LOG_I("A71_SetCounter(index=0x%02x, %ld)", index, tgtValue[index]);
        err = A71_SetCounter(index, tgtValue[index]);
        result &= AX_CHECK_SW(err, SW_OK, "Failed to set counter value");
    }

    // Verify all counters were set to target value
    for (index=0; index<A71CH_COUNTER_MAX; index++)
    {
        LOG_I("A71_GetCounter(index=0x%02x)", index);
        err = A71_GetCounter(index, &readValue);
        result &= AX_CHECK_SW(err, SW_OK, "Failed to retrieve counter value");
        if (err == SW_OK)
        {
            if (readValue != tgtValue[index])
            {
                LOG_E("Failed to retrieve expected counter value (index=0x%02x): %ld != %ld", index, readValue, tgtValue[index]);
                result &= 0;
            }
        }
    }

    // Increment all counters with one
    for (index=0; index<A71CH_COUNTER_MAX; index++)
    {
        LOG_I("A71_IncrementCounter(index=0x%02x)", index);
        err = A71_IncrementCounter(index);
        result &= AX_CHECK_SW(err, SW_OK, "Failed to increment counter value");
    }

    // Verify all counters have target value
    for (index=0; index<A71CH_COUNTER_MAX; index++)
    {
        LOG_I("A71_GetCounter(index=0x%02x)", index);
        err = A71_GetCounter(index, &readValue);
        result &= AX_CHECK_SW(err, SW_OK, "Failed to retrieve counter value");
        if (err == SW_OK)
        {
            if (readValue != (tgtValue[index]+1))
            {
                LOG_E("Failed to retrieve expected counter value (index=0x%02x): %ld != %ld", index, readValue, (tgtValue[index]+1));
                result &= 0;
            }
        }
    }

    // Now erase all counters and check whether they are back to default value 0
    for (index=0; index<A71CH_COUNTER_MAX; index++)
    {
        LOG_I("A71_DbgEraseCounter(index=0x%02x)", index);
        err = A71_DbgEraseCounter(index);
        result &= AX_CHECK_SW(err, SW_OK, "Failed to erase counter value");
    }
    for (index=0; index<A71CH_COUNTER_MAX; index++)
    {
        LOG_I("A71_GetCounter(index=0x%02x)", index);
        err = A71_GetCounter(index, &readValue);
        result &= AX_CHECK_SW(err, SW_OK, "Failed to retrieve counter value");
        if (err == SW_OK)
        {
            if (readValue != 0)
            {
                LOG_E("Failed to retrieve expected counter value (index=0x%02x): %ld != 0", index, readValue);
                result &= 0;
            }
        }
    }

    // Set all counters to a counter specific target value & increment the value & freeze the counter
    for (index=0; index<A71CH_COUNTER_MAX; index++)
    {
        LOG_I("A71_SetCounter(index=0x%02x, %ld)", index, tgtValue[index]);
        err = A71_SetCounter(index, tgtValue[index]);
        result &= AX_CHECK_SW(err, SW_OK, "Failed to set counter value");
        LOG_I("A71_IncrementCounter(index=0x%02x)", index);
        err = A71_IncrementCounter(index);
        result &= AX_CHECK_SW(err, SW_OK, "Failed to increment counter value");
    }

    // Check value just set
    for (index=0; index<A71CH_COUNTER_MAX; index++)
    {
        LOG_I("A71_GetCounter(index=0x%02x)", index);
        err = A71_GetCounter(index, &readValue);
        result &= AX_CHECK_SW(err, SW_OK, "Failed to retrieve counter value");
        if (err == SW_OK)
        {
            if (readValue != (tgtValue[index]+1))
            {
                LOG_E("Failed to retrieve expected counter value (index=0x%02x): %ld != %ld", index, readValue, (tgtValue[index]+1));
                result &= 0;
            }
        }

        // LOG_I("A71_IncrementCounter(index=0x%02x) - negative test", index);
        // err = A71_IncrementCounter(index);
        // result &= AX_CHECK_SW(err, SW_COMMAND_NOT_ALLOWED, "Increment frozen counter must fail");
    }

    LOG_I("-----------End exMonotonicCounter(%s), result = %s------------", getInitModeAsString(initMode),
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

    LOG_I( "-----------Start exGpStoragePacketSize(%s)------------",
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
            LOG_I( "A71_SetGpData(0, %d, ...)", packetSize);
            err = A71_SetGpData(0, gpStorageRef, packetSize);
            result &= AX_CHECK_SW(err, SW_OK, "A71_SetGpData fails");

            LOG_I( "A71_GetGpData(0, %d, ...)", packetSize);
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
            LOG_I( "A71_SetGpData(0, %d, ...)", testSizes[i]);
            err = A71_SetGpData(0, gpStorageRef, testSizes[i]);
            result &= AX_CHECK_SW(err, SW_OK, "A71_SetGpData fails");

            LOG_I( "A71_GetGpData(0, %d, ...)", testSizes[i]);
            err = A71_GetGpData(0, gpStorage, testSizes[i]);
            result &= AX_CHECK_SW(err, SW_OK, "A71_GetGpData fails");

            result &= AX_COMPARE_BYTE_ARRAY("gpStorageRef", gpStorageRef, testSizes[i], "gpStorage", gpStorage, testSizes[i], AX_COLON_32);
        }
    }

    LOG_I( "-----------End exGpStoragePacketSize(%s), result = %s------------",
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

    LOG_I( "-----------Start exGpStorageFreeze(%s, A71CH_GP_STORAGE_SIZE=%d)------------",
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
            LOG_I( "A71_SetGpData(0, %d, ...)", packetSize);
            err = A71_SetGpData(0, gpStorageRef, packetSize);
            result &= AX_CHECK_SW(err, SW_OK, "A71_SetGpData fails");

            LOG_I( "A71_GetGpData(0, %d, ...)", packetSize);
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
            LOG_I( "A71_SetGpData(0, %d, ...)", testSizes[i]);
            err = A71_SetGpData(0, gpStorageRef, testSizes[i]);
            result &= AX_CHECK_SW(err, SW_OK, "A71_SetGpData fails");

            LOG_I( "A71_GetGpData(0, %d, ...)", testSizes[i]);
            err = A71_GetGpData(0, gpStorage, testSizes[i]);
            result &= AX_CHECK_SW(err, SW_OK, "A71_GetGpData fails");

            result &= AX_COMPARE_BYTE_ARRAY("gpStorageRef", gpStorageRef, testSizes[i], "gpStorage", gpStorage, testSizes[i], AX_COLON_32);
        }
    }

    // Just fill up the full GpStorage with reference data
    packetSize = A71CH_GP_STORAGE_SIZE;
    LOG_I( "A71_SetGpData(0, %d, ...)", packetSize);
    err = A71_SetGpData(0, gpStorageRef, packetSize);
    result &= AX_CHECK_SW(err, SW_OK, "A71_SetGpData fails");

    // Check whether data was written successfully
    LOG_I( "A71_GetGpData(0, %d, ...)", packetSize);
    err = A71_GetGpData(0, gpStorage, packetSize);
    result &= AX_CHECK_SW(err, SW_OK, "A71_GetGpData fails");
    result &= AX_COMPARE_BYTE_ARRAY("gpStorageRef", gpStorageRef, packetSize,
        "gpStorage", gpStorage, packetSize, AX_COLON_32);

    // Lock the first half of GP storage
    LOG_I( "A71_FreezeGpData(offset=%d, dataLen=%d, ...)", 0, A71CH_GP_STORAGE_SIZE>>1);
    err = A71_FreezeGpData(0, A71CH_GP_STORAGE_SIZE>>1);
    result &= AX_CHECK_SW(err, SW_OK, "A71_FreezeGpData fails");

    // Attempt to write in the frozen area
    packetSize = 16;
    offset = 0;
    LOG_I( "A71_SetGpData(%d, %d, ...)", offset, packetSize);
    err = A71_SetGpData(offset, gpStorageNew, packetSize);
    result &= AX_CHECK_SW(err, SW_COMMAND_NOT_ALLOWED, "A71_SetGpData was expected to fail (because area has just been locked)");

    // Write to open area
    offset = A71CH_GP_STORAGE_SIZE>>1;
    LOG_I( "A71_SetGpData(%d, %d, ...)", offset, packetSize);
    err = A71_SetGpData(offset, gpStorageNew, packetSize);
    result &= AX_CHECK_SW(err, SW_OK, "A71_SetGpData fails");

    // Create expected data pattern
    memcpy(gpStorageExpected, gpStorageRef, A71CH_GP_STORAGE_SIZE);
    memcpy(gpStorageExpected+(A71CH_GP_STORAGE_SIZE>>1), gpStorageNew, 16);

    // Retrieve data and compare with expected data pattern
    packetSize = A71CH_GP_STORAGE_SIZE;
    LOG_I( "A71_GetGpData(0, %d, ...)", packetSize);
    err = A71_GetGpData(0, gpStorage, packetSize);
    result &= AX_CHECK_SW(err, SW_OK, "A71_GetGpData fails");
    result &= AX_COMPARE_BYTE_ARRAY("gpStorageExpected", gpStorageExpected, A71CH_GP_STORAGE_SIZE,
        "gpStorage", gpStorage, A71CH_GP_STORAGE_SIZE, AX_COLON_32);

    LOG_I( "-----------End exGpStorageFreeze(%s), result = %s------------",
        getInitModeAsString(initMode), ((result == 1)? "OK": "FAILED"));

    return result;
}
