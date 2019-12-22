/**
* @file HLSEMisc.h
* @author NXP Semiconductors
* @version 1.0
* @par License
* Copyright 2017 NXP
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
* Host Lib wrapper API: Miscellaneous functions
*
* @par HISTORY
*
*/
#ifndef _HLSE_MISC_H_
#define _HLSE_MISC_H_

#include "HLSETypes.h"

/**
* Permanently disables the Plain Injection mode
*
* \retval ::HLSE_SW_OK Upon successful execution
*/
HLSE_RET_CODE   HLSE_DisablePlainInjectionMode(void);

/**
* Clears all user data.
*
* \retval ::HLSE_SW_OK Upon successful execution
*/
HLSE_RET_CODE   HLSE_ResetContents(void);


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
