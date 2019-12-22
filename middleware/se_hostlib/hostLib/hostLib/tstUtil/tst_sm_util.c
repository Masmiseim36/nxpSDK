/**
 * @file tst_sm_util.c
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
 * This file implements utility functions for the example programs, not for
 * the Host Library.
 * @par HISTORY
 * 1.0   06-aug-2013 : Initial version
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "tst_sm_util.h"
#include "nxLog_App.h"
#ifdef TGT_EDEV
#include "sm_debug.h"
#endif

/**
* Utility function to pretty print a byte array
* @param[in] pName  Name associated with the byte array to be printed out
* @param[in] pData Byte array to be printed out
* @param[in] dataLength length in byte of byte array A
* @param[in] style Style of byte array decoration (e.g. ::AX_COLON_32)
* @return Always returns ::AX_UTIL_OK
*/
int axPrintByteArray(const char *pName, const U8 *pData, U16 dataLength, U16 style)
{
#if (defined(DEBUG) || defined(FLOW_VERBOSE) || defined(AX_CONSOLE_LOG)) && !(AX_EMBEDDED)
    U16 i = 0;
    int nBreak = 0;
    char preDeco[32];
    char postDeco[32];

    assert(pName != NULL);
    assert(pData != NULL);

    switch (style)
    {
    case AX_COMPACT_16:
        nBreak = 16;
        strcpy(preDeco, "");
        strcpy(postDeco, "");
        break;
    case AX_COMPACT_32:
        nBreak = 32;
        strcpy(preDeco, "");
        strcpy(postDeco, "");
        break;
    case AX_COMPACT_LINE:
        nBreak = -1;
        strcpy(preDeco, "");
        strcpy(postDeco, "");
        break;
    case AX_HEX_16:
        nBreak = 16;
        strcpy(preDeco, "0x");
        strcpy(postDeco, " ");
        break;
    case AX_HEX_32:
        nBreak = 32;
        strcpy(preDeco, "0x");
        strcpy(postDeco, " ");
        break;
    case AX_COLON_16:
        nBreak = 16;
        strcpy(preDeco, "");
        strcpy(postDeco, ":");
        break;
    case AX_COLON_32:
        nBreak = 32;
        strcpy(preDeco, "");
        strcpy(postDeco, ":");
        break;
    case AX_CARRAY_16:
        nBreak = 16;
        strcpy(preDeco, "0x");
        strcpy(postDeco, ", ");
        break;
    case AX_CARRAY_32:
        nBreak = 32;
        strcpy(preDeco, "0x");
        strcpy(postDeco, ", ");
        break;
    default:
        nBreak = 32;
        strcpy(preDeco, "");
        strcpy(postDeco, "");
        break;
    }

    printf("%s (LEN=%d):%c", pName, dataLength, (nBreak > 0) ? '\n' : ' ');

    for (i = 0; i < dataLength ; i++)
    {
        if (i == (dataLength-1))
        {
            // The last byte to print doesn't get postDeco appended
            printf("%s%02X", preDeco, pData[i]);
        }
        else
        {
            printf("%s%02X%s", preDeco, pData[i], postDeco);
        }
        if (nBreak > 0 )
        {
            if ((i+1) % nBreak == 0) {printf("\r\n");}
        }
    }
    printf("\r\n");
#endif
    return AX_UTIL_OK;
}

/**
* Convert a Byte array into an ASCII string representation of an Hexadecimal value.
*
* Example invocation: axConvertHexString2ByteArray(byteArray, "02AA0B", 0, 3)
*
* @param[in,out]  string        Target (byte array as ASCII string)
* @param[in]      stringBufSize size of string buffer (including storage required for closing '\0')
* @param[in]      byteArray     Source byte array
* @param[in]      nByte         Length to be converted (length as amount of byte; Note: 1 byte equals 2 ASCII characters)
* @param[in]      style         Style of byte array decoration (only one style supported: ::AX_COMPACT_LINE)
*
* @retval         ::AX_UTIL_OK upon successfull conversion
* @retval         ::AX_UTIL_ERROR upon failure
*/
int axConvertByteArray2HexString(char *string, int stringBufSize, const U8 *byteArray, int nByte, U16 style)
{
    int i = 0;

    // We only support one style at the moment
    if (style != AX_COMPACT_LINE)
    {
        return AX_UTIL_ERROR;
    }

    // Check whether provided stringbuffer can contain converted
    if (stringBufSize < ((nByte << 1) + 1))
    {
        return AX_UTIL_ERROR;
    }

    for (i = 0; i < nByte; i++)
    {
        sprintf(&string[i<<1], "%02X", byteArray[i]);
    }

    return AX_UTIL_OK;
}


/**
 * Convert an ASCII string representation of an Hexadecimal value into a bytearray.
 *
 * Example invocation: axConvertHexString2ByteArray(byteArray, "02AA0B", 0, 3)
 *
 * @param[in,out]  byteArray Target byte array, caller needs to ensure buffer is of sufficient size (> \p nByte)
 * @param[in]      string    Source (byte array as ASCII string)
 * @param[in]      nOffset   offset in string (ASCII string offset)
 * @param[in]      nByte     Length to be converted (length as amount of byte; note: 2 ASCII characters equals 1 byte)
 *
 * @retval         ::AX_UTIL_OK upon successfull conversion
 * @retval         ::AX_UTIL_ERROR upon failure
 */
int axConvertHexString2ByteArray(U8 *byteArray, const char *string, int nOffset, int nByte)
{
    char szDummy[] = "szDummy";
    char *pastConverted = szDummy; // Catch number conversion issues
    int j;

    for (j=0; j<nByte; j++) {
        char byteAsString[3];
        byteAsString[0] = string[nOffset+2*j];
        byteAsString[1] = string[nOffset+2*j + 1];
        byteAsString[2] = '\0';
        byteArray[j] = (U8)(strtoul(byteAsString, &pastConverted, 16));

        if (pastConverted == byteAsString) {
            // Conversion failed
            printf("(%s/%d) %s can not be converted to HEX value.\r\n", __FILE__, __LINE__, byteAsString);
            return AX_UTIL_ERROR;
        }

    }
    return AX_UTIL_OK;
}

#if defined(TGT_A71CH) || defined (TGT_A71CL)
/**
* Utility function to compare two byte arrays. Print out arrays in case they are different
*
* Use the macro ::AX_COMPARE_BYTE_ARRAY to invoke this function
*
* @param[in] aName  String associated with byte array A
* @param[in] pA Byte array A
* @param[in] aLen length in byte of byte array A
* @param[in] bName  String associated with byte array B
* @param[in] pB Byte array B
* @param[in] bLen length in byte of byte array B
* @param[in] style Style of byte array decoration (e.g. ::AX_COLON_32)
* @param[in] szFilename  Filename of source file from which this function was invoked
* @param[in] lineNr      Linenumber in source file from which the function was invoked
* @retval 0 Upon failed execution or wrong comparison
* @retval 1 Upon successfull comparison
*
*/
U8 axCompareByteArray(const char *aName, const U8 *pA, U16 aLen, const char *bName, const U8 *pB, U16 bLen, U16 style, char *szFilename, int lineNr)
#else
/**
* Utility function to compare two byte arrays. Print out arrays in case they are different
* @param[in] aName  String associated with byte array A
* @param[in] pA Byte array A
* @param[in] aLen length in byte of byte array A
* @param[in] bName  String associated with byte array B
* @param[in] pB Byte array B
* @param[in] bLen length in byte of byte array B
* @param[in] style Style of byte array decoration (e.g. ::AX_COLON_32)
* @retval 0 Upon failed execution or wrong comparison
* @retval 1 Upon successfull comparison
*
*/
U8 axCompareByteArray(const char *aName, const U8 *pA, U16 aLen, const char *bName, const U8 *pB, U16 bLen, U16 style)
#endif
{
    U8 uRet = 0;

    if (aName == NULL) {return 0;}
    if (pA == NULL) {return 0;}
    if (bName == NULL) {return 0;}
    if (pB == NULL) {return 0;}
    if (aLen == bLen)
    {
        if (memcmp(pA, pB, aLen) == 0)
        {
            uRet = 1;
        }
    }

    if (uRet != 1)
    {
        printf("\r\n***** ERROR (%s != %s)\r\n", aName, bName);
#if defined(TGT_A71CH) || defined (TGT_A71CL)
        printf("%s: line=%d\r\n", szFilename, lineNr);
#endif
        axPrintByteArray(aName, pA, aLen, style);
        axPrintByteArray(bName, pB, bLen, style);
        printf("*****\r\n");
    }

    return uRet;
}

/**
* Utility function to compare a status word (of type U16) with a reference value.
*
* Use the macro ::AX_CHECK_SW to invoke this function
*
* @param[in] sw          Status word to evaluate
* @param[in] expectedSw  Reference status word
* @param[in] msg         Message to print in case value \p err does not equal \p expectedErr
* @param[in] szFilename  Filename of source file from which this function was invoked
* @param[in] lineNr      Linenumber in source file from which the function was invoked
* @retval 0 Upon failed execution or wrong comparison
* @retval 1 Upon successfull comparison
*
*/
U8 axCheckSw(U16 sw, U16 expectedSw, char *msg, char *szFilename, int lineNr)
{
    if (sw != expectedSw)
    {
    	LOG_E("\r\n***** ERROR (%s)\r\n", msg);
    	LOG_E("%s: line=%d\r\n", szFilename, lineNr);
    	LOG_E("***** Expected SW 0x%04x, but got 0x%04X\r\n", expectedSw, sw);
        return 0;
    }
    else
    {
        return 1;
    }
}

/**
* Utility function to compare a value of type U8 with a reference value.
*
* Use the macro ::AX_CHECK_U8 to invoke this function
*
* @param[in] in          U8 variable to evaluate
* @param[in] expected    Reference value
* @param[in] msg         Message to print in case value \p in does not equal \p expected
* @param[in] szFilename  Filename of source file from which this function was invoked
* @param[in] lineNr      Linenumber in source file from which the function was invoked
* @retval 0 Upon failed execution or wrong comparison
* @retval 1 Upon successfull comparison
*
*/
U8 axCheckU8(U8 in, U8 expected, char *msg, char *szFilename, int lineNr)
{
    if (in != expected)
    {
        LOG_E("\r\n***** ERROR (%s)\r\n", msg);
        LOG_E("%s: line=%d\r\n", szFilename, lineNr);
        LOG_E("***** Expected 0x%02x, but got 0x%02x\r\n", expected, in);
        return 0;
    }
    else
    {
        return 1;
    }
}

/**
* Utility function to compare a value of type U16 with a reference value.
*
* Use the macro ::AX_CHECK_U16 to invoke this function
*
* @param[in] in          U16 variable to evaluate
* @param[in] expected    Reference value
* @param[in] msg         Message to print in case value \p in does not equal \p expected
* @param[in] szFilename  Filename of source file from which this function was invoked
* @param[in] lineNr      Linenumber in source file from which the function was invoked
* @retval 0 Upon failed execution or wrong comparison
* @retval 1 Upon successfull comparison
*
*/
U8 axCheckU16(U16 in, U16 expected, char *msg, char *szFilename, int lineNr)
{
    if (in != expected)
    {
        LOG_E("\r\n***** ERROR (%s)\r\n", msg);
        LOG_E("%s: line=%d\r\n", szFilename, lineNr);
        LOG_E("***** Expected 0x%02x, but got 0x%02x\r\n", expected, in);
        return 0;
    }
    else
    {
        return 1;
    }
}

/**
 * Extend the byte array \p pStore with 0x00 byte(s). This is typically required when
 * a big integer has - previously - been stripped from its (superfluous) sign bits.
 * The caller must ensure \p expectedLength is bigger than \p actualLength
 * @param[in,out]   pStore  Array representation of big number, to be zero sign extended.
 *      Size of corresponding buffer must be at least \p expectedLength
 * @param[in]       actualLength Length of incoming array \p pStore
 * @param[in]       expectedLength Zero sign extend until this length.
 *
 * @retval SW_OK In case of successfull execution
 * @retval ERR_API_ERROR Requested adjustment would result in truncation
 */
U16 axZeroSignExtend(U8* pStore, U16 actualLength, U16 expectedLength)
{
    U16 sw = SW_OK;

    int numExtraByte = (int)expectedLength - (int)actualLength;

    if (numExtraByte == 0) {
        // Do nothing
    }
    else if (numExtraByte < 0) {
        // Flag an API error
        sw = ERR_API_ERROR;
    }
    else {
        memmove(pStore + numExtraByte, pStore, actualLength);
        memset(pStore, 0x00, numExtraByte);
    }

    return sw;
}

#if !defined(TGT_A71CH) && !defined(TGT_A71CL)
// Utility functions
void printBytestring(const char *pName, const U8 *pData, U16 dataLength)
{
    U16 i = 0;
    assert(pName != NULL);
    assert(pData != NULL);

    printf("%s (LEN=%d):\r\n", pName, dataLength);

    for (i = 0; i < dataLength ; i++)
    {
        printf("%02X ", pData[i]);
    }
    printf("\r\n");
}

U8 checkBytestring(U8 *pA, U16 aLength, U8 * pB, U16 bLength, char *msg)
{
    if (compareBytestrings(pA, aLength,pB, bLength) != 0)
    {
        printf("\r\n***** ERROR (%s)\r\n", msg);
        printf("Bytestring are different:\r\n");
        axPrintByteArray ("A", pA, aLength, AX_COLON_32);
        axPrintByteArray ("B", pB, bLength, AX_COLON_32);
        // assert(0);
        return 0;
    }
    else
    {
        return 1;
    }
}

// U8 *byteArray       : Target byte array, caller needs to ensure buffer is of sufficient size (>nByte)
// const char *string  : Source (byte array as ASCII string)
// int nOffset         : offset in string (ASCII string offset)
// int nByte           : length to be converted (length as amount of byte; note: 2 ASCII characters equals 1 byte)
//
// Obsolete function, please use axConvertHexString2ByteArray
int convertString2ByteArray(U8 *byteArray, const char *string, int nOffset, int nByte)
{
    int j;
    for (j=0; j<nByte; j++) {
        char byteAsString[3];
        byteAsString[0] = string[nOffset+2*j];
        byteAsString[1] = string[nOffset+2*j + 1];
        byteAsString[2] = '\0';
        byteArray[j] = (U8)(strtoul(byteAsString, (char **)NULL, 16));
    }
    return 0;
}

int compareBytestrings(U8 *pA, U16 aLength, U8 *pB, U16 bLength)
{
    assert (pA != NULL);
    assert (pB != NULL);

    if (aLength < bLength) {
        return (-1);
    }
    else if (aLength > bLength) {
        return (+1);
    }
    else {
        return memcmp(pA, pB, aLength);
    }
}

/**
* Utility function to compare a status word (of type U16) with a reference value.
* @param[in] err         Status word to evaluate
* @param[in] expectedErr Reference status word
* @param[in] msg         Message to print in case value \p err does not equal \p expectedErr
* @retval 0 Upon failed execution or wrong comparison
* @retval 1 Upon successfull comparison
*
*/
U8 checkErr(U16 err, U16 expectedErr, char *msg)
{
    if (err != expectedErr)
    {
        printf("\r\n***** ERROR (%s)\r\n", msg);
        printf("***** Expected ERR 0x%04x, but got 0x%04X\r\n", expectedErr, err);
        // assert(0);
        return 0;
    }
    else
    {
        return 1;
    }
}

/**
* Utility function to compare a value of type U8 with a reference value.
* @param[in] in          U8 variable to evaluate
* @param[in] expected    Reference value
* @param[in] msg         Message to print in case value \p in does not equal \p expected
* @retval 0 Upon failed execution or wrong comparison
* @retval 1 Upon successfull comparison
*
*/
U8 checkU8(U8 in, U8 expected, char *msg)
{
    if (in != expected)
    {
        printf("\r\n***** ERROR (%s)\r\n", msg);
        printf("***** Expected 0x%02x, but got 0x%02x\r\n", expected, in);
        // assert(0);
        return 0;
    }
    else
    {
        return 1;
    }
}

/**
* Utility function to compare a value of type U16 with a reference value.
* @param[in] in          U8 variable to evaluate
* @param[in] expected    Reference value
* @param[in] msg         Message to print in case value \p in does not equal \p expected
* @retval 0 Upon failed execution or wrong comparison
* @retval 1 Upon successfull comparison
*
*/
U8 checkU16(U16 in, U16 expected, char *msg)
{
    if (in != expected)
    {
        printf("\r\n***** ERROR (%s)\r\n", msg);
        printf("***** Expected 0x%04x, but got 0x%04x\r\n", expected, in);
        // assert(0);
        return 0;
    }
    else
    {
        return 1;
    }
}

#endif // TGT_A71CH

#ifdef TGT_A70CI
char* getLifecycleStateName(U16 state)
{
    switch(state)
    {
        case SM_FACTORY:
            return "SM_FACTORY";
        case SM_CONFIGURE_INITIAL:
            return "SM_CONFIGURE_INITIAL";
        case SM_CONFIGURE:
            return "SM_CONFIGURE";
        case SM_OPERATE:
            return "SM_OPERATE";
        case SM_DEAD:
            return "SM_DEAD";
        case SM_UNKNOWN:
            return "SM_UNKNOWN";
        default:
            // this should not happen
            assert(0);
            return "";
    }
}
#elif defined(TGT_A70CM)
char* getLifecycleStateName(U16 state)
{
    switch(state) {
        case SM_FACTORY:
            return "SM_FACTORY";
        case SM_CONFIGURE_INITIAL:
            return "SM_CONFIGURE_INITIAL";
        case SM_CONFIGURE:
            return "SM_CONFIGURE";
        case SM_OPERATE:
            return "SM_OPERATE";
        case SM_RE_CONFIGURE:
            return "SM_RE_CONFIGURE";
        case SM_DEAD:
            return "SM_DEAD";
        case SM_UNKNOWN:
            return "SM_UNKNOWN";
        default:
            // this should not happen
            assert(0);
            return "";
    }
}
#elif defined(TGT_A71CH)

#elif defined(TGT_A71CL)
#else

char* getLifecycleStateName(U8 state)
{
    //switch(state)
    //{
    //    case SM_FACTORY:
    //        return "SM_FACTORY";
    //    case SM_LOCKED:
    //        return "SM_LOCKED";
    //    case SM_CONFIGURE_INITIAL:
    //        return "SM_CONFIGURE_INITIAL";
    //    case SM_CONFIGURE:
    //        return "SM_CONFIGURE";
    //    case SM_OPERATE:
    //        return "SM_OPERATE";
    //    case SM_DIAGNOSTIC:
    //        return "SM_DIAGNOSTIC";
    //    case SM_DEACTIVATED:
    //        return "SM_DEACTIVATED";
    //    case SM_DEAD:
    //        return "SM_DEAD";
    //    default:
    //        // this should not happen
    //        assert(0);
    //        return "";
    //}
}

char *getUserName(U8 user)
{
    switch (user)
    {
       /* case P2_USER_UNAUTH:
            return "unauth";
        case P2_USER_ADMIN:
            return "admin";
        case P2_USER_HOST:
            return "host";
        default:
            assert(0);
            return "";*/
    }
}

#ifdef TGT_EDEV
U8 setUser(U8 targetUser)
{
    U8 result = 1;
    U16 err = 0;

    sm_printf(CONSOLE, "DBG_SetUser(%s)\r\n", getUserName(targetUser));

    err = DBG_SetUser(targetUser);
    result &= checkErr(err, SW_OK, "err");

    return result;
}
#endif

#endif // TGT_A70CI
