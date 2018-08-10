/**
* @file HLSEMisc.h
* @author NXP Semiconductors
* @version 1.0
* @par License
* Copyright(C) NXP Semiconductors, 2017
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
* Host Lib wrapper API: Miscellaneous functions
*
* @par HISTORY
*
*/
#ifndef _HLSE_MISC_H_
#define _HLSE_MISC_H_

#include "HLSETypes.h"

/**
* Permanently disables the Debug API.
*
* \retval ::HLSE_SW_OK Upon successful execution
*/
HLSE_RET_CODE   HLSE_DbgDisableDebug(void);

/**
* Invokes data reflection APDU (facilitates link testing). No check of data payload returned
*
* \param[in] inData The data to be sent to the Secure Element
* \param[in] inDataLen The length of \p inData
* \param[in,out] outData IN: caller passes a buffer to hold the data to be returned;
OUT: contains the retruend data
* \param[in,out] outDataLen IN: length of the \p outData buffer passed;
OUT: the number of bytes returned in \p outData
*
* \retval ::HLSE_SW_OK Upon successful execution
* \retval ::HLSE_ERR_API_ERROR Invalid function arguments
*/
HLSE_RET_CODE   HLSE_DbgReflect(U8* inData, U16 inDataLen,
                                U8* outData, U16* outDataLen);

/**
* Resets the Secure Module to the initial state.
*
* \retval ::HLSE_SW_OK Upon successful execution
*/
HLSE_RET_CODE   HLSE_DbgReset(void);

/**
The purpose of this function is to turn the proprietary ECDSA signature format - that
may be returned by the applet - into a normalized ASN.1 format.

* \param[in] signature     buffer containing the ECDSA signature in the applet specific format; OUT: Signature compliant to ASN.1
* \param[in] signatureLen  length of ECDSA signature length
* \param[in,out] normalizedSignature IN: caller passes a buffer to hold the data to be returned;
OUT: contains the retruend data
* \param[in,out] normalizedSignatureLen IN: length of the \p outData buffer passed;
OUT: the number of bytes returned in \p outData
*
* \retval ::HLSE_SW_OK upon successfull execution
* \retval ::HLSE_ERR_API_ERROR Invalid function arguments
*/
HLSE_RET_CODE   HLSE_NormalizeECCSignature(U8 *signature, U16 signatureLen,
                                           U8 *normalizedSignature, U16 *normalizedSignatureLen);


#endif // _HLSE_MISC_H_
