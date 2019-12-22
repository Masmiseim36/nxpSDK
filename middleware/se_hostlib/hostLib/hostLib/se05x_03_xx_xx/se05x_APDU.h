/* Copyright 2019 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#ifndef SE050X_APDU_H_INC_RC6
#define SE050X_APDU_H_INC_RC6

#ifdef __cplusplus
extern "C" {
#endif


#include "se05x_tlv.h"
#include "se05x_const.h"
#include "se05x_APDU_apis.h"

/** Se05x_API_DeleteAll_Iterative
*
* Go through each object and delete it individually.
*
* @param[in] session_ctx Session Context [0:pSession]
*/
smStatus_t Se05x_API_DeleteAll_Iterative(pSe05xSession_t session_ctx);

smStatus_t Se05x_API_EC_CurveGetId(pSe05xSession_t session_ctx, uint32_t objectID, SE05x_ECCurve_t *pcurveId);

#define Se05x_API_ECGenSharedSecret Se05x_API_ECDHGenerateSharedSecret
#define Se05x_API_SHAOneShot Se05x_API_DigestOneShot

// For SIMW-656
bool Se05x_IsInValidRangeOfUID(uint32_t uid);

#ifdef __cplusplus
}
#endif

#endif
