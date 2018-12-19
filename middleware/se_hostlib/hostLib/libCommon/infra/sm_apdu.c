/**
* @file sm_apdu.c
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
* This file implements the high-level APDU handling of the SM module.
* @par History
* 1.0   31-march-2014 : Initial version
*
*****************************************************************************/
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "sm_apdu.h"
// #include "ax_api.h"
#include "scp.h"

// #define USE_MALLOC_FOR_APDU_BUFFER

static void ReserveLc(apdu_t * pApdu);
static void SetLc(apdu_t * pApdu, U16 lc);
static void AddLe(apdu_t * pApdu, U16 le);

#ifndef USE_MALLOC_FOR_APDU_BUFFER
static U8 sharedApduBuffer[MAX_APDU_BUF_LENGTH];
#endif

/**
* Associates a memory buffer with the APDU buffer.
*
* By default (determined at compile time) the buffer is not allocated with each call, but a reference
* is made to a static data structure.
*
* \param[in,out] pApdu         APDU buffer
* \returns always returns 0
*/
U8 AllocateAPDUBuffer(apdu_t * pApdu)
{
    assert(pApdu);
    // In case of e.g. TGT_A7, pApdu is pointing to a structure defined on the stack
    // so pApdu->pBuf contains random data

#ifdef USE_MALLOC_FOR_APDU_BUFFER
    pApdu->pBuf = (U8*) malloc(MAX_APDU_BUF_LENGTH);
#else
    pApdu->pBuf = sharedApduBuffer;
#endif

    return 0;
}

/**
* Clears the previously referenced APDU buffer.
*
* In case the buffer was effectively malloc'd by ::AllocateAPDUBuffer it will also be freed.
*
* \param[in,out] pApdu              APDU buffer
* \return Always returns 0
*/
U8 FreeAPDUBuffer(apdu_t * pApdu)
{
    assert(pApdu);
    if (pApdu->pBuf)
    {
        U16 nClear = (pApdu->rxlen > MAX_APDU_BUF_LENGTH) ? MAX_APDU_BUF_LENGTH : pApdu->rxlen;
        memset(pApdu->pBuf, 0, nClear);
#ifdef USE_MALLOC_FOR_APDU_BUFFER
        free(pApdu->pBuf);
#endif
        pApdu->pBuf = 0;
    }
    return 0;
}

/**
* Sets up the command APDU header.
* \param[in,out] pApdu      APDU buffer
* \param[in] extendedLength Indicates if command/response have extended length. Either ::USE_STANDARD_APDU_LEN or ::USE_EXTENDED_APDU_LEN
* \return                   offset in APDU buffer after the header
*/
U8 SetApduHeader(apdu_t * pApdu, U8 extendedLength)
{
//    pApdu->edc = eEdc_NoErrorDetection;
    assert(pApdu);

    pApdu->pBuf[0] = pApdu->cla;
    pApdu->pBuf[1] = pApdu->ins;
    pApdu->pBuf[2] = pApdu->p1;
    pApdu->pBuf[3] = pApdu->p2;

    pApdu->extendedLength = extendedLength;
    pApdu->hasData = false;
    pApdu->lcLength = 0;
    pApdu->lc = 0;
    pApdu->hasLe = false;

    // No LC yet
    pApdu->offset = APDU_OFFSET_LC;

    // adapt length
    pApdu->buflen = pApdu->offset;

    // Set rxlen to default value
    pApdu->rxlen = 0;

    return (U8)(pApdu->offset);
}

/**
 * In the final stage before sending the APDU cmd one needs to update the values of lc (and le).
 * \param[in,out] pApdu        APDU buffer
 * \param[in] lc
 */
void smApduAdaptLc(apdu_t *pApdu, U16 lc)
{
    SetLc(pApdu, lc);
}

/**
 * In the final stage before sending the APDU cmd one needs to update the values of le (and lc).
 * \param[in,out] pApdu        APDU buffer
 * \param[in] le
 */
void smApduAdaptLe(apdu_t *pApdu, U16 le)
{
    AddLe(pApdu, le);
}

/**
 * In the final stage before sending the APDU cmd one needs to update the values of lc and le.
 * \param[in,out] pApdu        APDU buffer
 * \param[in] lc
 * \param[in] le
 */
void smApduAdaptLcLe(apdu_t *pApdu, U16 lc, U16 le)
{
    SetLc(pApdu, lc);
    AddLe(pApdu, le);
}

/**
 * Reserves bytes for the LC in the command APDU and updated the pApdu data structure to match.
 * Must be called once in case the APDU cmd has a command data section.
 * \pre pApdu->hasData has been set.
 * \param[in,out] pApdu        APDU buffer
*/
static void ReserveLc(apdu_t * pApdu)
{
    pApdu->lcLength = 0;

    assert(pApdu->hasData != 0);

    if (pApdu->hasData) {
        if (pApdu->extendedLength) {
            pApdu->lcLength = 3;
        }
        else {
            pApdu->lcLength = 1;
        }
    }

    pApdu->offset += pApdu->lcLength;
    pApdu->buflen += pApdu->lcLength;
}

/**
* Sets the LC value in the command APDU.
* @pre ReserveLc(...) has been called or there is no command data section
* @param[in,out] pApdu APDU buffer
* @param[in]     lc    LC value to be set
*/
static void SetLc(apdu_t * pApdu, U16 lc)
{
    assert ( (pApdu->lcLength != 0) || (pApdu->hasData == 0) );

    // NOTE:
    // pApdu->lcLength was set to its proper value in a call to ReserveLc(...)

    if (pApdu->hasData) {
        if (pApdu->extendedLength) {
            pApdu->lc = lc;
            // pApdu->lcLength = 3;
            pApdu->pBuf[APDU_OFFSET_LC] = 0x00;
            pApdu->pBuf[APDU_OFFSET_LC + 1] = (U8)(lc >> 8);
            pApdu->pBuf[APDU_OFFSET_LC + 2] = (U8)(lc & 0xFF);
        }
        else {
            pApdu->lc = lc;
            // pApdu->lcLength = 1;
            pApdu->pBuf[APDU_OFFSET_LC] = (U8)(lc & 0xFF);
        }
    }
    else {
        pApdu->lcLength = 0;
    }
}

/**
* Adds the LE value to the command APDU.
* @param pApdu              [IN/OUT] APDU buffer
* @param le                 [IN] LE
* @return
*/
static void AddLe(apdu_t * pApdu, U16 le)
{
    pApdu->hasLe = true;
    pApdu->le = le;

    if (pApdu->extendedLength) {
        if (pApdu->hasData) {
            pApdu->pBuf[pApdu->offset] = (U8)(le >> 8);
            pApdu->pBuf[pApdu->offset + 1] = (U8)(le & 0xFF);
            pApdu->leLength = 2;
        }
        else {
            pApdu->pBuf[pApdu->offset] = 0x00;
            pApdu->pBuf[pApdu->offset + 1] = (U8)(le >> 8);
            pApdu->pBuf[pApdu->offset + 2] = (U8)(le & 0xFF);
            pApdu->leLength = 3;
        }
    }
    else {
        // regular length
        pApdu->pBuf[pApdu->offset] = (U8)(le & 0xFF);
        pApdu->leLength = 1;
    }

    pApdu->offset += pApdu->leLength;
    pApdu->buflen += pApdu->leLength;
}


#ifndef TGT_A71CH
/**
* @function             AddTlvItem
* @description          Adds a Tag-Length-Value structure to the command APDU.
* @param pApdu          [IN/OUT] APDU buffer.
* @param tag            [IN] tag; either a 1-byte tag or a 2-byte tag
* @param dataLength     [IN] length of the Value
* @param pValue         [IN] Value
* @return               SW_OK or ERR_BUF_TOO_SMALL
*/
U16 AddTlvItem(apdu_t * pApdu, U16 tag, U16 dataLength, const U8 *pValue)
{
    U8 msbTag = tag >> 8;
    U8 lsbTag = tag & 0xff;

    // If this is the first tag added to the buffer, we needs to ensure
    // the correct offset is used writing the data. This depends on
    // whether the APDU is a standard or an extended APDU.
    if (pApdu->hasData == 0)
    {
        pApdu->hasData = 1;
        ReserveLc(pApdu);
    }

    // Ensure no buffer overflow will occur before writing any data to buffer
    {
        U32 xtraData = 0;
        U32 u32_Offset = (U32)(pApdu->offset);

        xtraData = 1;
        // Tag
        if (msbTag != 0x00)
        {
            // 2-byte tag
            xtraData++;
        }

        // Length
        if (dataLength <= 0x7f)
        {
            // 1-byte length
            xtraData++;
        }
        else if (dataLength <= 0xff)
        {
            // 2-byte length
            xtraData += 2;
        }
        else
        {
            // 3-byte length
            xtraData += 3;
        }
        xtraData += dataLength;

        // Can we still add 'xtraData' to internal buffer without buffer overwrite?
        if ( (u32_Offset + xtraData) > MAX_APDU_BUF_LENGTH)
        {
            // Bufferflow would occur
            return ERR_BUF_TOO_SMALL;
        }
    }

    // Tag
    if (msbTag != 0x00)
    {
        // 2-byte tag
        pApdu->pBuf[pApdu->offset++] = msbTag;
    }
    pApdu->pBuf[pApdu->offset++] = lsbTag;

    // Length
    if (dataLength <= 0x7f)
    {
        // 1-byte length
        pApdu->pBuf[pApdu->offset++] = (U8) dataLength;
        pApdu->lc += 2 + dataLength;
    }
    else if (dataLength <= 0xff)
    {
        // 2-byte length
        pApdu->pBuf[pApdu->offset++] = 0x81;
        pApdu->pBuf[pApdu->offset++] = (U8) dataLength;
        pApdu->lc += 3 + dataLength;
    }
    else
    {
        // 3-byte length
        pApdu->pBuf[pApdu->offset++] = 0x82;
        pApdu->pBuf[pApdu->offset++] = dataLength >> 8;
        pApdu->pBuf[pApdu->offset++] = dataLength & 0xff;
        pApdu->lc += 4 + dataLength;
    }

    // Value
    memcpy(&pApdu->pBuf[pApdu->offset], pValue, dataLength);
    pApdu->offset += dataLength;

    // adapt length
    pApdu->buflen = pApdu->offset;

    return SW_OK;
}

/**
* AddStdCmdData
* \deprecated Use ::smApduAppendCmdData instead
*/
U16 AddStdCmdData(apdu_t * pApdu, U16 dataLen, const U8 *data)
{

    pApdu->hasData = 1;
    ReserveLc(pApdu);

    pApdu->lc += dataLen;

    // Value
    memcpy(&pApdu->pBuf[pApdu->offset], data, dataLen);
    pApdu->offset += dataLen;

    // adapt length
    pApdu->buflen = pApdu->offset;

    return pApdu->offset;
}

/**
* @function                 ParseResponse
* @description              Parses a received Tag-Length-Value structure (response APDU).
* @param pApdu              [IN] APDU buffer
* @param expectedTag        [IN] expected tag; either a 1-byte tag or a 2-byte tag
* @param pLen               [IN,OUT] IN: size of buffer provided; OUT: length of the received Value
* @param pValue             [OUT] received Value
* @return status
*/
U16 ParseResponse(apdu_t *pApdu, U16 expectedTag, U16 *pLen, U8 *pValue)
{
    U16 tag = 0;
    U16 rv = ERR_GENERAL_ERROR;
    int foundTag = 0;
    U16 bufferLen = *pLen;

    *pLen = 0;

    if (pApdu->rxlen < 2) /* minimum: 2 byte for response */
    {
        return ERR_GENERAL_ERROR;
    }
    else
    {
        /* check status returned is okay */
        if ((pApdu->pBuf[pApdu->rxlen - 2] != 0x90) || (pApdu->pBuf[pApdu->rxlen - 1] != 0x00))
        {
            return ERR_GENERAL_ERROR;
        }
        else // response okay
        {
            pApdu->offset = 0;

            do
            {
                U16 len = 0;

                // Ensure we don't parse beyond the APDU Response Data
                if (pApdu->offset >= (pApdu->rxlen -2)) { break; }

                /* get the tag (see ISO 7816-4 annex D); limited to max 2 bytes */
                if ((pApdu->pBuf[pApdu->offset] & 0x1F) != 0x1F) /* 1 byte tag only */
                {
                    tag = (pApdu->pBuf[pApdu->offset] & 0x00FF);
                    pApdu->offset += 1;
                }
                else /* tag consists out of 2 bytes */
                {
                    tag = (pApdu->pBuf[pApdu->offset] << 8) + pApdu->pBuf[pApdu->offset + 1];
                    pApdu->offset += 2;
                }

                // Ensure we don't parse beyond the APDU Response Data
                if (pApdu->offset >= (pApdu->rxlen -2)) { break; }

                // tag is OK
                /* get the length (see ISO 7816-4 annex D) */
                if ((pApdu->pBuf[pApdu->offset] & 0x80) != 0x80)
                {
                    /* 1 byte length */
                    len = (pApdu->pBuf[pApdu->offset++] & 0x00FF);
                }
                else
                {
                    /* length consists of 2 or 3 bytes */

                    U8 additionalBytesForLength = (pApdu->pBuf[pApdu->offset++] & 0x7F);

                    if (additionalBytesForLength == 1)
                    {
                        len = pApdu->pBuf[pApdu->offset];
                        pApdu->offset += 1;
                    }
                    else if (additionalBytesForLength == 2)
                    {
                        len = (pApdu->pBuf[pApdu->offset] << 8) + pApdu->pBuf[pApdu->offset + 1];
                        pApdu->offset += 2;
                    }
                    else
                    {
                        return ERR_GENERAL_ERROR;
                    }
                }

                // Ensure we don't parse beyond the APDU Response Data
                if (pApdu->offset >= (pApdu->rxlen -2)) { break; }

                if (tag == expectedTag)
                {
                    // copy the value
                    if ( (len > 0) && (bufferLen >= len) )
                    {
                        *pLen = len;
                        memcpy(pValue, &pApdu->pBuf[pApdu->offset], *pLen);
                        rv = SW_OK;
                        foundTag = 1;
                        break;
                    }
                    else
                    {
                        rv = ERR_BUF_TOO_SMALL;
                        break;
                    }
                }

                // update the offset
                pApdu->offset += len;
            } while (!foundTag);
        }
    }

    return rv;
}

#endif // TGT_A71CH

/**
* Add or append data to the body of a command APDU.
*/
U16 smApduAppendCmdData(apdu_t *pApdu, const U8 *data, U16 dataLen)
{
    // If this is the first commmand data section added to the buffer, we needs to ensure
    // the correct offset is used writing the data. This depends on
    // whether the APDU is a standard or an extended APDU.
    if (pApdu->hasData == 0)
    {
        pApdu->hasData = 1;
        ReserveLc(pApdu);
    }

    pApdu->lc += dataLen;

    // Value
    memcpy(&pApdu->pBuf[pApdu->offset], data, dataLen);
    pApdu->offset += dataLen;

    // adapt length
    pApdu->buflen = pApdu->offset;

    return pApdu->offset;
}

/**
* Gets the Status Word from the APDU.
* @param[in]      pApdu Pointer to the APDU.
* @param[in,out]  pIsOk IN: Pointer to the error indicator, allowed to be NULL; OUT: Points to '1' in case SW is 0x9000
* @return      Status Word or ::ERR_COMM_ERROR
*/
U16 smGetSw(apdu_t *pApdu, U8 *pIsOk)
{
    U16 sw;
    U16 offset;
    assert(pApdu != NULL);

    if (pApdu->rxlen >= 2)
    {
        offset = pApdu->rxlen - 2;
        sw = (pApdu->pBuf[offset] << 8) + pApdu->pBuf[offset + 1];

        if (pIsOk != NULL)
        {
            if (sw == SW_OK)
            {
                *pIsOk = 1;
            }
            else
            {
                *pIsOk = 0;
            }
        }
    }
    else
    {
        sw = ERR_COMM_ERROR;
        if (pIsOk != NULL)
        {
            *pIsOk = 0;
        }
    }

    return sw;
}

/**
* Retrieve the response data of the APDU response, in case the status word matches ::SW_OK
*/
U16 smApduGetResponseBody(apdu_t *pApdu, U8 *buf, U16 *bufLen)
{
    if (pApdu->rxlen < 2) /* minimum: 2 byte for response */
    {
        *bufLen = 0;
        return ERR_GENERAL_ERROR;
    }
    else
    {
        /* check status returned is okay */
        if ((pApdu->pBuf[pApdu->rxlen - 2] != 0x90) || (pApdu->pBuf[pApdu->rxlen - 1] != 0x00))
        {
            *bufLen = 0;
            return ERR_GENERAL_ERROR;
        }
        else // response okay
        {
            pApdu->offset = 0;

            if ( (pApdu->rxlen - 2) > *bufLen)
            {
                *bufLen = 0;
                return ERR_BUF_TOO_SMALL;
            }
            else
            {
                *bufLen = pApdu->rxlen - 2;
                memcpy(buf, &(pApdu->pBuf[pApdu->offset]), pApdu->rxlen - 2);
            }
        }
    }

    return SW_OK;
}
