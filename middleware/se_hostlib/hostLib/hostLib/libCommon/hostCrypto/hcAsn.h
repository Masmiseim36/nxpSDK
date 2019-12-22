/**
 * @file hcAsn.h
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
 * @par History
 *
 */

#ifndef _HCASN_H_
#define _HCASN_H_

#include "sm_types.h"

#ifdef __cplusplus
extern "C" {
#endif


U16 hcAsnToPlainEcc256(U8 *signature, U16 *pSignatureLen, U8 *asnSignature, U16 asnSignatureLen);
U16 hcPlainToAsnEcc256(U8 *asnSignature, U16 *asnSignatureLen, U8 *signature, U16 signatureLen);

U16 hcNormalizeAsnSignatureEcc(U8 *asnSignature, U16 *asnSignatureLen);

#ifdef __cplusplus
}
#endif
#endif
