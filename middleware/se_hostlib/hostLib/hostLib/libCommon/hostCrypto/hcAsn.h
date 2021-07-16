/**
 * @file hcAsn.h
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 *
 * Copyright 2016 NXP
 * SPDX-License-Identifier: Apache-2.0
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
