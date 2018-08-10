/**
 * @file smComSCI2c.h
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
 *
 * @par Description
 * This file provides the API of the SmCom SCI2C communication layer.
 *
 *****************************************************************************/

#ifndef _SMCOMSCI2C_H_
#define _SMCOMSCI2C_H_

#include "smCom.h"

#define ESTABLISH_SCI2C 0x00
#define RESUME_SCI2C    0x01

U16 smComSCI2C_Close(U8 mode);

#ifndef TGT_A71CH
/**
 * Initializes or resumes the SCI2C communication layer. Deprecated, use smComSCI2C_Open instead
 * @param mode      Either ::ESTABLISH_SCI2C to open or re-open communication with a SM, or ::RESUME_SCI2C to resume communication (typically handover from boot loader to main OS)
 * @param seqCnt    Ignored in case mode==::ESTABLISH_SCI2C; SCI2C protocol seqCnt to set in case communication is resumed.
 * @param SCI2Catr     IN: Pointer to buffer to contain SCI2C_ATR value
 * @param SCI2CatrLen  IN: Size of buffer provided; OUT: Actual length of atr retrieved
 * @return
 */
void smComSCI2C_Init(U8 mode, U8 seqCnt, U8 *SCI2Catr, U16 *SCI2CatrLen); // Deprecated
#endif

/**
 * Initializes or resumes the SCI2C communication layer.
 * @param mode      Either ::ESTABLISH_SCI2C to open or re-open communication with a SM, or ::RESUME_SCI2C to resume communication (typically handover from boot loader to main OS)
 * @param seqCnt    Ignored in case mode==::ESTABLISH_SCI2C; SCI2C protocol seqCnt to set in case communication is resumed.
 * @param SCI2Catr     IN: Pointer to buffer to contain SCI2C_ATR value
 * @param SCI2CatrLen  IN: Size of buffer provided; OUT: Actual length of atr retrieved
 * @return
 */
U16 smComSCI2C_Open(U8 mode, U8 seqCnt, U8 *SCI2Catr, U16 *SCI2CatrLen);


#endif /* _SMCOMSCI2C_H_ */
