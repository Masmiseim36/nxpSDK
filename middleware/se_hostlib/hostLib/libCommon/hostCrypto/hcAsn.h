/**
 * @file hcAsn.h
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
 * @par History
 *
 */

#ifndef _HCASN_H_
#define _HCASN_H_

#include "sm_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef TGT_A71CH
U16 hcAsnToPlainEcc256(U8 *signature, U16 *pSignatureLen, U8 *asnSignature, U16 asnSignatureLen);
U16 hcPlainToAsnEcc256(U8 *asnSignature, U16 *asnSignatureLen, U8 *signature, U16 signatureLen);
#endif
U16 hcNormalizeAsnSignatureEcc(U8 *asnSignature, U16 *asnSignatureLen);

#ifdef __cplusplus
}
#endif
#endif
