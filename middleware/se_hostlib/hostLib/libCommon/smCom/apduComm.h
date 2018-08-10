/**
* @file apduComm.h
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
} apdu_t;

#endif //_APDUCOMM_H_
