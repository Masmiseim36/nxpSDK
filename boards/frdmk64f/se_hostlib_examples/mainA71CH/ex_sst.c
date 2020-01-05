/**
 * @file ex_sst.c
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
 * Example invocation of secure storage specific functionality of the A71CH
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "a71ch_ex.h"
#include "a71_debug.h"
#include "sm_types.h"
#include "sm_apdu.h"
#include "ax_util.h"
#include "tst_sm_util.h"
#include "tst_a71ch_util.h"
#include "tstHostCrypto.h"

#include "HostCryptoAPI.h"
static U8 exSstSym(U8 initMode);
static U8 exSstPub(U8 initMode);

/**
 * Demonstrate storage of symmetric keys:
 * - ::exSstSym
 *
 * Demonstrate storage of public keys:
 * - ::exSstPub
 *
 */
U8 exSst()
{
    U8 result = 1;
    PRINTF( "\r\n-----------\r\nStart exSst()\r\n------------\r\n");

    DEV_ClearChannelState();

    // No channel encryption
    // ---------------------
    // - Sym keys
    result &= exSstSym(INIT_MODE_RESET);

    // - Public keys
    result &= exSstPub(INIT_MODE_RESET);

    // Using channel encryption
    // ------------------------
    // - Sym keys
    result &= exSstSym(INIT_MODE_RESET_DO_SCP03);

    // - Public keys
    result &= exSstPub(INIT_MODE_RESET_DO_SCP03);

    // overall result
    PRINTF( "\r\n-----------\r\nEnd exSst(), result = %s\r\n------------\r\n", ((result == 1)? "OK": "FAILED"));

    return result;
}

/**
 * Demonstrate
 * - setting/erasing/freezing of symmetric keys
 * - blocking injection of unwrapped symmetric keys
 *
 * @param[in]  initMode       Visit the documentation of ::a71chInitModule for
 *                            more information on this parameter
 * @param[in]  appletVersion  The applet version
 *
 * @return     1 if successful.
 */
static U8 exSstSym(U8 initMode)
{
    U8 result = 1;
    U16 err;
    U8 info[] = {0x01, 0x02, 0x03, 0x04};
    U16 infoLen = sizeof(info);
    U8 derivedData[32];
    U16 derivedDataLen  = sizeof(derivedData);

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

#if 0
    // ddDataAfterUpdate used as reference
    static U8 ddDataAfterUpdate[A71CH_SYM_KEY_MAX_B][32] = {
        {0x36, 0x54, 0xBE, 0x49, 0x32, 0xB6, 0xE3, 0x84, 0xB8, 0x0C, 0xF5, 0xFC, 0xF7, 0x7B, 0x93, 0x93,
         0x82, 0x59, 0xD0, 0x45, 0x7E, 0xED, 0xE3, 0xB8, 0x6F, 0xC0, 0x91, 0x99, 0x67, 0x10, 0xA4, 0xF3},
        {0x9E, 0xA8, 0xD4, 0x14, 0xE2, 0x9A, 0x9A, 0x42, 0x2B, 0x45, 0x65, 0x21, 0xA1, 0x08, 0xA9, 0x85,
         0x97, 0x52, 0x03, 0xC3, 0x15, 0x23, 0xC0, 0xE2, 0x05, 0x86, 0x40, 0x8C, 0xE3, 0xAF, 0x39, 0x7F},
        {0x48, 0x37, 0x34, 0x13, 0x78, 0x8C, 0x4E, 0xFB, 0x10, 0xD6, 0xC7, 0xE1, 0xD8, 0xB7, 0x70, 0xE4,   // Updated
         0xA0, 0xF8, 0x57, 0x0D, 0xA6, 0xFF, 0x79, 0xA3, 0xE7, 0xBD, 0xB6, 0xF6, 0x5A, 0xB5, 0x2B, 0xD0},
        {0xDE, 0x12, 0xE2, 0x55, 0xB8, 0x3E, 0x37, 0xC1, 0xAB, 0xBF, 0x3D, 0xD8, 0x58, 0xFE, 0x6C, 0x2E,   // Updated
         0xAB, 0xE2, 0x71, 0xA1, 0xEC, 0x12, 0xC4, 0xDE, 0x05, 0x95, 0xD7, 0x84, 0x22, 0xA5, 0xFB, 0x76},
        {0x36, 0x54, 0xBE, 0x49, 0x32, 0xB6, 0xE3, 0x84, 0xB8, 0x0C, 0xF5, 0xFC, 0xF7, 0x7B, 0x93, 0x93, // First 4 repeated
         0x82, 0x59, 0xD0, 0x45, 0x7E, 0xED, 0xE3, 0xB8, 0x6F, 0xC0, 0x91, 0x99, 0x67, 0x10, 0xA4, 0xF3},
        {0x9E, 0xA8, 0xD4, 0x14, 0xE2, 0x9A, 0x9A, 0x42, 0x2B, 0x45, 0x65, 0x21, 0xA1, 0x08, 0xA9, 0x85,
         0x97, 0x52, 0x03, 0xC3, 0x15, 0x23, 0xC0, 0xE2, 0x05, 0x86, 0x40, 0x8C, 0xE3, 0xAF, 0x39, 0x7F},
        {0x48, 0x37, 0x34, 0x13, 0x78, 0x8C, 0x4E, 0xFB, 0x10, 0xD6, 0xC7, 0xE1, 0xD8, 0xB7, 0x70, 0xE4,   // Updated
         0xA0, 0xF8, 0x57, 0x0D, 0xA6, 0xFF, 0x79, 0xA3, 0xE7, 0xBD, 0xB6, 0xF6, 0x5A, 0xB5, 0x2B, 0xD0},
        {0xDE, 0x12, 0xE2, 0x55, 0xB8, 0x3E, 0x37, 0xC1, 0xAB, 0xBF, 0x3D, 0xD8, 0x58, 0xFE, 0x6C, 0x2E,   // Updated
         0xAB, 0xE2, 0x71, 0xA1, 0xEC, 0x12, 0xC4, 0xDE, 0x05, 0x95, 0xD7, 0x84, 0x22, 0xA5, 0xFB, 0x76}
    };
#endif

    static U8 ddDataAfterUpdate[A71CH_SYM_KEY_MAX_B][32] = {
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

    U8 indexAesKey = 0;
    U8 nBlock = 0x01;

    PRINTF("\r\n-----------\r\nStart exSstSym(%s)\r\n------------\r\n", getInitModeAsString(initMode));

    // Initialize the A71CH (Debug mode restrictions may apply)
    result &= a71chInitModule(initMode);

    // Fill up the symmetric key store with reference values (straight, no wrapping)
    for (indexAesKey=0; indexAesKey<A71CH_SYM_KEY_MAX; indexAesKey++)
    {
        PRINTF( "\r\nA71_SetSymKey(0x%02x)\r\n", indexAesKey);
        err = A71_SetSymKey((SST_Index_t)indexAesKey, aesRef[indexAesKey], sizeof(aesRef[indexAesKey]));
        result &= AX_CHECK_SW(err, SW_OK, "err");
        axPrintByteArray("aesRef[indexAesKey]", aesRef[indexAesKey], sizeof(aesRef[indexAesKey]), AX_COLON_32);
    }

    // To demonstrate the slots are filled up with keys, do a KDF
    for (indexAesKey=0; indexAesKey<A71CH_SYM_KEY_MAX; indexAesKey++)
    {
        PRINTF( "\r\nA71_HkdfExpandSymKey(0x%02x)\r\n", indexAesKey);
        derivedDataLen = sizeof(derivedData);
        err = A71_HkdfExpandSymKey((SST_Index_t)indexAesKey, nBlock, info, infoLen, derivedData, derivedDataLen);
        result &= AX_CHECK_SW(err, SW_OK, "err");
        axPrintByteArray("derivedData", derivedData, derivedDataLen, AX_COLON_32);
        result &= AX_COMPARE_BYTE_ARRAY("derivedDataRef[indexAesKey]", derivedDataRef[indexAesKey], sizeof(derivedDataRef[indexAesKey]),
            "derivedData", derivedData, derivedDataLen, AX_COLON_32);
    }

    // Erase the symmetric key at index 1 & verify the value is no longer useable
    // ** Erase **
    indexAesKey = A71CH_SYM_KEY_1;
    PRINTF("\r\nA71_EraseSymKey(index=0x%02X)\r\n", indexAesKey);
    err = A71_EraseSymKey(indexAesKey);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    // ** Verify value is no longer useable **
    PRINTF( "\r\nA71_HkdfExpandSymKey(0x%02x)\r\n", indexAesKey);
    derivedDataLen = sizeof(derivedData);
    err = A71_HkdfExpandSymKey((SST_Index_t)indexAesKey, nBlock, info, infoLen, derivedData, derivedDataLen);
    result &= AX_CHECK_SW(err, SW_CONDITIONS_NOT_SATISFIED, "Expected to fail as the key was erased");

    // Fill in the original value again
    indexAesKey = A71CH_SYM_KEY_1;
    PRINTF( "\r\nA71_SetSymKey(0x%02x)\r\n", indexAesKey);
    err = A71_SetSymKey((SST_Index_t)indexAesKey, aesRef[indexAesKey], sizeof(aesRef[indexAesKey]));
    result &= AX_CHECK_SW(err, SW_OK, "err");


    // Slots are filled up with known keys, do a KDF as proof
    for (indexAesKey=0; indexAesKey<A71CH_SYM_KEY_MAX; indexAesKey++)
    {
        PRINTF( "\r\nA71_HkdfExpandSymKey(0x%02x)\r\n", indexAesKey);
        derivedDataLen = sizeof(derivedData);
        err = A71_HkdfExpandSymKey((SST_Index_t)indexAesKey, nBlock, info, infoLen, derivedData, derivedDataLen);
        result &= AX_CHECK_SW(err, SW_OK, "err");
        // axPrintByteArray("derivedData", derivedData, derivedDataLen, AX_COLON_32);
        result &= AX_COMPARE_BYTE_ARRAY("derivedDataRef[indexAesKey]", derivedDataRef[indexAesKey], sizeof(derivedDataRef[indexAesKey]),
            "derivedData", derivedData, derivedDataLen, AX_COLON_32);
    }

    // Overwrite the second half
    for (indexAesKey=A71CH_SYM_KEY_MAX>>1; indexAesKey<A71CH_SYM_KEY_MAX; indexAesKey++)
    {
        U8 tmpKey[16];
        int j;

        for (j=0; j<16; j++) {tmpKey[j] = (U8)indexAesKey;}
        PRINTF( "\r\nA71_SetSymKey(0x%02x)\r\n", indexAesKey);
        err = A71_SetSymKey((SST_Index_t)indexAesKey, tmpKey, sizeof(tmpKey));
        result &= AX_CHECK_SW(err, SW_OK, "err");

        // Calculate and store the expected derivedData based on the new sym keys written.
        err = HOSTCRYPTO_HkdfExpandSha256(tmpKey, sizeof(tmpKey), info, infoLen, ddDataAfterUpdate[indexAesKey], sizeof(ddDataAfterUpdate[indexAesKey]));
        result &= AX_CHECK_SW(err, SW_OK, "err");
    }

    // Check that the second half was overwritten (compare with another set of reference data)
    for (indexAesKey=A71CH_SYM_KEY_MAX>>1; indexAesKey<A71CH_SYM_KEY_MAX; indexAesKey++)
    {
        PRINTF( "\r\nA71_HkdfExpandSymKey(0x%02x)\r\n", indexAesKey);
        derivedDataLen = sizeof(derivedData);
        err = A71_HkdfExpandSymKey((SST_Index_t)indexAesKey, nBlock, info, infoLen, derivedData, derivedDataLen);
        result &= AX_CHECK_SW(err, SW_OK, "err");
        // axPrintByteArray("derivedData", derivedData, derivedDataLen, AX_CARRAY_16);
        result &= AX_COMPARE_BYTE_ARRAY("ddDataAfterUpdate[indexAesKey]", ddDataAfterUpdate[indexAesKey], sizeof(ddDataAfterUpdate[indexAesKey]),
            "derivedData", derivedData, derivedDataLen, AX_COLON_32);
    }

    // Put back the original (reference) values in the second half
    // Fill up the symmetric key store with reference values (straight, no wrapping)
    for (indexAesKey=A71CH_SYM_KEY_MAX>>1; indexAesKey<A71CH_SYM_KEY_MAX; indexAesKey++)
    {
        PRINTF( "\r\nA71_SetSymKey(0x%02x)\r\n", indexAesKey);
        err = A71_SetSymKey((SST_Index_t)indexAesKey, aesRef[indexAesKey], sizeof(aesRef[indexAesKey]));
        result &= AX_CHECK_SW(err, SW_OK, "err");
        axPrintByteArray("aesRef[indexAesKey]", aesRef[indexAesKey], sizeof(aesRef[indexAesKey]), AX_COLON_32);
    }
    // Slots are filled up with known keys, do a KDF as proof
    for (indexAesKey=0; indexAesKey<A71CH_SYM_KEY_MAX; indexAesKey++)
    {
        PRINTF( "\r\nA71_HkdfExpandSymKey(0x%02x)\r\n", indexAesKey);
        derivedDataLen = sizeof(derivedData);
        err = A71_HkdfExpandSymKey((SST_Index_t)indexAesKey, nBlock, info, infoLen, derivedData, derivedDataLen);
        result &= AX_CHECK_SW(err, SW_OK, "err");
        // axPrintByteArray("derivedData", derivedData, derivedDataLen, AX_COLON_32);
        result &= AX_COMPARE_BYTE_ARRAY("derivedDataRef[indexAesKey]", derivedDataRef[indexAesKey], sizeof(derivedDataRef[indexAesKey]),
            "derivedData", derivedData, derivedDataLen, AX_COLON_32);
    }

    // Now disable the plain insertion of Symmetric keys
    PRINTF("\r\nA71_InjectLock()\r\n");
    err = A71_InjectLock();
    result &= AX_CHECK_SW(err, SW_OK, "err");
    assert(result);

    // Inserting a key in second half - plain mode - must fail.
    for (indexAesKey=A71CH_SYM_KEY_MAX>>1; indexAesKey<A71CH_SYM_KEY_MAX; indexAesKey++)
    {
        U8 tmpKey[16];
        int j;

        for (j=0; j<16; j++) {tmpKey[j] = (U8)indexAesKey;}
        PRINTF( "\r\nA71_SetSymKey(0x%02x)\r\n", indexAesKey);
        err = A71_SetSymKey((SST_Index_t)indexAesKey, tmpKey, sizeof(tmpKey));
        result &= AX_CHECK_SW(err, SW_COMMAND_NOT_ALLOWED, "Expected to fail. Inject lock state does not allow insertion plain symmetric keys");
    }

    // Inserting a wrapped key must succeed (just inserting the same key)
    for (indexAesKey=A71CH_SYM_KEY_MAX>>1; indexAesKey<A71CH_SYM_KEY_MAX; indexAesKey++)
    {
        PRINTF("\r\nA71_SetRfc3394WrappedAesKey(0x%02X)\r\n", indexAesKey);
        err = A71_SetRfc3394WrappedAesKey(indexAesKey, aesRefWrapped[indexAesKey], 24);
        result &= AX_CHECK_SW(err, SW_OK, "err");
        assert(result);
    }

    // Slots are filled up with known keys, do a KDF as proof
    for (indexAesKey=0; indexAesKey<A71CH_SYM_KEY_MAX; indexAesKey++)
    {
        PRINTF( "\r\nA71_HkdfExpandSymKey(0x%02x)\r\n", indexAesKey);
        derivedDataLen = sizeof(derivedData);
        err = A71_HkdfExpandSymKey((SST_Index_t)indexAesKey, nBlock, info, infoLen, derivedData, derivedDataLen);
        result &= AX_CHECK_SW(err, SW_OK, "err");
        // axPrintByteArray("derivedData", derivedData, derivedDataLen, AX_COLON_32);
        result &= AX_COMPARE_BYTE_ARRAY("derivedDataRef[indexAesKey]", derivedDataRef[indexAesKey], sizeof(derivedDataRef[indexAesKey]),
            "derivedData", derivedData, derivedDataLen, AX_COLON_32);
    }

    PRINTF( "\r\n-----------\r\nEnd exSstSym(%s), result = %s\r\n------------\r\n", getInitModeAsString(initMode), ((result == 1)? "OK": "FAILED"));

    return result;
}

/**
 * Demonstrate
 * - setting/getting/erasing/freezing of public keys
 * - demonstrate key can be used through verify operation
 * - illustrate setting a public key after INJECT_PLAIN lock has been set
 *
 * @param[in] initMode Visit the documentation of ::a71chInitModule for more information on this parameter. A debug reset must be issued for the test
 *   to be successfull.
 * @param[in] appletVersion In case an applet older than Revision 1.2 (0x012x) is attached, a negative test is skipped.
 */
static U8 exSstPub(U8 initMode)
{
    U8 result = 1;
    U16 err;
    int i;
    HLSE_RET_CODE retcode;
    U8 isOk = 0x00;
    HLSE_MECHANISM_INFO mechInfo;

    ECCCurve_t eccCurve = ECCCurve_NIST_P256;

    EC_KEY *eccKeyCA[A71CH_PUBLIC_KEY_MAX] = {0};
    eccKeyComponents_t eccKcCA[A71CH_PUBLIC_KEY_MAX] = {0};

    EC_KEY *eccKeyAlternative = NULL;
    eccKeyComponents_t eccKcAlternative = {0};

    U8 fetchedPubKey[65];
    U16 fetchedPubKeyLen = sizeof(fetchedPubKey);

    U8 hashSha256[32];
    U16 hashSha256Len = sizeof(hashSha256);

    U8 signatureOnHost[128];
    U32 signatureOnHostLen = sizeof(signatureOnHost);

    // const U16 expectedPubKeyLen = 65;
    const U16 expectedPrivKeyLen = 32;

    SST_Index_t pubKeyIndex;

    // When inject lock is set, public keys must be wrapped before being written.
    SST_Index_t  indexCfgKey;
    U8 configKeyPublicKey[16] = {
        0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11,
        0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99 };
    U8 wrappedKey[64+8];
    U16 wrappedKeyLen = sizeof(wrappedKey);

    PRINTF("\r\n-----------\r\nStart exSstPub(%s)\r\n------------\r\n", getInitModeAsString(initMode));

    // Initialize the A71CH (Debug mode restrictions may apply)
    result &= a71chInitModule(initMode);

    // Start by creating, inserting and checking keys
    for (i=0; i<A71CH_PUBLIC_KEY_MAX; i++)
    {
        eccKeyCA[i] = NULL;
        err = HOSTCRYPTO_GenerateEccKey(eccCurve, &eccKeyCA[i]);
        result &= AX_CHECK_SW(err, SW_OK, "err");

        err = HOSTCRYPTO_GetPublicKey(eccKeyCA[i], eccKcCA[i].pub, &(eccKcCA[i].pubLen), (64 << 1)+1);
        result &= AX_CHECK_SW(err, SW_OK, "err");

        err = HOSTCRYPTO_GetPrivateKey(eccKeyCA[i], eccKcCA[i].priv, &(eccKcCA[i].privLen), 64);
        result &= AX_CHECK_SW(err, SW_OK, "err");
        eccKcCA[i].bits = expectedPrivKeyLen << 3;
        eccKcCA[i].curve = eccCurve;

        PRINTF( "\r\nA71_SetEccPublicKey(0x%02X)\r\n", (SST_Index_t)i);
        err = A71_SetEccPublicKey ((SST_Index_t) i, eccKcCA[i].pub, eccKcCA[i].pubLen);
        result &= AX_CHECK_SW(err, SW_OK, "err");
    }

    // Read out and verify public key
    for (i=0; i<A71CH_PUBLIC_KEY_MAX; i++)
    {
        PRINTF( "\r\nA71_GetEccPublicKey(0x%02X)\r\n", (SST_Index_t)i);
        fetchedPubKeyLen = sizeof(fetchedPubKey);
        err = A71_GetEccPublicKey ((SST_Index_t) i, fetchedPubKey, &fetchedPubKeyLen);
        result &= AX_CHECK_SW(err, SW_OK, "err");

        // Compare with reference value
        result &= AX_COMPARE_BYTE_ARRAY("eccKcCA[i].pub", eccKcCA[i].pub, eccKcCA[i].pubLen,
            "fetchedPubKey", fetchedPubKey, fetchedPubKeyLen, AX_COLON_32);
    }

    // Create alternative key
    err = HOSTCRYPTO_GenerateEccKey(eccCurve, &eccKeyAlternative);
    result &= AX_CHECK_SW(err, SW_OK, "err");

    err = HOSTCRYPTO_GetPublicKey(eccKeyAlternative, eccKcAlternative.pub, &(eccKcAlternative.pubLen), (64 << 1)+1);
    result &= AX_CHECK_SW(err, SW_OK, "err");

    err = HOSTCRYPTO_GetPrivateKey(eccKeyAlternative, eccKcAlternative.priv, &(eccKcAlternative.privLen), 64);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    eccKcAlternative.bits = expectedPrivKeyLen << 3;
    eccKcAlternative.curve = eccCurve;

    // Fill up the hash with some reference data
    hashSha256Len = sizeof(hashSha256);
    for (i=0; i<hashSha256Len; i++) { hashSha256[i] = (U8)i; }

    // Sign a hash of correct length on the host and do the verification on the A71CH
    for (i=0; i<A71CH_PUBLIC_KEY_MAX; i++)
    {
        PRINTF("\r\n(Host)ECDSA_sign API with eccKeyCA[i].\r\n");
        signatureOnHostLen = sizeof(signatureOnHost);
        memset(&mechInfo, 0, sizeof(mechInfo));
        mechInfo.mechanism = HLSE_ECDSA_SIGN;
        retcode = HLCRYPT_Sign(&mechInfo, (U8 *)eccKeyCA[i], 0,hashSha256, hashSha256Len, signatureOnHost, &signatureOnHostLen);

        if (retcode != HLSE_SW_OK)
        {
            PRINTF("(Host)ECDSA_sign operation failed.\r\n");
            result &= 0;
        }
        else
        {
            axPrintByteArray("signatureOnHost", signatureOnHost, (U16)signatureOnHostLen, AX_COLON_32);
        }
        // Verify on A71CH
        isOk = 0x00;
        PRINTF("\r\nA71_EccVerify(0x%02X)\r\n", (SST_Index_t)i);
        err = A71_EccVerify((SST_Index_t) i, hashSha256, hashSha256Len, signatureOnHost, (U16)signatureOnHostLen, &isOk);
        result &= AX_CHECK_SW(err, SW_OK, "err");
        result &= AX_CHECK_U8(isOk, 1, "Signature verification failed");

        // It's also possible to do a verification on the A71CH by passing the public key by value
        // In case the key has been provisioned inside the A71CH, referring to the key by index is preferred
        // as it does not imply an EEPROM write operation
        isOk = 0x00;
        PRINTF("\r\nA71_EccVerifyWithKey(PubkeyByValue)\r\n");
        err = A71_EccVerifyWithKey(eccKcCA[i].pub, eccKcCA[i].pubLen, hashSha256, hashSha256Len,
            signatureOnHost, (U16)signatureOnHostLen, &isOk);
        result &= AX_CHECK_SW(err, SW_OK, "err");
        result &= AX_CHECK_U8(isOk, 1, "Signature verification failed");
    }

    // Erase the public key at index 0 & verify the value is no longer readable
    // ** Erase **
    pubKeyIndex = A71CH_PUBLIC_KEY_0;
    PRINTF("\r\nA71_EraseEccPublicKey(index=0x%02X)\r\n", pubKeyIndex);
    err = A71_EraseEccPublicKey(pubKeyIndex);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    // ** Check whether erase was effective **
    PRINTF("\r\nA71_GetEccPublicKey(index=0x%02X)\r\n", pubKeyIndex);
    fetchedPubKeyLen = sizeof(fetchedPubKey);
    err = A71_GetEccPublicKey((SST_Index_t)pubKeyIndex, fetchedPubKey, &fetchedPubKeyLen);
    result &= AX_CHECK_SW(err, SW_CONDITIONS_NOT_SATISFIED, "Get Public Ecc Key was supposed to fail");

    // Fill in the original value again
    pubKeyIndex = A71CH_PUBLIC_KEY_0;
    PRINTF( "\r\nA71_SetEccPublicKey(0x%02x)\r\n", pubKeyIndex);
    err = A71_SetEccPublicKey((SST_Index_t)pubKeyIndex, eccKcCA[pubKeyIndex].pub, eccKcCA[pubKeyIndex].pubLen);
    result &= AX_CHECK_SW(err, SW_OK, "err");

    // Now Lock the first half of the slots for update
    for (pubKeyIndex=0; pubKeyIndex<A71CH_PUBLIC_KEY_MAX>>1; pubKeyIndex++)
    {
        PRINTF( "\r\nA71_FreezeEccPublicKey(0x%02x)\r\n", pubKeyIndex);
        err = A71_FreezeEccPublicKey((SST_Index_t)pubKeyIndex);
        result &= AX_CHECK_SW(err, SW_OK, "err");
    }

    // Now fetch and compare the values with the reference values
    for (pubKeyIndex=0; pubKeyIndex<A71CH_PUBLIC_KEY_MAX; pubKeyIndex++)
    {
        PRINTF( "\r\nA71_GetEccPublicKey(0x%02x)\r\n", pubKeyIndex);
        fetchedPubKeyLen = sizeof(fetchedPubKey);
        err = A71_GetEccPublicKey((SST_Index_t)pubKeyIndex, fetchedPubKey, &fetchedPubKeyLen);
        result &= AX_CHECK_SW(err, SW_OK, "err");
        result &= AX_COMPARE_BYTE_ARRAY("eccKcCA[pubKeyIndex]", eccKcCA[pubKeyIndex].pub, eccKcCA[pubKeyIndex].pubLen,
            "fetchedKey", fetchedPubKey, fetchedPubKeyLen, AX_COLON_32);
    }

    // Check whether the locked half (i.e. first half) is truly 'frozen' ....
    for (pubKeyIndex=0; pubKeyIndex<A71CH_PUBLIC_KEY_MAX>>1; pubKeyIndex++)
    {
        PRINTF( "\r\nA71_SetEccPublicKey(0x%02x)\r\n", pubKeyIndex);
        err = A71_SetEccPublicKey((SST_Index_t)pubKeyIndex, eccKcAlternative.pub, eccKcAlternative.pubLen);
        result &= AX_CHECK_SW(err, SW_COMMAND_NOT_ALLOWED, "Expected to fail, frozen credential cannot be overwritten");
    }

    // Overwrite the second half
    for (pubKeyIndex=A71CH_PUBLIC_KEY_MAX>>1; pubKeyIndex<A71CH_PUBLIC_KEY_MAX; pubKeyIndex++)
    {
        PRINTF( "\r\nA71_SetEccPublicKey(0x%02x)\r\n", pubKeyIndex);
        err = A71_SetEccPublicKey((SST_Index_t)pubKeyIndex, eccKcAlternative.pub, eccKcAlternative.pubLen);
        result &= AX_CHECK_SW(err, SW_OK, "err");
    }

    // Check that the second half was overwritten, by signing a hash of correct length on the host and do the verification on the A71CH
    for (pubKeyIndex=A71CH_PUBLIC_KEY_MAX>>1; pubKeyIndex<A71CH_PUBLIC_KEY_MAX; pubKeyIndex++)
    {
        PRINTF("\r\n(Host)ECDSA_sign API with eccKeyCA[i].\r\n");
        signatureOnHostLen = sizeof(signatureOnHost);
        memset(&mechInfo, 0, sizeof(mechInfo));
        mechInfo.mechanism = HLSE_ECDSA_SIGN;
        retcode = HLCRYPT_Sign(&mechInfo, (U8 *)eccKeyAlternative, 0,hashSha256, hashSha256Len, signatureOnHost, &signatureOnHostLen);

        if (retcode != HLSE_SW_OK)
        {
            PRINTF("(Host)ECDSA_sign operation failed.\r\n");
            result &= 0;
        }
        else
        {
            axPrintByteArray("signatureOnHost", signatureOnHost, (U16)signatureOnHostLen, AX_COLON_32);
        }
        // Verify on A71CH
        isOk = 0x00;
        PRINTF( "\r\nA71_EccVerify(0x%02X)\r\n", (SST_Index_t)pubKeyIndex);
        err = A71_EccVerify((SST_Index_t) pubKeyIndex, hashSha256, hashSha256Len, signatureOnHost, (U16)signatureOnHostLen, &isOk);
        result &= AX_CHECK_SW(err, SW_OK, "err");
        result &= AX_CHECK_U8(isOk, 1, "Signature verification failed");
    }

    // Put back the original (reference) values in the second half
    for (pubKeyIndex=A71CH_PUBLIC_KEY_MAX>>1; pubKeyIndex<A71CH_PUBLIC_KEY_MAX; pubKeyIndex++)
    {
        PRINTF( "\r\nA71_SetEccPublicKey(0x%02x)\r\n", pubKeyIndex);
        err = A71_SetEccPublicKey ((SST_Index_t) pubKeyIndex, eccKcCA[pubKeyIndex].pub, eccKcCA[pubKeyIndex].pubLen);
        result &= AX_CHECK_SW(err, SW_OK, "err");
    }

    // Check all keys are still fine by doing a verify operation
    for (i=0; i<A71CH_PUBLIC_KEY_MAX; i++)
    {
        PRINTF("\r\n(Host)ECDSA_sign API with eccKeyCA[i].\r\n");
        signatureOnHostLen = sizeof(signatureOnHost);
        memset(&mechInfo, 0, sizeof(mechInfo));
        mechInfo.mechanism = HLSE_ECDSA_SIGN;
        retcode = HLCRYPT_Sign(&mechInfo, (U8 *)eccKeyCA[i], 0,hashSha256, hashSha256Len, signatureOnHost, &signatureOnHostLen);
        if (retcode != HLSE_SW_OK)
        {
            PRINTF("(Host)ECDSA_sign operation failed.\r\n");
            result &= 0;
        }
        else
        {
            axPrintByteArray("signatureOnHost", signatureOnHost, (U16)signatureOnHostLen, AX_COLON_32);
        }
        // Verify on A71CH
        isOk = 0x00;
        PRINTF( "\r\nA71_EccVerify(0x%02X)\r\n", (SST_Index_t)i);
        err = A71_EccVerify((SST_Index_t) i, hashSha256, hashSha256Len, signatureOnHost, (U16)signatureOnHostLen, &isOk);
        result &= AX_CHECK_SW(err, SW_OK, "err");
        result &= AX_CHECK_U8(isOk, 1, "Signature verification failed");
    }

    // Now disable the plain insertion/reading out of Symmetric keys & Keypairs
    // THIS ALSO IMPACTS functionality of Public Key Storage, second half of storage is only writeable when public key
    // has been wrapped.
    // ** First set wrapping key (in secure element)
    indexCfgKey = A71CH_CFG_KEY_IDX_PUBLIC_KEYS;
    PRINTF("\r\nA71_SetConfigKey(0x%02x)\r\n", indexCfgKey);
    err = A71_SetConfigKey((SST_Index_t)indexCfgKey, configKeyPublicKey, sizeof(configKeyPublicKey));
    result &= AX_CHECK_SW(err, SW_OK, "err");
    PRINTF("\r\nA71_InjectLock()\r\n");
    err = A71_InjectLock();
    result &= AX_CHECK_SW(err, SW_OK, "err");
    assert(result);

    {
        // Setting unwrapped public keys (in second half) MUST fail
        for (pubKeyIndex = A71CH_PUBLIC_KEY_MAX >> 1; pubKeyIndex < A71CH_PUBLIC_KEY_MAX; pubKeyIndex++)
        {
            PRINTF("\r\nA71_SetEccPublicKey(0x%02X) - unwrapped (negative test)\r\n", (SST_Index_t)pubKeyIndex);
            err = A71_SetEccPublicKey((SST_Index_t)pubKeyIndex, eccKcAlternative.pub, eccKcAlternative.pubLen);
            result &= AX_CHECK_SW(err, SW_COMMAND_NOT_ALLOWED, "err");
        }
    }

    // Setting wrapped public keys (in second half) MUST succeed
    // ** Wrapping key must have been set before Inject Lock was requested
    // ** Wrap the public key (strip first character before wrapping) on host
    PRINTF("\r\nHOSTCRYPTO_AesWrapKeyRFC3394(eccKcAlternative)\r\n");
    err = HOSTCRYPTO_AesWrapKeyRFC3394(configKeyPublicKey, sizeof(configKeyPublicKey),
        wrappedKey, &wrappedKeyLen, &(eccKcAlternative.pub[1]), eccKcAlternative.pubLen - 1);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    axPrintByteArray("wrappedKey", wrappedKey, wrappedKeyLen, AX_COLON_32);

    // ** Setting the wrapped public key (in second half)
    for (pubKeyIndex=A71CH_PUBLIC_KEY_MAX>>1; pubKeyIndex<A71CH_PUBLIC_KEY_MAX; pubKeyIndex++)
    {
        PRINTF( "\r\nA71_SetEccPublicKey(0x%02X) - wrapped\r\n", (SST_Index_t)pubKeyIndex);
        err = A71_SetEccPublicKey ((SST_Index_t)pubKeyIndex, wrappedKey, wrappedKeyLen);
        result &= AX_CHECK_SW(err, SW_OK, "err");
    }

    // Retrieving and comparing second half public keys
    for (pubKeyIndex=A71CH_PUBLIC_KEY_MAX>>1; pubKeyIndex<A71CH_PUBLIC_KEY_MAX; pubKeyIndex++)
    {
        PRINTF( "\r\nA71_GetEccPublicKey(0x%02x)\r\n", pubKeyIndex);
        fetchedPubKeyLen = sizeof(fetchedPubKey);
        err = A71_GetEccPublicKey((SST_Index_t)pubKeyIndex, fetchedPubKey, &fetchedPubKeyLen);
        result &= AX_CHECK_SW(err, SW_OK, "err");
        result &= AX_COMPARE_BYTE_ARRAY("eccKcAlternative.pub", eccKcAlternative.pub, eccKcAlternative.pubLen,
            "fetchedKey", fetchedPubKey, fetchedPubKeyLen, AX_COLON_32);
    }

    HOSTCRYPTO_FreeEccKey(&eccKeyAlternative);
    for (i=0; i<A71CH_PUBLIC_KEY_MAX; i++)
    {
        HOSTCRYPTO_FreeEccKey(&eccKeyCA[i]);
    }

    PRINTF( "\r\n-----------\r\nEnd exSstPub(%s), result = %s\r\n------------\r\n", getInitModeAsString(initMode),
        ((result == 1)? "OK": "FAILED"));

    return result;
}
