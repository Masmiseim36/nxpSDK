/**
 * @file ex_hlse_aes.c
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
 * Example invocation of symmetric key related crypto functionality of the A71CH
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "a71ch_ex_hlse.h"
#include "a71_debug.h"
#include "sm_types.h"
#include "sm_apdu.h"
#include "ax_util.h"
#include "tst_sm_util.h"

#include "tst_a71ch_util.h"
#include "tst_hlse_a71ch_util.h"

#include "tstHostCrypto.h"

#include "HLSEAPI.h"

static U8 exAesRfc3394(U8 initMode);
static U8 exAesRfc3394Precooked(U8 initMode);
static U8 exSymHkdf(U8 initMode);
static U8 exSymHmacSha256(U8 initMode);

/**
 * Demonstrate wrapped setting/retrieving of symmetric keys:
 * - ::exAesRfc3394Precooked
 * - ::exAesRfc3394
 *
 * Demonstrate crypto applications of stored symmetric keys:
 * - ::exSymHkdf
 * - ::exSymHmacSha256
 */
U8 exHlseAes()
{
    U8 result = 1;
    PRINTF( "\r\n-----------\r\nStart exAes()\r\n------------\r\n");

    DEV_ClearChannelState();

    // ** Without channel encryption **
    // RFC3394 wrapping (using prepared values)
    result &= exAesRfc3394Precooked(INIT_MODE_RESET);

    // RFC3394 wrapping
    result &= exAesRfc3394(INIT_MODE_RESET);

    // HKDF
    result &= exSymHkdf(INIT_MODE_RESET);

    // HMAC
    result &= exSymHmacSha256(INIT_MODE_RESET);

    // ** With an SCP03 channel setup between host and A71CH **
    // RFC3394 wrapping (using prepared values)
    result &= exAesRfc3394Precooked(INIT_MODE_RESET_DO_SCP03);

    // RFC3394 wrapping
    result &= exAesRfc3394(INIT_MODE_RESET_DO_SCP03);

    // HMAC
    result &= exSymHkdf(INIT_MODE_RESET_DO_SCP03);

    // HMAC
    result &= exSymHmacSha256(INIT_MODE_RESET_DO_SCP03);

    // overall result
    PRINTF( "\r\n-----------\r\nEnd exAes(), result = %s\r\n------------\r\n", ((result == 1)? "OK": "FAILED"));

    return result;
}

/**
 * Demonstrate setting wrapped AES keys.
 * No Host Side crypto is required because test vectors have been pre-calculated ('precooked').
 *
 * @param[in] initMode Visit the documentation of ::a71chInitModule for more information on this parameter
 */
static U8 exAesRfc3394Precooked(U8 initMode)
{
    U8 result = 1;
    U16 err;

    const U8 aesRef[A71CH_SYM_KEY_MAX_B][16] = {
        {0xDB, 0xFE, 0xE9, 0xE3, 0xB2, 0x76, 0x15, 0x4D, 0x67, 0xF9, 0xD8, 0x4C, 0xB9, 0x35, 0x54, 0x56},
        {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f},
        {0xC0, 0x79, 0xEF, 0x82, 0xCD, 0xF7, 0x12, 0xF2, 0x87, 0x28, 0xFD, 0x18, 0xED, 0xD7, 0xF2, 0xE4},
        {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x06, 0x07, 0x08, 0x09, 0xA0, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4},
        {0xDB, 0xFE, 0xE9, 0xE3, 0xB2, 0x76, 0x15, 0x4D, 0x67, 0xF9, 0xD8, 0x4C, 0xB9, 0x35, 0x54, 0x56}, // First 4 repeated
        {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f},
        {0xC0, 0x79, 0xEF, 0x82, 0xCD, 0xF7, 0x12, 0xF2, 0x87, 0x28, 0xFD, 0x18, 0xED, 0xD7, 0xF2, 0xE4},
        {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x06, 0x07, 0x08, 0x09, 0xA0, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4}
    };

    // aesRef keys wrapped with themselves
    const U8 aesRefWrapped[A71CH_SYM_KEY_MAX_B][24] = {
        {0x83, 0x49, 0x96, 0xFE, 0x38, 0xD4, 0xBC, 0x97, 0x85, 0xA2, 0xC5, 0x1F, 0x9F, 0xD6, 0x4E, 0xF3,
         0x9C, 0x2C, 0x5A, 0xE0, 0xF2, 0x82, 0x02, 0x3F},
        {0x93, 0x5A, 0x3E, 0xB1, 0x01, 0xC3, 0x4A, 0xDD, 0x02, 0x5E, 0x17, 0x0B, 0x46, 0xFF, 0x0D, 0xB2,
         0x3E, 0x5C, 0x2F, 0xAE, 0x8C, 0x8F, 0x83, 0x70},
        {0xD0, 0x7B, 0xEB, 0x03, 0xE3, 0xB0, 0x01, 0x4F, 0xD9, 0x7D, 0x42, 0x3B, 0x9E, 0x51, 0x23, 0xEE,
         0x9A, 0x4B, 0x49, 0xAB, 0x9F, 0x10, 0xBE, 0x18},
        {0x2D, 0x36, 0x31, 0xEA, 0xF4, 0x69, 0xAD, 0xAF, 0x08, 0xE5, 0xB5, 0xEE, 0x97, 0xF5, 0x0A, 0x14,
         0xC2, 0x6A, 0x6F, 0xE8, 0xDC, 0xBB, 0x39, 0xF3},
        {0x83, 0x49, 0x96, 0xFE, 0x38, 0xD4, 0xBC, 0x97, 0x85, 0xA2, 0xC5, 0x1F, 0x9F, 0xD6, 0x4E, 0xF3, // First 4 repeated
         0x9C, 0x2C, 0x5A, 0xE0, 0xF2, 0x82, 0x02, 0x3F},
        {0x93, 0x5A, 0x3E, 0xB1, 0x01, 0xC3, 0x4A, 0xDD, 0x02, 0x5E, 0x17, 0x0B, 0x46, 0xFF, 0x0D, 0xB2,
         0x3E, 0x5C, 0x2F, 0xAE, 0x8C, 0x8F, 0x83, 0x70},
        {0xD0, 0x7B, 0xEB, 0x03, 0xE3, 0xB0, 0x01, 0x4F, 0xD9, 0x7D, 0x42, 0x3B, 0x9E, 0x51, 0x23, 0xEE,
         0x9A, 0x4B, 0x49, 0xAB, 0x9F, 0x10, 0xBE, 0x18},
        {0x2D, 0x36, 0x31, 0xEA, 0xF4, 0x69, 0xAD, 0xAF, 0x08, 0xE5, 0xB5, 0xEE, 0x97, 0xF5, 0x0A, 0x14,
         0xC2, 0x6A, 0x6F, 0xE8, 0xDC, 0xBB, 0x39, 0xF3}
    };

    // derivedDataRef used as reference
    const U8 derivedDataRef[A71CH_SYM_KEY_MAX_B][32] = {
        {0x36, 0x54, 0xBE, 0x49, 0x32, 0xB6, 0xE3, 0x84, 0xB8, 0x0C, 0xF5, 0xFC, 0xF7, 0x7B, 0x93, 0x93,
         0x82, 0x59, 0xD0, 0x45, 0x7E, 0xED, 0xE3, 0xB8, 0x6F, 0xC0, 0x91, 0x99, 0x67, 0x10, 0xA4, 0xF3},
        {0x9E, 0xA8, 0xD4, 0x14, 0xE2, 0x9A, 0x9A, 0x42, 0x2B, 0x45, 0x65, 0x21, 0xA1, 0x08, 0xA9, 0x85,
         0x97, 0x52, 0x03, 0xC3, 0x15, 0x23, 0xC0, 0xE2, 0x05, 0x86, 0x40, 0x8C, 0xE3, 0xAF, 0x39, 0x7F},
        {0xE8, 0x50, 0xE2, 0x1A, 0xB3, 0xBB, 0x41, 0xC0, 0x79, 0x39, 0x80, 0x1D, 0xA5, 0x87, 0x2F, 0x4C,
         0x4D, 0xE8, 0xB4, 0x6C, 0x2D, 0x56, 0x88, 0xA4, 0x44, 0x21, 0x65, 0xD6, 0xEB, 0xD8, 0x83, 0x3E},
        {0x2D, 0xCA, 0xE0, 0x37, 0x32, 0x7D, 0xA9, 0x97, 0xC6, 0x50, 0xAC, 0x9B, 0x93, 0x1E, 0x6F, 0x46,
         0xB6, 0xB5, 0x2B, 0xF6, 0x36, 0x57, 0xB2, 0x65, 0x70, 0x04, 0x5A, 0x06, 0x26, 0x37, 0x77, 0x2E},
        {0x36, 0x54, 0xBE, 0x49, 0x32, 0xB6, 0xE3, 0x84, 0xB8, 0x0C, 0xF5, 0xFC, 0xF7, 0x7B, 0x93, 0x93, // First 4 repeated
         0x82, 0x59, 0xD0, 0x45, 0x7E, 0xED, 0xE3, 0xB8, 0x6F, 0xC0, 0x91, 0x99, 0x67, 0x10, 0xA4, 0xF3},
        {0x9E, 0xA8, 0xD4, 0x14, 0xE2, 0x9A, 0x9A, 0x42, 0x2B, 0x45, 0x65, 0x21, 0xA1, 0x08, 0xA9, 0x85,
         0x97, 0x52, 0x03, 0xC3, 0x15, 0x23, 0xC0, 0xE2, 0x05, 0x86, 0x40, 0x8C, 0xE3, 0xAF, 0x39, 0x7F},
        {0xE8, 0x50, 0xE2, 0x1A, 0xB3, 0xBB, 0x41, 0xC0, 0x79, 0x39, 0x80, 0x1D, 0xA5, 0x87, 0x2F, 0x4C,
         0x4D, 0xE8, 0xB4, 0x6C, 0x2D, 0x56, 0x88, 0xA4, 0x44, 0x21, 0x65, 0xD6, 0xEB, 0xD8, 0x83, 0x3E},
        {0x2D, 0xCA, 0xE0, 0x37, 0x32, 0x7D, 0xA9, 0x97, 0xC6, 0x50, 0xAC, 0x9B, 0x93, 0x1E, 0x6F, 0x46,
         0xB6, 0xB5, 0x2B, 0xF6, 0x36, 0x57, 0xB2, 0x65, 0x70, 0x04, 0x5A, 0x06, 0x26, 0x37, 0x77, 0x2E}
    };

    U8 aesRefIdxLastWrappedByIdx0[] = {
        0x3C, 0x0B, 0x6E, 0x87, 0x11, 0x5C, 0xD1, 0x44, 0x34, 0x36, 0x25, 0x13, 0x6D, 0x66, 0xE5, 0x33,
        0x3D, 0x24, 0x7A, 0x42, 0xC8, 0x13, 0x38, 0xC1};
    U16 aesRefIdxLastWrappedByIdx0Len = sizeof(aesRefIdxLastWrappedByIdx0);

    U8 indexAesKey = 0;
    U8 indexWrapKey = 0;
    U8 indexWriteKey = 0;

    U8 info[] = {0x01, 0x02, 0x03, 0x04};
    U16 infoLen = sizeof(info);
    U8 derivedData[32];
    U16 derivedDataLen  = sizeof(derivedData);
    U8 nBlock = 0x01;

    // to hold handles for created sym keys
    HLSE_OBJECT_HANDLE handles[A71CH_SYM_KEY_MAX] = {0};

    PRINTF("\r\n-----------\r\nStart exAesRfc3394Precooked(%s)\r\n------------\r\n", getInitModeAsString(initMode));

    // Initialize the A71CH (Debug mode restrictions may apply)
    result &= hlse_a71chInitModule(initMode);

    for (indexAesKey=0; indexAesKey<A71CH_SYM_KEY_MAX; indexAesKey++)
    {
        HLSE_OBJECT_INDEX index = indexAesKey;
        HLSE_OBJECT_TYPE objType = HLSE_SYMMETRIC_KEY;
        HLSE_ATTRIBUTE attr[3];
        unsigned short templateSize = 3;

        // Write the key (unwrapped)
        PRINTF( "\r\nA71_SetSymKey(0x%02x)\r\n", indexAesKey);
#if 0
        err = A71_SetSymKey((SST_Index_t)indexAesKey, aesRef[indexAesKey], sizeof(aesRef[indexAesKey]));
#else
        attr[0].type = HLSE_ATTR_OBJECT_TYPE;
        attr[0].value = &objType;
        attr[0].valueLen = sizeof(objType);
        attr[1].type = HLSE_ATTR_OBJECT_INDEX;
        attr[1].value = &index;
        attr[1].valueLen = sizeof(index);
        attr[2].type = HLSE_ATTR_OBJECT_VALUE;
        attr[2].value = (U8 *)&aesRef[indexAesKey]; // &smVal;
        attr[2].valueLen = sizeof(aesRef[indexAesKey]); //sizeof(smVal);

        err = HLSE_CreateObject(attr, templateSize, &handles[indexAesKey]);
#endif
        result &= AX_CHECK_SW(err, HLSE_SW_OK, "err");
        axPrintByteArray("aesRef[indexAesKey]", aesRef[indexAesKey], sizeof(aesRef[indexAesKey]), AX_COLON_32);
    }

#if 0
    {
        // Get the existing aes keys just created
        HLSE_OBJECT_HANDLE handles[10];
        U16 handleNum = sizeof(handles) / sizeof(HLSE_OBJECT_HANDLE);

        err = HLSE_EnumerateObjects(HLSE_SYMMETRIC_KEY, handles, &handleNum);
        result &= AX_CHECK_SW(err, HLSE_SW_OK, "err");
    }
#endif

    // To demonstrate the slots are filled up with keys, do a KDF
    for (indexAesKey=0; indexAesKey<A71CH_SYM_KEY_MAX; indexAesKey++)
    {
        HLSE_HKDF_PARAMS hlseHkdfParams;
        HLSE_MECHANISM_INFO mechInfo;
        PRINTF( "\r\nA71_HkdfExpandSymKey(0x%02x)\r\n", indexAesKey);
        derivedDataLen = sizeof(derivedData);
#if 0
        err = A71_HkdfExpandSymKey((SST_Index_t)indexAesKey, nBlock, info, infoLen, derivedData, derivedDataLen);
#else
        memset(&hlseHkdfParams, 0, sizeof(hlseHkdfParams));
        hlseHkdfParams.nBlock = nBlock;
        hlseHkdfParams.pInfo = info;
        hlseHkdfParams.ulInfoLen = infoLen;

        memset(&mechInfo, 0, sizeof(mechInfo));
        mechInfo.mechanism = HLSE_HKDF_HMAC_SHA256_EXTRACT_AND_EXPAND;
        mechInfo.pParameter = &hlseHkdfParams;
        mechInfo.ulParameterLen = sizeof(hlseHkdfParams);

        err = HLSE_DeriveKey(&mechInfo, handles[indexAesKey], derivedData, &derivedDataLen);
#endif
        result &= AX_CHECK_SW(err, SW_OK, "err");
        axPrintByteArray("derivedData", derivedData, derivedDataLen, AX_COLON_32);
        result &= AX_COMPARE_BYTE_ARRAY("derivedDataRef[indexAesKey]", derivedDataRef[indexAesKey], sizeof(derivedDataRef[indexAesKey]),
                                        "derivedData", derivedData, derivedDataLen, AX_COLON_32);
    }

    // Inserting a wrapped key must succeed (inserting the same key value)
    for (indexAesKey=0; indexAesKey<A71CH_SYM_KEY_MAX; indexAesKey++)
    {
        HLSE_ATTRIBUTE attr;
        PRINTF("\r\nA71_SetRfc3394WrappedAesKey(0x%02X)\r\n", indexAesKey);
#if 0
        err = A71_SetRfc3394WrappedAesKey(indexAesKey, aesRefWrapped[indexAesKey], 24);
#else
        attr.type = HLSE_ATTR_WRAPPED_OBJECT_VALUE;
        attr.value = (U8 *)&aesRefWrapped[indexAesKey];
        attr.valueLen = 24;

        err = HLSE_SetObjectAttribute(handles[indexAesKey], &attr);
#endif
        result &= AX_CHECK_SW(err, SW_OK, "err");
        assert(result);
    }

    // To demonstrate the slots are filled up with the same keys, do a KDF
    for (indexAesKey=0; indexAesKey<A71CH_SYM_KEY_MAX; indexAesKey++)
    {
        HLSE_HKDF_PARAMS hlseHkdfParams;
        HLSE_MECHANISM_INFO mechInfo;
        PRINTF( "\r\nA71_HkdfExpandSymKey(0x%02x)\r\n", indexAesKey);
        derivedDataLen = sizeof(derivedData);
#if 0
        err = A71_HkdfExpandSymKey((SST_Index_t)indexAesKey, nBlock, info, infoLen, derivedData, derivedDataLen);
#else
        memset(&hlseHkdfParams, 0, sizeof(hlseHkdfParams));
        hlseHkdfParams.nBlock = nBlock;
        hlseHkdfParams.pInfo = info;
        hlseHkdfParams.ulInfoLen = infoLen;

        memset(&mechInfo, 0, sizeof(mechInfo));
        mechInfo.mechanism = HLSE_HKDF_HMAC_SHA256_EXTRACT_AND_EXPAND;
        mechInfo.pParameter = &hlseHkdfParams;
        mechInfo.ulParameterLen = sizeof(hlseHkdfParams);

        err = HLSE_DeriveKey(&mechInfo, handles[indexAesKey], derivedData, &derivedDataLen);
#endif
        result &= AX_CHECK_SW(err, SW_OK, "err");
        // axPrintByteArray("derivedData", derivedData, derivedDataLen, AX_COLON_32);
        result &= AX_COMPARE_BYTE_ARRAY("derivedDataRef[indexAesKey]", derivedDataRef[indexAesKey], sizeof(derivedDataRef[indexAesKey]),
                                        "derivedData", derivedData, derivedDataLen, AX_COLON_32);
    }

    // Now replace the key at index 0, with the value of the key at index A71CH_SYM_KEY_MAX-1
    indexWriteKey = A71CH_SYM_KEY_0;
    indexWrapKey = A71CH_SYM_KEY_0;
    PRINTF("\r\nA71_SetRfc3394WrappedAesKey(0x%02X, wrapId=0x%02X)\r\n", indexWriteKey, indexWrapKey);
#if 0
    err = A71_SetRfc3394WrappedAesKey((SST_Index_t)indexWriteKey, aesRefIdxLastWrappedByIdx0, aesRefIdxLastWrappedByIdx0Len);
#else
    {
        HLSE_ATTRIBUTE attr;
        attr.type = HLSE_ATTR_WRAPPED_OBJECT_VALUE;
        attr.value = aesRefIdxLastWrappedByIdx0;
        attr.valueLen = aesRefIdxLastWrappedByIdx0Len;

        err = HLSE_SetObjectAttribute(handles[indexWriteKey], &attr);
    }
#endif
    result &= AX_CHECK_SW(err, SW_OK, "err");
    assert(result);

    // Now do a KDF on the newly stored key at index A71CH_SYM_KEY_0
    // The resulting derived data must match the reference data stored on index A71CH_SYM_KEY_MAX-1
    indexAesKey = indexWriteKey;
    PRINTF( "\r\nA71_HkdfExpandSymKey(0x%02x)\r\n", indexAesKey);
    derivedDataLen = sizeof(derivedData);
#if 0
    err = A71_HkdfExpandSymKey((SST_Index_t)indexAesKey, nBlock, info, infoLen, derivedData, derivedDataLen);
#else
    {
        HLSE_HKDF_PARAMS hlseHkdfParams;
        HLSE_MECHANISM_INFO mechInfo;
        memset(&hlseHkdfParams, 0, sizeof(hlseHkdfParams));
        hlseHkdfParams.nBlock = nBlock;
        hlseHkdfParams.pInfo = info;
        hlseHkdfParams.ulInfoLen = infoLen;

        memset(&mechInfo, 0, sizeof(mechInfo));
        mechInfo.mechanism = HLSE_HKDF_HMAC_SHA256_EXTRACT_AND_EXPAND;
        mechInfo.pParameter = &hlseHkdfParams;
        mechInfo.ulParameterLen = sizeof(hlseHkdfParams);

        err = HLSE_DeriveKey(&mechInfo, handles[indexAesKey], derivedData, &derivedDataLen);
    }
#endif
    result &= AX_CHECK_SW(err, SW_OK, "err");
    axPrintByteArray("derivedData", derivedData, derivedDataLen, AX_COLON_32);
    result &= AX_COMPARE_BYTE_ARRAY("derivedDataRef[A71CH_SYM_KEY_MAX-1]", derivedDataRef[A71CH_SYM_KEY_MAX-1], sizeof(derivedDataRef[A71CH_SYM_KEY_MAX-1]),
                                    "derivedData", derivedData, derivedDataLen, AX_COLON_32);

    PRINTF( "\r\n-----------\r\nEnd exAesRfc3394Precooked(), result = %s\r\n------------\r\n", ((result == 1)? "OK": "FAILED"));

    return result;
}

/**
 * Demonstrate setting wrapped AES keys.
 * Use Host Side crypto to calculate wrapped key values for the keys.
 * Keys are created from random data.
 *
 * @param[in] initMode Visit the documentation of ::a71chInitModule for more information on this parameter
 */
static U8 exAesRfc3394(U8 initMode)
{
    U8 result = 1;
    // U16 state = 0;
    U16 err;

    U8 wrappedKey[24];
    U16 wrappedKeyLen = sizeof(wrappedKey);

    static U8 aesRef[A71CH_SYM_KEY_MAX][16];

    static U8 derivedDataRef[A71CH_SYM_KEY_MAX][32];

    U8 indexAesKey = 0;
    U8 indexWriteKey = 0;
    U8 indexCompareKey = 0;

    U8 info[] = {0x01, 0x02, 0x03, 0x04};
    U16 infoLen = sizeof(info);
    U8 derivedData[32];
    U16 derivedDataLen  = sizeof(derivedData);
    U8 nBlock = 0x01;

    // to hold handles for created sym keys
    HLSE_OBJECT_HANDLE handles[A71CH_SYM_KEY_MAX] = {0};

    PRINTF("\r\n-----------\r\nStart exAesRfc3394(%s)\r\n------------\r\n", getInitModeAsString(initMode));

    // Initialize the A71CH (Debug mode restrictions may apply)
    result &= hlse_a71chInitModule(initMode);

    // Fetch random values from A71CH, use these values as reference keys
    for (indexAesKey=0; indexAesKey<A71CH_SYM_KEY_MAX; indexAesKey++)
    {
        U8 randomLen = 16;
        HLSE_OBJECT_HANDLE moduleHandle = 0;
        U16 moduleHandleNum = 1;
        HLSE_ATTRIBUTE attr;
        sm_printf(CONSOLE, "A71_GetRandom(%d)\r\n", randomLen);
#if 0
        err = A71_GetRandom(aesRef[indexAesKey], randomLen);
#else
        // Get the Module's handle

        err = HLSE_EnumerateObjects(HLSE_MODULE, &moduleHandle, &moduleHandleNum);
        result &= AX_CHECK_SW(err, HLSE_SW_OK, "err");

        attr.type = HLSE_ATTR_MODULE_RANDOM;
        attr.value = aesRef[indexAesKey];
        attr.valueLen = randomLen;

        err = HLSE_GetObjectAttribute(moduleHandle, &attr);
#endif
        result &= AX_CHECK_SW(err, SW_OK, "err");
        axPrintByteArray("aesRef[indexAesKey]", aesRef[indexAesKey], randomLen, AX_COLON_32);
    }

    // Write the key (unwrapped)
    for (indexAesKey=0; indexAesKey<A71CH_SYM_KEY_MAX; indexAesKey++)
    {
        HLSE_OBJECT_INDEX index = indexAesKey;
        HLSE_OBJECT_TYPE objType = HLSE_SYMMETRIC_KEY;
        HLSE_ATTRIBUTE attr[3];
        unsigned short templateSize = 3;
        PRINTF( "\r\nA71_SetSymKey(0x%02x)\r\n", indexAesKey);
#if 0
        err = A71_SetSymKey((SST_Index_t)indexAesKey, aesRef[indexAesKey], sizeof(aesRef[indexAesKey]));
#else

        attr[0].type = HLSE_ATTR_OBJECT_TYPE;
        attr[0].value = &objType;
        attr[0].valueLen = sizeof(objType);
        attr[1].type = HLSE_ATTR_OBJECT_INDEX;
        attr[1].value = &index;
        attr[1].valueLen = sizeof(index);
        attr[2].type = HLSE_ATTR_OBJECT_VALUE;
        attr[2].value = &aesRef[indexAesKey]; // &smVal;
        attr[2].valueLen = sizeof(aesRef[indexAesKey]); //sizeof(smVal);

        err = HLSE_CreateObject(attr, templateSize, &handles[indexAesKey]);
#endif
        result &= AX_CHECK_SW(err, SW_OK, "err");
        axPrintByteArray("aesRef[indexAesKey]", aesRef[indexAesKey], sizeof(aesRef[indexAesKey]), AX_COLON_32);
    }

    // To demonstrate the slots are filled up with keys, do a KDF
    // and store the value for reference
    for (indexAesKey=0; indexAesKey<A71CH_SYM_KEY_MAX; indexAesKey++)
    {
        HLSE_HKDF_PARAMS hlseHkdfParams;
        HLSE_MECHANISM_INFO mechInfo;
        PRINTF( "\r\nA71_HkdfExpandSymKey(0x%02x)\r\n", indexAesKey);
        derivedDataLen = sizeof(derivedData);
#if 0
        err = A71_HkdfExpandSymKey((SST_Index_t)indexAesKey, nBlock, info, infoLen, derivedDataRef[indexAesKey], derivedDataLen);
#else
        memset(&hlseHkdfParams, 0, sizeof(hlseHkdfParams));
        hlseHkdfParams.nBlock = nBlock;
        hlseHkdfParams.pInfo = info;
        hlseHkdfParams.ulInfoLen = infoLen;

        memset(&mechInfo, 0, sizeof(mechInfo));
        mechInfo.mechanism = HLSE_HKDF_HMAC_SHA256_EXTRACT_AND_EXPAND;
        mechInfo.pParameter = &hlseHkdfParams;
        mechInfo.ulParameterLen = sizeof(hlseHkdfParams);

        err = HLSE_DeriveKey(&mechInfo, handles[indexAesKey], derivedDataRef[indexAesKey] /*derivedData*/, &derivedDataLen);
#endif
        result &= AX_CHECK_SW(err, SW_OK, "err");
        axPrintByteArray("derivedData", derivedDataRef[indexAesKey], derivedDataLen, AX_COLON_32);
        // result &= AX_COMPARE_BYTE_ARRAY("derivedDataRef[indexAesKey]", derivedDataRef[indexAesKey], sizeof(derivedDataRef[indexAesKey]),
        //  "derivedData", derivedData, derivedDataLen, AX_COLON_32);
    }

    // Now overwrite the AES keys in reversed order.
    // I.e. the key at the last index will be written on the first index and so on
    // assuming we have for keys, this gives
    // key[0] -> key[3]
    // key[1] -> key[2]
    // key[2] -> key[1]
    // key[3] -> key[0]

    indexWriteKey = A71CH_SYM_KEY_MAX;
    // Now shuffle the keys
    for (indexAesKey=0; indexAesKey<A71CH_SYM_KEY_MAX; indexAesKey++)
    {
        HLSE_ATTRIBUTE attr;
        indexWriteKey--;

        // Calculate the correct wrapped value on the host
        wrappedKeyLen = sizeof(wrappedKey);
#if 1
        err = HOSTCRYPTO_AesWrapKeyRFC3394(aesRef[indexWriteKey], sizeof(aesRef[indexWriteKey]), wrappedKey, &wrappedKeyLen,
                                           aesRef[indexAesKey], sizeof(aesRef[indexAesKey]));
#else
        // TODO : consider using/adding in HostCryptoAPI.h
#endif
        result &= AX_CHECK_SW(err, SW_OK, "err");

        PRINTF("\r\nA71_SetRfc3394WrappedAesKey(0x%02X, ..., wrappedKeyLen=%d)\r\n", indexWriteKey, wrappedKeyLen);
#if 0
        err = A71_SetRfc3394WrappedAesKey((SST_Index_t)indexWriteKey, wrappedKey, wrappedKeyLen);
#else
        attr.type = HLSE_ATTR_WRAPPED_OBJECT_VALUE;
        attr.value = wrappedKey;
        attr.valueLen = wrappedKeyLen;

        err = HLSE_SetObjectAttribute(handles[indexWriteKey], &attr);
#endif
        result &= AX_CHECK_SW(err, SW_OK, "err");
        assert(result);
    }

    // Do a KDF with the reshuffled keys and compare with the reference values calculated above.
    indexCompareKey = A71CH_SYM_KEY_MAX;
    // Now fetch and check the values of the reshuffled keys
    for (indexAesKey=0; indexAesKey<A71CH_SYM_KEY_MAX; indexAesKey++)
    {
        HLSE_HKDF_PARAMS hlseHkdfParams;
        HLSE_MECHANISM_INFO mechInfo;

        indexCompareKey--;

        PRINTF( "\r\nA71_HkdfExpandSymKey(0x%02x)\r\n", indexAesKey);
        derivedDataLen = sizeof(derivedData);
#if 0
        err = A71_HkdfExpandSymKey((SST_Index_t)indexAesKey, nBlock, info, infoLen, derivedData, derivedDataLen);
#else
        memset(&hlseHkdfParams, 0, sizeof(hlseHkdfParams));
        hlseHkdfParams.nBlock = nBlock;
        hlseHkdfParams.pInfo = info;
        hlseHkdfParams.ulInfoLen = infoLen;

        memset(&mechInfo, 0, sizeof(mechInfo));
        mechInfo.mechanism = HLSE_HKDF_HMAC_SHA256_EXTRACT_AND_EXPAND;
        mechInfo.pParameter = &hlseHkdfParams;
        mechInfo.ulParameterLen = sizeof(hlseHkdfParams);

        err = HLSE_DeriveKey(&mechInfo, handles[indexAesKey], derivedData, &derivedDataLen);
#endif
        result &= AX_CHECK_SW(err, SW_OK, "err");
        axPrintByteArray("derivedData", derivedData, derivedDataLen, AX_COLON_32);

        PRINTF("indexCompareKey: 0x%02X\r\n", indexCompareKey);
        axPrintByteArray("derivedDataRef[indexCompareKey]", derivedDataRef[indexCompareKey], sizeof(derivedDataRef[indexCompareKey]), AX_COLON_32);
        axPrintByteArray("derivedData", derivedData, derivedDataLen, AX_COLON_32);

        // Compare with stored reference value
        result &= AX_COMPARE_BYTE_ARRAY("derivedDataRef[indexCompareKey]", derivedDataRef[indexCompareKey], sizeof(derivedDataRef[indexCompareKey]),
                                        "derivedData", derivedData, derivedDataLen, AX_COLON_32);
    }

    PRINTF( "\r\n-----------\r\nEnd exAesRfc3394(), result = %s\r\n------------\r\n", ((result == 1)? "OK": "FAILED"));

    return result;
}

/**
 * Demonstrate 'full' and 'expand only' HKDF functionality.
 * Verify the values returned by values calculated on the host. Depends on availability of Host Crypto
 *
 * @param[in] initMode Visit the documentation of ::a71chInitModule for more information on this parameter
 */
static U8 exSymHkdf(U8 initMode)
{
    U8 result = 1;
    U16 err;

    const U8 aesRef[A71CH_SYM_KEY_MAX_B][16] = {
        {0xDB, 0xFE, 0xE9, 0xE3, 0xB2, 0x76, 0x15, 0x4D, 0x67, 0xF9, 0xD8, 0x4C, 0xB9, 0x35, 0x54, 0x56},
        {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f},
        {0xC0, 0x79, 0xEF, 0x82, 0xCD, 0xF7, 0x12, 0xF2, 0x87, 0x28, 0xFD, 0x18, 0xED, 0xD7, 0xF2, 0xE4},
        {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x06, 0x07, 0x08, 0x09, 0xA0, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4},
        {0x04, 0xFE, 0xE9, 0xE3, 0xB2, 0x76, 0x15, 0x4D, 0x67, 0xF9, 0xD8, 0x4C, 0xB9, 0x35, 0x54, 0x56},
        {0x05, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f},
        {0x06, 0x79, 0xEF, 0x82, 0xCD, 0xF7, 0x12, 0xF2, 0x87, 0x28, 0xFD, 0x18, 0xED, 0xD7, 0xF2, 0xE4},
        {0x07, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x06, 0x07, 0x08, 0x09, 0xA0, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4}
    };

    static U8 salt[A71CH_HKDF_MAX_SALT] = {
        0xAA, 0x1A, 0x2A, 0xE3, 0xB2, 0x76, 0x15, 0x4D, 0x67, 0xF9, 0xD8, 0x4C, 0xB9, 0x35, 0x54, 0x56,
        0xBB, 0x1B, 0x2B, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
    };
    U16 saltLen = sizeof(salt);

    U8 indexAesKey = 0;
    U8 nBlock = 1;

    U8 hostPsk[A71CH_SYM_KEY_MAX*16] = { 0 };
    U16 hostPskLen;
    U8 indexOffset = 0x00;

    U8 derivedData[DERIVE_KEYDATA_FROM_SHARED_SECRET_MAX_DERIVED_DATA] = { 0 };
    U16 derivedDataLen = sizeof(derivedData);

    U8 derivedDataOnHost[DERIVE_KEYDATA_FROM_SHARED_SECRET_MAX_DERIVED_DATA] = { 0 };
    U16 derivedDataOnHostLen = sizeof(derivedDataOnHost);

    U8 info[DERIVE_KEYDATA_FROM_SHARED_SECRET_MAX_INFO] = { 0 };
    U8 infoLen = sizeof(info);

    U16 nReqData = 0;

    int i;

    // to hold handles for created sym keys
    HLSE_OBJECT_HANDLE handles[A71CH_SYM_KEY_MAX] = { 0 };

    PRINTF( "\r\n-----------\r\nStart exSymHkdf(%s)\r\n------------\r\n", getInitModeAsString(initMode));

    // Initialize the A71CH (Debug mode restrictions may apply)
    result &= hlse_a71chInitModule(initMode);
    assert(result);

    if ( a71chScp03Requested(initMode) == 1 )
    {
        derivedDataLen       = A71CH_SCP03_MAX_PAYLOAD_SIZE;
        derivedDataOnHostLen = A71CH_SCP03_MAX_PAYLOAD_SIZE;
    }

    for (indexAesKey=0; indexAesKey<A71CH_SYM_KEY_MAX; indexAesKey++)
    {
        HLSE_OBJECT_INDEX index = indexAesKey;
        HLSE_OBJECT_TYPE objType = HLSE_SYMMETRIC_KEY;
        HLSE_ATTRIBUTE attr[3];
        unsigned short templateSize = 3;
        // Write the key (unwrapped)
        PRINTF( "\r\nA71_SetSymKey(0x%02x)\r\n", indexAesKey);
#if 0
        err = A71_SetSymKey((SST_Index_t)indexAesKey, aesRef[indexAesKey], sizeof(aesRef[indexAesKey]));
#else

        attr[0].type = HLSE_ATTR_OBJECT_TYPE;
        attr[0].value = &objType;
        attr[0].valueLen = sizeof(objType);
        attr[1].type = HLSE_ATTR_OBJECT_INDEX;
        attr[1].value = &index;
        attr[1].valueLen = sizeof(index);
        attr[2].type = HLSE_ATTR_OBJECT_VALUE;
        attr[2].value = (U8 *)&aesRef[indexAesKey]; // &smVal;
        attr[2].valueLen = sizeof(aesRef[indexAesKey]); //sizeof(smVal);

        err = HLSE_CreateObject(attr, templateSize, &handles[indexAesKey]);
#endif
        result &= AX_CHECK_SW(err, SW_OK, "err");
        axPrintByteArray("aesRef[indexAesKey]", aesRef[indexAesKey], sizeof(aesRef[indexAesKey]), AX_COLON_32);
    }

    // Call KDF on Secure Element and compare derived value with value returned by KDF on Host
    // ** Derive data from AES @ A71CH_SYM_KEY_0
    for (i=0; i<DERIVE_KEYDATA_FROM_SHARED_SECRET_MAX_INFO; i++) { info[i] = (U8)i; }
    infoLen = sizeof(info);
    // derivedDataLen = sizeof(derivedData);
    indexAesKey = A71CH_SYM_KEY_0;
    nBlock = 0x01;
    PRINTF("\r\nA71_HkdfExpandSymKey(0x%02x(nBlock=%d), infoLen=%d, derivedDataLen=%d)\r\n", indexAesKey, nBlock, infoLen, derivedDataLen);
#if 0
    err = A71_HkdfExpandSymKey(indexAesKey, nBlock, info, infoLen, derivedData, derivedDataLen);
#else
    {
        HLSE_HKDF_PARAMS hlseHkdfParams;
        HLSE_MECHANISM_INFO mechInfo;
        memset(&hlseHkdfParams, 0, sizeof(hlseHkdfParams));
        hlseHkdfParams.nBlock = nBlock;
        hlseHkdfParams.pInfo = info;
        hlseHkdfParams.ulInfoLen = infoLen;

        memset(&mechInfo, 0, sizeof(mechInfo));
        mechInfo.mechanism = HLSE_HKDF_HMAC_SHA256_EXTRACT_AND_EXPAND;
        mechInfo.pParameter = &hlseHkdfParams;
        mechInfo.ulParameterLen = sizeof(hlseHkdfParams);

        err = HLSE_DeriveKey(&mechInfo, handles[indexAesKey], derivedData, &derivedDataLen);
    }
#endif
    result &= AX_CHECK_SW(err, SW_OK, "err");

    // ** Derive data on Host
    infoLen = sizeof(info);
    // derivedDataOnHostLen = sizeof(derivedDataOnHost);
    PRINTF("\r\nHOSTCRYPTO_HkdfExpandSha256(KeyLen=%d, infoLen=%d, derivedDataLen=%d)\r\n", sizeof(aesRef[indexAesKey]), infoLen, derivedDataOnHostLen);
#if 1
    err = HOSTCRYPTO_HkdfExpandSha256(aesRef[indexAesKey], sizeof(aesRef[indexAesKey]), info, infoLen, derivedDataOnHost, derivedDataOnHostLen);
#else
    // TODO : consider using/adding in HostCryptoAPI.h
#endif
    result &= AX_CHECK_SW(err, SW_OK, "err");
    // Compare both values
    if (memcmp(derivedDataOnHost, derivedData, derivedDataLen) != 0)
    {
        result = 0;
        PRINTF("Calculated HKDF fails to compare with test vector.\r\n");
        axPrintByteArray("derivedDataOnHost", derivedDataOnHost, derivedDataLen, AX_COLON_32);
        axPrintByteArray("derivedData", derivedData, derivedDataLen, AX_COLON_32);
    }

    // Call KDF on Secure Element and compare derived value with value returned by KDF on Host (Reduce infoLen & nReqData)
    // ** Derive data from AES @ A71CH_SYM_KEY_0
    for (i=0; i<DERIVE_KEYDATA_FROM_SHARED_SECRET_MAX_INFO; i++) { info[i] = (U8)i; }
    infoLen = 42;
    nReqData = 72;
    indexAesKey = A71CH_SYM_KEY_0;
    nBlock = 0x01;
    PRINTF("\r\nA71_HkdfExpandSymKey(0x%02x(nBlock=%d), infoLen=%d, derivedDataLen=%d)\r\n", indexAesKey, nBlock, infoLen, derivedDataLen);
#if 0
    err = A71_HkdfExpandSymKey(indexAesKey, nBlock, info, infoLen, derivedData, derivedDataLen);
#else
    {
        HLSE_HKDF_PARAMS hlseHkdfParams;
        HLSE_MECHANISM_INFO mechInfo;

        memset(&hlseHkdfParams, 0, sizeof(hlseHkdfParams));
        hlseHkdfParams.nBlock = nBlock;
        hlseHkdfParams.pInfo = info;
        hlseHkdfParams.ulInfoLen = infoLen;

        memset(&mechInfo, 0, sizeof(mechInfo));
        mechInfo.mechanism = HLSE_HKDF_HMAC_SHA256_EXTRACT_AND_EXPAND;
        mechInfo.pParameter = &hlseHkdfParams;
        mechInfo.ulParameterLen = sizeof(hlseHkdfParams);

        err = HLSE_DeriveKey(&mechInfo, handles[indexAesKey], derivedData, &derivedDataLen);
    }
#endif
    result &= AX_CHECK_SW(err, SW_OK, "err");

    // ** Derive data on Host
    // Re-use infoLen and nReqData values just set
    PRINTF("\r\nHOSTCRYPTO_HkdfExpandSha256(KeyLen=%d, infoLen=%d, derivedDataLen=%d)\r\n", sizeof(aesRef[indexAesKey]), infoLen, nReqData);
#if 1
    err = HOSTCRYPTO_HkdfExpandSha256(aesRef[indexAesKey], sizeof(aesRef[indexAesKey]), info, infoLen, derivedDataOnHost, nReqData);
#else
    // TODO : consider using/adding in HostCryptoAPI.h
#endif
    result &= AX_CHECK_SW(err, SW_OK, "err");
    // Compare both values
    if (memcmp(derivedDataOnHost, derivedData, nReqData) != 0)
    {
        result = 0;
        PRINTF("Calculated HKDF fails to compare with test vector.\r\n");
        axPrintByteArray("derivedDataOnHost", derivedDataOnHost, nReqData, AX_COLON_32);
        axPrintByteArray("derivedData", derivedData, nReqData, AX_COLON_32);
    }

    // Check on all possible sizes (Expand only)
    for (indexOffset = 0x00; indexOffset < A71CH_SYM_KEY_MAX; indexOffset++)
    {
        U8 nBlockMax = ( (A71CH_SYM_KEY_MAX-indexOffset) > A71CH_SYM_KEY_COMBINED_MAX ) ? A71CH_SYM_KEY_COMBINED_MAX : (A71CH_SYM_KEY_MAX-indexOffset);
        for (nBlock=1; nBlock<=nBlockMax; nBlock++)
        {
            int j;
            HLSE_MECHANISM_INFO mechInfo;
            HLSE_HKDF_PARAMS hlseHkdfParams;

            // Call A71_HkdfExpandSymKey on Secure Element and compare derived key with value returned by KDF on Host
            // ** Shared secret is nBlock * 16 byte long and located in (starts at) AES-STORE @ [A71CH_SYM_KEY_0 + indexOffset]
            indexAesKey = A71CH_SYM_KEY_0 + indexOffset;
            // derivedDataLen = sizeof(derivedData);
            PRINTF("\r\nA71_HkdfExpandSymKey(0x%02x(nBlock=%d), infoLen=%d, derivedDataLen=%d)\r\n", indexAesKey, nBlock, infoLen, derivedDataLen);
#if 0
            err = A71_HkdfExpandSymKey(indexAesKey, nBlock, info, infoLen, derivedData, derivedDataLen);
#else
            memset(&hlseHkdfParams, 0, sizeof(hlseHkdfParams));
            hlseHkdfParams.nBlock = nBlock;
            hlseHkdfParams.pInfo = info;
            hlseHkdfParams.ulInfoLen = infoLen;

            memset(&mechInfo, 0, sizeof(mechInfo));
            mechInfo.mechanism = HLSE_HKDF_HMAC_SHA256_EXTRACT_AND_EXPAND;
            mechInfo.pParameter = &hlseHkdfParams;
            mechInfo.ulParameterLen = sizeof(hlseHkdfParams);

            err = HLSE_DeriveKey(&mechInfo, handles[indexAesKey], derivedData, &derivedDataLen);
#endif
            result &= AX_CHECK_SW(err, SW_OK, "err");

            // ** Calculate master secret on Host
            // *** First create pre-master secret based upon PSK
            for (j=0; j<nBlock; j++)
            {
                memcpy(&hostPsk[j*16], &aesRef[indexAesKey+j], sizeof(aesRef[indexAesKey+j]));
            }
            hostPskLen = nBlock*sizeof(aesRef[indexAesKey]);
            axPrintByteArray("hostPsk", hostPsk, nBlock*sizeof(aesRef[indexAesKey]), AX_COLON_32);
            // derivedDataOnHostLen = sizeof(derivedDataOnHost);
            PRINTF("\r\nHOSTCRYPTO_HkdfExpandSha256(KeyLen=%d, infoLen=%d, derivedDataLen=%d)\r\n", hostPskLen, infoLen, derivedDataOnHostLen);
#if 1
            err = HOSTCRYPTO_HkdfExpandSha256(hostPsk, hostPskLen, info, infoLen, derivedDataOnHost, derivedDataOnHostLen);
#else
            // TODO : consider using/adding in HostCryptoAPI.h
#endif
            result &= AX_CHECK_SW(err, SW_OK, "err");

            // Compare both values
            if (memcmp(derivedDataOnHost, derivedData, derivedDataLen) != 0)
            {
                result = 0;
                PRINTF("Calculated HKDF fails to compare with test vector.\r\n");
                axPrintByteArray("derivedDataOnHost", derivedDataOnHost, derivedDataLen, AX_COLON_32);
                axPrintByteArray("derivedData", derivedData, derivedDataLen, AX_COLON_32);
            }
        }
    }

    // Check on all possible sizes (Full HKDF)
    for (indexOffset = 0x00; indexOffset < A71CH_SYM_KEY_MAX; indexOffset++)
    {
        U8 nBlockMax = ( (A71CH_SYM_KEY_MAX-indexOffset) > A71CH_SYM_KEY_COMBINED_MAX ) ? A71CH_SYM_KEY_COMBINED_MAX : (A71CH_SYM_KEY_MAX-indexOffset);
        for (nBlock=1; nBlock<=nBlockMax; nBlock++)
        {
            int j;
            HLSE_HKDF_PARAMS hlseHkdfParams;
            HLSE_MECHANISM_INFO mechInfo;

            // Call A71_HkdfSymKey on Secure Element and compare derived key with value returned by KDF on Host
            // ** Shared secret is nBlock * 16 byte long and located in (starts at) AES-STORE @ [A71CH_SYM_KEY_0 + indexOffset]
            indexAesKey = A71CH_SYM_KEY_0 + indexOffset;
            // derivedDataLen = sizeof(derivedData);
            PRINTF("\r\nA71_HkdfSymKey(0x%02x(nBlock=%d), saltLen=%d, infoLen=%d, derivedDataLen=%d)\r\n", indexAesKey, nBlock,
                   saltLen, infoLen, derivedDataLen);
#if 0
            err = A71_HkdfSymKey(indexAesKey, nBlock, salt, saltLen, info, infoLen, derivedData, derivedDataLen);
#else
            hlseHkdfParams.nBlock = nBlock;
            hlseHkdfParams.pSalt = salt;
            hlseHkdfParams.ulSaltLen = saltLen;
            hlseHkdfParams.pInfo = info;
            hlseHkdfParams.ulInfoLen = infoLen;

            memset(&mechInfo, 0, sizeof(mechInfo));
            mechInfo.mechanism = HLSE_HKDF_HMAC_SHA256_SKIP_EXTRACT;
            mechInfo.pParameter = &hlseHkdfParams;
            mechInfo.ulParameterLen = sizeof(hlseHkdfParams);

            err = HLSE_DeriveKey(&mechInfo, handles[indexAesKey], derivedData, &derivedDataLen);
#endif
            result &= AX_CHECK_SW(err, SW_OK, "err");

            // ** Calculate master secret on Host
            // *** First create pre-master secret based upon PSK
            for (j=0; j<nBlock; j++)
            {
                memcpy(&hostPsk[j*16], &aesRef[indexAesKey+j], sizeof(aesRef[indexAesKey+j]));
            }
            hostPskLen = nBlock*sizeof(aesRef[indexAesKey]);
            axPrintByteArray("hostPsk", hostPsk, nBlock*sizeof(aesRef[indexAesKey]), AX_COLON_32);
            // derivedDataOnHostLen = sizeof(derivedDataOnHost);
            PRINTF("\r\nHOSTCRYPTO_HkdfFullSha256(KeyLen=%d, infoLen=%d, derivedDataLen=%d)\r\n", hostPskLen, infoLen, derivedDataOnHostLen);
#if 1
            err = HOSTCRYPTO_HkdfFullSha256(salt, saltLen, hostPsk, hostPskLen, info, infoLen, derivedDataOnHost, derivedDataOnHostLen);
#else
            // TODO : consider using/adding in HostCryptoAPI.h
#endif
            result &= AX_CHECK_SW(err, SW_OK, "err");

            // Compare both values
            if (memcmp(derivedDataOnHost, derivedData, derivedDataLen) != 0)
            {
                result = 0;
                PRINTF("Calculated HKDF fails to compare with test vector.\r\n");
                axPrintByteArray("derivedDataOnHost", derivedDataOnHost, derivedDataLen, AX_COLON_32);
                axPrintByteArray("derivedData", derivedData, derivedDataLen, AX_COLON_32);
            }
        }
    }

    return result;
}

/**
 * Demonstrate HmacSha256 functionality.
 * Verify the values returned by values calculated on the host. Depends on availability of Host Crypto
 * - First loop always uses key size of 128 bits
 * - Second loop explores using different key sizes
 *
 * @param[in] initMode Visit the documentation of ::a71chInitModule for more information on this parameter
 */
static U8 exSymHmacSha256(U8 initMode)
{
    U8 result = 1;
    U16 err;

    const U8 aesRef[A71CH_SYM_KEY_MAX_B][16] = {
        {0xDB, 0xFE, 0xE9, 0xE3, 0xB2, 0x76, 0x15, 0x4D, 0x67, 0x00, 0x10, 0x20, 0xB9, 0x35, 0x54, 0x56},
        {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x01, 0x11, 0x21, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f},
        {0xC0, 0x79, 0xEF, 0x82, 0xCD, 0xF7, 0x12, 0x02, 0x12, 0x22, 0xFD, 0x18, 0xED, 0xD7, 0xF2, 0xE4},
        {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x03, 0x13, 0x23, 0x09, 0xA0, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4},
        {0x04, 0xFE, 0xE9, 0xE3, 0xB2, 0x76, 0x15, 0x4D, 0x67, 0x00, 0x10, 0x20, 0xB9, 0x35, 0x54, 0x56},
        {0x05, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x01, 0x11, 0x21, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f},
        {0x06, 0x79, 0xEF, 0x82, 0xCD, 0xF7, 0x12, 0x02, 0x12, 0x22, 0xFD, 0x18, 0xED, 0xD7, 0xF2, 0xE4},
        {0x07, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x03, 0x13, 0x23, 0x09, 0xA0, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4}
    };

    U8 data[1024];

    U16 reqDataLenArray[] = {100, 200, 220, A71CH_HMAC_SHA256_MAX_DATA_CHUNK-1,
                             A71CH_HMAC_SHA256_MAX_DATA_CHUNK, A71CH_HMAC_SHA256_MAX_DATA_CHUNK+1,
                             A71CH_HMAC_SHA256_MAX_DATA_CHUNK*2, A71CH_HMAC_SHA256_MAX_DATA_CHUNK*2 + 1,
                             A71CH_HMAC_SHA256_MAX_DATA_CHUNK*3, A71CH_HMAC_SHA256_MAX_DATA_CHUNK*3 + 1};

    U16 nReqDataMax = sizeof(reqDataLenArray) / sizeof(U16);
    U16 nReqData = 0;
    U16 reqDataLen = 0;

    U8 indexAesKey = 0;
    U8 nBlock = 1;

    U8 hostPsk[A71CH_SYM_KEY_MAX*16];
    U16 hostPskLen;
    U8 indexOffset = 0x00;

    U8 hmac[32];
    U16 hmacLen = sizeof(hmac);

    U8 hmacOnHost[32];
    U16 hmacOnHostLen = sizeof(hmacOnHost);

    // to hold handles for created sym keys
    HLSE_OBJECT_HANDLE handles[A71CH_SYM_KEY_MAX] = {0};

    int i;

    PRINTF( "\r\n-----------\r\nStart exSymHmacSha256(%s)\r\n------------\r\n", getInitModeAsString(initMode));

    // Initialize the A71CH (Debug mode restrictions may apply)
    result &= hlse_a71chInitModule(initMode);

    for (indexAesKey=0; indexAesKey<A71CH_SYM_KEY_MAX; indexAesKey++)
    {
        HLSE_OBJECT_INDEX index = indexAesKey;
        HLSE_OBJECT_TYPE objType = HLSE_SYMMETRIC_KEY;
        HLSE_ATTRIBUTE attr[3];
        unsigned short templateSize = 3;

        // Write the key (unwrapped)
        PRINTF( "\r\nA71_SetSymKey(0x%02x)\r\n", indexAesKey);
#if 0
        err = A71_SetSymKey((SST_Index_t)indexAesKey, aesRef[indexAesKey], sizeof(aesRef[indexAesKey]));
#else

        attr[0].type = HLSE_ATTR_OBJECT_TYPE;
        attr[0].value = &objType;
        attr[0].valueLen = sizeof(objType);
        attr[1].type = HLSE_ATTR_OBJECT_INDEX;
        attr[1].value = &index;
        attr[1].valueLen = sizeof(index);
        attr[2].type = HLSE_ATTR_OBJECT_VALUE;
        attr[2].value = (U8 *)&aesRef[indexAesKey]; // &smVal;
        attr[2].valueLen = sizeof(aesRef[indexAesKey]); //sizeof(smVal);

        err = HLSE_CreateObject(attr, templateSize, /*&objHandle*/ &handles[indexAesKey]);
#endif
        result &= AX_CHECK_SW(err, SW_OK, "err");
        axPrintByteArray("aesRef[indexAesKey]", aesRef[indexAesKey], sizeof(aesRef[indexAesKey]), AX_COLON_32);
    }

    for (i=0; i<sizeof(data); i++) { data[i] = (U8)i; }

    // Simple HMAC example
    // ** Calculate HMAC on A71CH - Use AES @ A71CH_SYM_KEY_0 (128 bit)
    // Try out different sizes of 'data' that will be HMACd
    indexAesKey = A71CH_SYM_KEY_0;
    nBlock = 0x01;
    for (nReqData = 0; nReqData < nReqDataMax; nReqData++)
    {
        HLSE_HKDF_PARAMS hlseHmacParams;
        HLSE_MECHANISM_INFO mechInfo;
        reqDataLen = reqDataLenArray[nReqData];
        PRINTF( "\r\nA71_GetHmacSha256(0x%02x (nBlock=%d) dataLen=%d)\r\n", indexAesKey, nBlock, reqDataLen);
#if 0
        err = A71_GetHmacSha256(indexAesKey, nBlock, data, reqDataLen, hmac, &hmacLen);
#else
        memset(&hlseHmacParams, 0, sizeof(hlseHmacParams));
        hlseHmacParams.nBlock = nBlock;

        memset(&mechInfo, 0, sizeof(mechInfo));
        mechInfo.mechanism = HLSE_HMAC_AES_SHA256;
        mechInfo.pParameter = &hlseHmacParams;
        mechInfo.ulParameterLen = sizeof(hlseHmacParams);

        err = HLSE_Sign(&mechInfo, handles[indexAesKey], data, reqDataLen, hmac, &hmacLen);
#endif
        result &= AX_CHECK_SW(err, SW_OK, "A71_GetHmacSha256 failed");
        if (err == SW_OK)
        {
            axPrintByteArray("hmac", hmac, hmacLen, AX_COLON_32);

            hmacOnHostLen = sizeof(hmacOnHost);
            // ** Calculate the same HMAC on Host
#if 1
            err = HOSTCRYPTO_HmacSha256(aesRef[indexAesKey], sizeof(aesRef[indexAesKey]), data, reqDataLen, hmacOnHost);
#else
            // TODO : consider using/adding in HostCryptoAPI.h
#endif
            result &= AX_CHECK_SW(err, SW_OK, "HOSTCRYPTO_HmacSha256 failed");
            if (err == SW_OK)
            {
                axPrintByteArray("hmacOnHost", hmacOnHost, hmacOnHostLen, AX_COLON_32);
            }

            // Compare both values
            if (memcmp(hmacOnHost, hmac, hmacOnHostLen) != 0)
            {
                result = 0;
                PRINTF("Calculated HMAC's fail to compare.\r\n");
                axPrintByteArray("hmacOnHost", hmacOnHost, hmacOnHostLen, AX_COLON_32);
                axPrintByteArray("hmac", hmac, hmacLen, AX_COLON_32);
            }
        }
    }

    // Check out HMAC for all possible stored secrets
    // Check on all possible key sizes; data to hash fixed on 100 byte
    for (indexOffset = 0x00; indexOffset < A71CH_SYM_KEY_MAX; indexOffset++)
    {
        U8 nBlockMax = ( (A71CH_SYM_KEY_MAX-indexOffset) > A71CH_SYM_KEY_COMBINED_MAX ) ? A71CH_SYM_KEY_COMBINED_MAX : (A71CH_SYM_KEY_MAX-indexOffset);
        for (nBlock=1; nBlock<=nBlockMax; nBlock++)
        {
            int j;
            HLSE_HKDF_PARAMS hlseHmacParams;
            HLSE_MECHANISM_INFO mechInfo;

            // Call A71_GetHmacSha256 on Secure Element and compare calculated hmac with value returned by HMAC on Host
            // ** Shared secret is nBlock * 16 byte long and located in (starts at) AES-STORE @ [A71CH_SYM_KEY_0 + indexOffset]
            reqDataLen = 100;
            indexAesKey = A71CH_SYM_KEY_0 + indexOffset;
            PRINTF( "\r\nA71_GetHmacSha256(0x%02x (nBlock=%d))\r\n", indexAesKey, nBlock);
#if 0
            err = A71_GetHmacSha256(indexAesKey, nBlock, data, reqDataLen, hmac, &hmacLen);
#else
            memset(&hlseHmacParams, 0, sizeof(hlseHmacParams));
            hlseHmacParams.nBlock = nBlock;

            memset(&mechInfo, 0, sizeof(mechInfo));
            mechInfo.mechanism = HLSE_HMAC_AES_SHA256;
            mechInfo.pParameter = &hlseHmacParams;
            mechInfo.ulParameterLen = sizeof(hlseHmacParams);

            err = HLSE_Sign(&mechInfo, handles[indexAesKey], data, reqDataLen, hmac, &hmacLen);
#endif
            result &= AX_CHECK_SW(err, SW_OK, "A71_GetHmacSha256 failed");

            if (err == SW_OK)
            {
                // ** Calculate the same HMAC on Host
                // *** First create secret ready for use by HMAC on Host
                for (j=0; j<nBlock; j++)
                {
                    memcpy(&hostPsk[j*16], &aesRef[indexAesKey+j], sizeof(aesRef[indexAesKey+j]));
                }
                hostPskLen = nBlock*sizeof(aesRef[indexAesKey]);
                hmacOnHostLen = sizeof(hmacOnHost);
#if 1
                err = HOSTCRYPTO_HmacSha256(hostPsk, hostPskLen, data, reqDataLen, hmacOnHost);
#else
                // TODO : consider using/adding in HostCryptoAPI.h
#endif
                result &= AX_CHECK_SW(err, SW_OK, "HOSTCRYPTO_HmacSha256 failed");

                // Compare both values
                if (memcmp(hmacOnHost, hmac, hmacOnHostLen) != 0)
                {
                    result = 0;
                    PRINTF("Calculated HMAC's fail to compare.\r\n");
                    axPrintByteArray("hmacOnHost", hmacOnHost, hmacOnHostLen, AX_COLON_32);
                    axPrintByteArray("hmac", hmac, hmacLen, AX_COLON_32);
                }
            }
        }
    }

    return result;
}
