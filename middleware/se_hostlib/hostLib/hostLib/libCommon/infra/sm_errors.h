/**
 * @file sm_errors.h
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
 * This file provides an interface to generic APDU response evaluation.
 * @par History
 * 1.0   20-feb-2012 : Initial version
 *
 */

#ifndef _SM_ERRORS_
#define _SM_ERRORS_

#include "apduComm.h"

#ifdef __cplusplus
extern "C" {
#endif

U16 CheckNoResponseData(apdu_t * pApdu);
U16 CheckNoResponseDataRaw(U8 *rawResponse, U16 rawResponseLen);

#ifdef __cplusplus
}
#endif
#endif //_SM_ERRORS_
