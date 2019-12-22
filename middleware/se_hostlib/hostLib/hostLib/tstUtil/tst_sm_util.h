/**
 * @file tst_sm_util.h
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
 * This file provides the interface to utility functions used by the example programs, not
 * the actual Host Library.
 * @par HISTORY
 * 1.0   06-aug-2013 : Initial version
 *
 */

#ifndef _TST_SM_UTIL_H_
#define _TST_SM_UTIL_H_

#include "sm_types.h"
#include "sm_printf.h"

#if !defined(TGT_A71CH) && !defined(TGT_A71CL)
#include "ax_api.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define AX_UTIL_OK    0  //!< Integer return status value to indicate succesfull execution
#define AX_UTIL_ERROR 1  //!< Integer return status value to indicate execution failure

/// @cond
#define SM_KEEP_STATE 0xFE
/// @endcond

// ByteArray print style
#define AX_COMPACT_16   0x0010  //!< Resulting style: 001122AA..  16 eq. HEX Values
#define AX_COMPACT_32   0x0020  //!< Resulting style: 001122AA..  32 eq. HEX Values
#define AX_COMPACT_LINE 0x00F0  //!< Resulting style: 001122AA.... HEX Values. No line breaks.
#define AX_HEX_16       0x0110  //!< Resulting style: 0x00 0x11 0x22 0xAA..  16 eq. HEX Values
#define AX_HEX_32       0x0120  //!< Resulting style: 0x00 0x11 0x22 0xAA..  32 eq. HEX Values
#define AX_COLON_16     0x0210  //!< Resulting style: 00:11:22:AA..  16 eq. HEX Values
#define AX_COLON_32     0x0220  //!< Resulting style: 00:11:22:AA..  32 eq. HEX Values
#define AX_CARRAY_16    0x0310  //!< Resulting style: 0x00, 0x11, 0x22, 0xAA, ..  16 eq. HEX Values
#define AX_CARRAY_32    0x0320  //!< Resulting style: 0x00, 0x11, 0x22, 0xAA, ..  32 eq. HEX Values

#if !defined(TGT_A71CH) && !defined(TGT_A71CL)
/// @cond
typedef struct {
    SST_Item_t item;
    U16 sw;
} ItemSw_t;
/// @endcond
#endif

#define AX_CHECK_SW(A,B,C) axCheckSw(A,B,C,__FILE__,__LINE__) //!< Macro allowing to invoke ::axCheckSw without explicitly specifying source file name and line number as parameters
#define AX_CHECK_U8(A,B,C) axCheckU8(A,B,C,__FILE__,__LINE__) //!< Macro allowing to invoke ::axCheckU8 without explicitly specifying source file name and line number as parameters
#define AX_CHECK_U16(A,B,C) axCheckU16(A,B,C,__FILE__,__LINE__) //!< Macro allowing to invoke ::axCheckU16 without explicitly specifying source file name and line number as parameters
#if defined(TGT_A71CH) || defined (TGT_A71CL)
#define AX_COMPARE_BYTE_ARRAY(A,B,C,D,E,F,G) \
    axCompareByteArray(A,B,C,D,E,F,G,__FILE__,__LINE__) //!< Macro allowing to invoke ::axCompareByteArray without explicitly specifying source file name and line number as parameters
#endif

int axPrintByteArray(const char *pName, const U8 *pData, U16 dataLength, U16 style);
#if defined(TGT_A71CH) || defined (TGT_A71CL)
U8 axCompareByteArray(const char *aName, const U8 *pA, U16 aLen, const char *bName, const U8 *pB, U16 bLen, U16 style, char *szFilename, int lineNr);
#else
U8 axCompareByteArray(const char *aName, const U8 *pA, U16 aLen, const char *bName, const U8 *pB, U16 bLen, U16 style);
#endif
int axConvertHexString2ByteArray(U8 *byteArray, const char *string, int nOffset, int nByte);
int axConvertByteArray2HexString(char *string, int stringBufSize, const U8 *byteArray, int nByte, U16 style);
U8 axCheckSw(U16 sw, U16 expectedSw, char *msg, char *szFilename, int lineNr);
U8 axCheckU8(U8 in, U8 expected, char *msg, char *szFilename, int lineNr);
U8 axCheckU16(U16 in, U16 expected, char *msg, char *szFilename, int lineNr);

U16 axZeroSignExtend(U8* pStore, U16 actualLength, U16 expectedLength);

#if !defined(TGT_A71CH) && !defined(TGT_A71CL)
int convertString2ByteArray(U8 *byteArray, const char *string, int nOffset, int nByte);
U8 checkBytestring(U8 *pA, U16 aLength, U8 * pB, U16 bLength, char *msg);
void printBytestring (const char *pName, const U8 *pData, U16 dataLength);
int compareBytestrings(U8 *pA, U16 aLength, U8 * pB, U16 bLength);
U8 checkErr(U16 err, U16 expectedErr, char *msg);
U8 checkU8(U8 in, U8 expected, char *msg);
U8 checkU16(U16 in, U16 expected, char *msg);

#ifndef TGT_A70CM
U8 setUser(U8 targetUser);
#endif
#if defined(TGT_A70CI) || defined(TGT_A70CM)
char* getLifecycleStateName(U16 state);
#else
char* getLifecycleStateName(U8 state);
#endif
char* GetAuthenticationStateName(U16 state);
char *getItemName(SST_Item_t item);
char *getUserName(U8 user);
#endif // TGT_A71CH

#ifdef __cplusplus
}
#endif
#endif //
