/*--------------------------------------------------------------------------*/
/* Copyright 2023-2024 NXP                                                  */
/*                                                                          */
/* NXP Proprietary. This software is owned or controlled by NXP and may     */
/* only be used strictly in accordance with the applicable license terms.   */
/* By expressly accepting such terms or by downloading, installing,         */
/* activating and/or otherwise using the software, you are agreeing that    */
/* you have read, and that you agree to comply with and are bound by, such  */
/* license terms.  If you do not agree to be bound by the applicable        */
/* license terms, then you may not retain, install, activate or otherwise   */
/* use the software.                                                        */
/*--------------------------------------------------------------------------*/

/**
 * @file  mcuxClEcc_ECDSA_GenerateProtocolDescriptor.c
 * @brief Implementation of ECDSA protocol descriptor generation functionality
 */


#include <mcuxClCore_Platform.h>

#include <mcuxClSession.h>
#include <mcuxCsslFlowProtection.h>
#include <mcuxClCore_FunctionIdentifiers.h>
#include <mcuxClEcc.h>
#include <mcuxClMac.h>

#include <internal/mcuxClEcc_Weier_Internal.h>
#include <internal/mcuxClEcc_ECDSA_Internal.h>


#ifdef MCUXCL_FEATURE_ECC_ECDSA_DETERMINISTIC
MCUX_CSSL_FP_FUNCTION_DEF(mcuxClEcc_ECDSA_GenerateProtocolDescriptor_Deterministic)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEcc_Status_t) mcuxClEcc_ECDSA_GenerateProtocolDescriptor_Deterministic(
    mcuxClSession_Handle_t session,
    mcuxClEcc_ECDSA_SignatureProtocolDescriptor_t * const pProtocolDescriptor,
    mcuxClMac_Mode_t hmacMode )
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClEcc_ECDSA_GenerateProtocolDescriptor_Deterministic);
    pProtocolDescriptor->pBlindedEphemeralKeyGenFct = mcuxClEcc_ECDSA_BlindedEphemeralKeyGen_Deterministic;
    pProtocolDescriptor->pBlindedEphemeralKeyGenFct_FP_FuncId = MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEcc_ECDSA_BlindedEphemeralKeyGen_Deterministic);
    pProtocolDescriptor->pHmacModeDesc = hmacMode;
    pProtocolDescriptor->keyGenCtxSizeInWords = sizeof(mcuxClEcc_ECDSA_Deterministic_KeyGenCtx_t) / sizeof(uint32_t);

    MCUX_CSSL_FP_FUNCTION_EXIT_WITH_CHECK(mcuxClEcc_ECDSA_GenerateProtocolDescriptor_Deterministic,
                                         MCUXCLECC_STATUS_OK, MCUXCLECC_STATUS_FAULT_ATTACK);
}
#endif /* MCUXCL_FEATURE_ECC_ECDSA_DETERMINISTIC */
