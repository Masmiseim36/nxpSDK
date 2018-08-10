/**
 * @file tst_hlse_a71ch_util.h
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
 * API of test bench utility functions specific to the a71ch using HLSE Generic API
 * @par History
 * 1.0   2016-Oct-1 : Initial version
 */

#ifndef _TST_HLSE_A71CH_UTIL_H_
#define _TST_HLSE_A71CH_UTIL_H_

#include "sm_types.h"
#include "ax_api.h"

#include "HLSEAPI.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \name Module functions
   @{ */
U8 hlse_a71chInitModule(U8 initMode);
U8 hlse_a71chSetupScp03(void);
U8 hlse_a71chShowModuleInfo(U8 *scpState);
/** @}*/

/** \name HLSE helper functions
   @{ */
U16 hlse_GenerateEccKeyPair(SST_Index_t index, HLSE_OBJECT_HANDLE* hObject);
U16 hlse_GetEccPublicKey(U8 *publicKey, U16 *publicKeyLen, HLSE_OBJECT_HANDLE* hObject);
U16 hlse_SetEccKeyPair(SST_Index_t index, const U8 *publicKey, U16 publicKeyLen, const U8 *privateKey, U16 privateKeyLen, HLSE_OBJECT_HANDLE* hObject);
U16 hlse_SetEccPublicKey(SST_Index_t index, const U8 *publicKey, U16 publicKeyLen, HLSE_OBJECT_HANDLE* hObject);
U16 hlse_EcdhGetSharedSecret(HLSE_OBJECT_HANDLE hObject, const U8 *pOtherPublicKey, U16 otherPublicKeyLen, U8 *pSharedSecret, U16 *pSharedSecretLen);
U16 hlse_EccSign(HLSE_OBJECT_HANDLE hObject, const U8 *pHash, U16 hashLen, U8 *pSignature, U16 *pSignatureLen);
U16 hlse_EccNormalizedAsnSign(HLSE_OBJECT_HANDLE hObject, const U8 *pHash, U16 hashLen, U8 *pSignature, U16 *pSignatureLen);
U16 hlse_EccVerifyWithKey(const U8 *pKeyData, U16 keyDataLen, const U8 *pHash, U16 hashLen, const U8 *pSignature, U16 signatureLen, U8 *pResult);

U16 hlse_EccVerify(SST_Index_t index, const U8 *pHash, U16 hashLen, const U8 *pSignature, U16 signatureLen, U8 *pResult);

U16 hlse_GetRandom(U8 *random, U8 randomLen);
U16 hlse_GetUniqueID(U8 *uid, U16 *uidLen);
U16 hlse_InjectLock(void);
U16 hlse_GetCredentialInfo(U8 *map, U16 *mapLen);

U16 hlse_SCP_GP_PutKeys(U8 keyVersion, U8 *keyEnc, U8 *keyMac, U8 *keyDek, U8 *currentKeyDek, U16 keyBytes);
U16 hlse_SCP_Authenticate(U8 *keyEnc, U8 *keyMac, U8 *keyDek, U16 keyBytes, U8 *sCounter, U16 *sCounterLen);

U16 hlse_SetSymKey(SST_Index_t index, const U8 *key, U16 keyLen, HLSE_OBJECT_HANDLE* hObject);

U16 hlse_GetGPDataSize(U16* gpSize);
/** @}*/

#ifdef __cplusplus
}
#endif
#endif // _TST_HLSE_A71CH_UTIL_H_
