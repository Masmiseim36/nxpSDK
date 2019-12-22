/**
* @file apduComm.h
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
 *
 */

#ifndef _APDUCOMM_H_
#define _APDUCOMM_H_

#include "sm_types.h"
/// @cond
#define APDU_OFFSET_CLA                 (0)
#define APDU_OFFSET_INS                 (1)
#define APDU_OFFSET_P1                  (2)
#define APDU_OFFSET_P2                  (3)
/// @endcond
#define APDU_OFFSET_LC                  (4) //!< Zero index based offset into the APDU of the LC field.

/**
 * Contains APDU exchanged between Host and Secure Modulde.
 */
typedef struct
{
    U8 cla;
    U8 ins;
    U8 p1;
    U8 p2;
    U8* pBuf;
    U16 buflen;
    U16 rxlen;
    U8 extendedLength;
    U8 hasData;
    U16 lc;
    U8 lcLength;
    U8 hasLe;
    U16 le;
    U8 leLength;
    U16 offset;

#ifdef TGT_A71CL
    U8 txHasChkSum;
    U16 txChkSum;
    U16 txChkSumLength;
    U8 rxHasChkSum;
    U16 rxChkSum;
    U16 rxChkSumLength;
#endif

} apdu_t;

#endif //_APDUCOMM_H_
