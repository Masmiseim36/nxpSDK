/*--------------------------------------------------------------------------*/
/* Copyright 2021-2024 NXP                                                  */
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
 * @file  mcuxClEcc_Internal_Types.c
 * @brief Instantiation of the type descriptors supported by the mcuxClEcc component.
 */

#include <mcuxClEcc_Constants.h>

#include <internal/mcuxClEcc_EdDSA_Internal.h>
#include <internal/mcuxClEcc_ECDSA_Internal.h>

/**********************************************************/
/* Key pair generation descriptors                        */
/**********************************************************/
const mcuxClEcc_EdDSA_GenerateKeyPairDescriptor_t mcuxClEcc_EdDsa_GeneratePrivKeyDescriptor =
{
    .options       = MCUXCLECC_EDDSA_PRIVKEY_GENERATE,
    .pPrivKeyInput = NULL
};


/**********************************************************/
/* Signature protocol descriptors                         */
/**********************************************************/

const mcuxClEcc_EdDSA_SignatureProtocolDescriptor_t mcuxClEcc_EdDsa_Ed25519ProtocolDescriptor =
{
    .generateOption = 0u,
    .verifyOption   = 0u,
    .phflag             = MCUXCLECC_EDDSA_PHFLAG_ZERO,
    .pHashPrefix    = NULL,
    .hashPrefixLen      = 0u
};


const mcuxClEcc_ECDSA_SignatureProtocolDescriptor_t mcuxClEcc_ECDSA_ProtocolDescriptor =
{
    .pBlindedEphemeralKeyGenFct           = mcuxClEcc_ECDSA_BlindedEphemeralKeyGen_RandomWithExtraBits,
    .pBlindedEphemeralKeyGenFct_FP_FuncId = MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEcc_ECDSA_BlindedEphemeralKeyGen_RandomWithExtraBits),
    .keyGenCtxSizeInWords                 = 0U,
#ifdef MCUXCL_FEATURE_ECC_ECDSA_DETERMINISTIC
    .pHmacModeDesc                        = NULL /* not needed for regular (randomized) ECDSA */
#endif /* MCUXCL_FEATURE_ECC_ECDSA_DETERMINISTIC */
};

