/*--------------------------------------------------------------------------*/
/* Copyright 2020-2024 NXP                                                  */
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
 * @file  mcuxClEcc_ECDSA_Internal.h
 * @brief internal header for ECDSA
 */


#ifndef MCUXCLECC_ECDSA_INTERNAL_H_
#define MCUXCLECC_ECDSA_INTERNAL_H_


#include <mcuxClCore_Platform.h>
#include <mcuxClEcc_Types.h>
#include <mcuxClKey_Types.h>
#include <mcuxCsslFlowProtection.h>
#include <mcuxClCore_FunctionIdentifiers.h>
#include <mcuxClSession.h>
#include <mcuxClBuffer.h>
#ifdef MCUXCL_FEATURE_ECC_ECDSA_DETERMINISTIC
#include <mcuxClMac.h>
#include <internal/mcuxClRandomModes_Internal_SizeDefinitions.h>
#include <internal/mcuxClRandomModes_Internal_HmacDrbg_Functions.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif


struct mcuxClEcc_ECDSA_KeyGenCtx;
typedef struct mcuxClEcc_ECDSA_KeyGenCtx mcuxClEcc_ECDSA_KeyGenCtx_t;

/**
 * @brief Cast a pointer to word-aligned data to a pointer to the mcuxClEcc_ECDSA_KeyGenCtx_t type.
 *
 * @param pCtx    The pointer to cast to a proper mcuxClEcc_ECDSA_KeyGenCtx_t* type. Must be aligned.
 */
MCUX_CSSL_FP_FUNCTION_DEF(mcuxClEcc_castToECDSAKeyGenCtx)
static inline mcuxClEcc_ECDSA_KeyGenCtx_t* mcuxClEcc_castToECDSAKeyGenCtx(uint32_t* pCtx)
{
  MCUX_CSSL_ANALYSIS_START_PATTERN_REINTERPRET_MEMORY_OF_OPAQUE_TYPES()
  return (mcuxClEcc_ECDSA_KeyGenCtx_t *) pCtx;
  MCUX_CSSL_ANALYSIS_STOP_PATTERN_REINTERPRET_MEMORY_OF_OPAQUE_TYPES()
}

/**
 * Blinded secret key generation function structure for ECDSA ephemeral key generation.
 */
MCUX_CSSL_FP_FUNCTION_POINTER(mcuxClEcc_ECDSA_EphemeralKeyGenFunction_t,
typedef MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEcc_Status_t) (*mcuxClEcc_ECDSA_EphemeralKeyGenFunction_t)(mcuxClSession_Handle_t pSession,
                                                                                                  const mcuxClEcc_Sign_Param_t * pParam,
                                                                                                  mcuxClEcc_ECDSA_KeyGenCtx_t *pCtx));


/**
 * This function implements an ECDSA ephemeral key generation according to the method
 * "ECDSA Key Pair Generation using Extra Random Bits" specified in appendix A.2.1 of FIPS 186-5.
 * The generation is done in a blinded way and the ephemeral key k is output multiplicatively split
 * as k0 and k1, satisfying k0*k1 mod n = k = (c mod (n-1)) + 1, in which k is derived from a
 * (bitLen(n)+64)-bit true (DRBG) random number c and k0 is a 64-bit random number (with bit 63 set).
 *
 * Inputs:
 *   pSession: pointer to the current session
 *   pParam:   Pointer to the input parameters of the mcuxClEcc_Sign function
 *
 * Inputs in pOperands[] and PKC workarea: N/A.
 *
 * Prerequisites:
 *   ps1Len = (operandSize, operandSize);
 *   curve order n in N, NDash of n in NFULL;
 *   no on-going calculation on N;
 *   buffers S0, S1, S2 and S3 are with doubled-size (2*operandSize).
 *
 * Result in PKC workarea:
 *   buffers S0 and S1 contain multiplicative split private key k0 and k1 (operandSize);
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEcc_ECDSA_BlindedEphemeralKeyGen_RandomWithExtraBits, mcuxClEcc_ECDSA_EphemeralKeyGenFunction_t)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEcc_Status_t) mcuxClEcc_ECDSA_BlindedEphemeralKeyGen_RandomWithExtraBits(
    mcuxClSession_Handle_t pSession,
    const mcuxClEcc_Sign_Param_t * pParam,
    mcuxClEcc_ECDSA_KeyGenCtx_t *pCtx);

#ifdef MCUXCL_FEATURE_ECC_ECDSA_DETERMINISTIC

#define MCUXCLECC_INT_ECDSADETERMINISTIC_CTX_INITIALIZED        (0x742EB8E1U)
typedef struct mcuxClEcc_ECDSA_Deterministic_KeyGenCtx
{
    uint32_t initFlag;
    uint32_t testModeDescriptor[MCUXCLCORE_NUM_OF_CPUWORDS_CEIL(MCUXCLRANDOM_MODE_DESCRIPTOR_SIZE)];
    uint32_t hmacDrbgModeDescriptor[MCUXCLRANDOMMODES_HMAC_DRBG_MODE_DESCRIPTOR_SIZE_IN_WORDS];
    uint32_t randomCtx[MCUXCLRANDOMMODES_HMAC_DRBG_MAX_CONTEXT_SIZE_IN_WORDS];
} mcuxClEcc_ECDSA_Deterministic_KeyGenCtx_t;

/**
 * @brief Cast a pointer to type mcuxClEcc_ECDSA_KeyGenCtx_t to a pointer to the more-specific type mcuxClEcc_ECDSA_Deterministic_KeyGenCtx_t.
 *
 * @param pCtx  The pointer to cast to a proper mcuxClEcc_ECDSA_Deterministic_KeyGenCtx_t* type. Alignment must fit the new type.
 */
MCUX_CSSL_FP_FUNCTION_DEF(mcuxClEcc_castToECDSADeterministicKeyGenCtx)
static inline mcuxClEcc_ECDSA_Deterministic_KeyGenCtx_t* mcuxClEcc_castToECDSADeterministicKeyGenCtx(mcuxClEcc_ECDSA_KeyGenCtx_t* pCtx)
{
  MCUX_CSSL_ANALYSIS_START_PATTERN_CAST_TO_MORE_SPECIFIC_ALIGNED_TYPE()
  return (mcuxClEcc_ECDSA_Deterministic_KeyGenCtx_t *) pCtx;
  MCUX_CSSL_ANALYSIS_STOP_PATTERN_CAST_TO_MORE_SPECIFIC_ALIGNED_TYPE()
}


/**
 * This function implements a deterministic ECDSA ephemeral key generation according to RFC 6979.
 * The generation is done in a blinded way and the ephemeral key k is output multiplicatively split
 * as k0 and k1, satisfying k0*k1 mod n = k = (c mod (n-1)) + 1, in which k is derived from a
 * (bitLen(n)+64)-bit true (DRBG) random number c and k0 is a 64-bit random number (with bit 63 set).
 *
 * Inputs:
 *   pSession: pointer to the current session
 *   pParam:   Pointer to the input parameters of the mcuxClEcc_Sign function
 *
 * Inputs in pOperands[] and PKC workarea: N/A.
 *
 * Prerequisites:
 *   ps1Len = (operandSize, operandSize);
 *   curve order n in N, NDash of n in NFULL;
 *   no on-going calculation on N;
 *   buffers S0, S1, S2 and S3 are with doubled-size (2*operandSize).
 *
 * Result in PKC workarea:
 *   buffers S0 and S1 contain multiplicative split private key k0 and k1 (operandSize);
 *
 * @attention The PKC calculation might be still on-going, call #mcuxClPkc_WaitForFinish before CPU accesses to the result.
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEcc_ECDSA_BlindedEphemeralKeyGen_Deterministic, mcuxClEcc_ECDSA_EphemeralKeyGenFunction_t)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEcc_Status_t) mcuxClEcc_ECDSA_BlindedEphemeralKeyGen_Deterministic(
    mcuxClSession_Handle_t pSession,
    const mcuxClEcc_Sign_Param_t * pParam,
    mcuxClEcc_ECDSA_KeyGenCtx_t *pCtx);
#endif /* MCUXCL_FEATURE_ECC_ECDSA_DETERMINISTIC */


/**
 *  ECDSA SignatureProtocol variant structure.
 */
struct mcuxClEcc_ECDSA_SignatureProtocolDescriptor
{
    mcuxClEcc_ECDSA_EphemeralKeyGenFunction_t pBlindedEphemeralKeyGenFct; ///< Function to be generate multiplicatively split ephemeral key in blinded way
    uint32_t pBlindedEphemeralKeyGenFct_FP_FuncId;                       ///< ID of function to be used for ephemeral key generation
    uint32_t keyGenCtxSizeInWords;                                       ///< Size of pBlindedEphemeralKeyGenFct function context
#ifdef MCUXCL_FEATURE_ECC_ECDSA_DETERMINISTIC
    const mcuxClMac_ModeDescriptor_t *pHmacModeDesc;                      ///< HMAC mode
#endif /* MCUXCL_FEATURE_ECC_ECDSA_DETERMINISTIC */
};


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MCUXCLECC_ECDSA_INTERNAL_H_ */
