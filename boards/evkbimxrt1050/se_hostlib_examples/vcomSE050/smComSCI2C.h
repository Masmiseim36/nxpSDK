/**
 * @file smComSCI2c.h
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
 * This file provides the API of the SmCom SCI2C communication layer.
 *
 *****************************************************************************/

#ifndef _SMCOMSCI2C_H_
#define _SMCOMSCI2C_H_

#include "smCom.h"

#define ESTABLISH_SCI2C 0x00
#define RESUME_SCI2C    0x01

U16 smComSCI2C_Close(U8 mode);



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
