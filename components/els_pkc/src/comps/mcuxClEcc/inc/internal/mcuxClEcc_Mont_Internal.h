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
 * @file  mcuxClEcc_Mont_Internal.h
 * @brief internal header of mcuxClEcc MontDH functionalities
 */


#ifndef MCUXCLECC_MONT_INTERNAL_H_
#define MCUXCLECC_MONT_INTERNAL_H_

#include <mcuxClCore_Platform.h>
#include <mcuxClSession.h>
#include <mcuxCsslFlowProtection.h>
#include <mcuxClCore_FunctionIdentifiers.h>
#include <mcuxClEcc_Types.h>
#include <mcuxClPkc.h>

#include <internal/mcuxClEcc_Internal.h>
#include <internal/mcuxClEcc_Mont_Internal_PkcWaLayout.h>

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************/
/* Internal return codes for MontDH functions             */
/**********************************************************/
// None


/**********************************************************/
/* Internal MontDH defines                                */
/**********************************************************/

/** Use 4-byte (32-bit) multiplicative blinding in MontDH. */
#define MCUXCLECC_MONTDH_SCALAR_BLINDING_BYTELEN  4u


/**********************************************************/
/* Internal MontDH types                                  */
/**********************************************************/

/**
 * Domain parameter structure for MontDH functions.
 */
struct mcuxClEcc_MontDH_DomainParams
{
    mcuxClEcc_CommonDomainParams_t common;  ///< structure containing pointers and lengths for common ECC parameters (see Common ECC Domain parameters)
    uint16_t c;     ///< cofactor exponent
    uint16_t t;     ///< bit position of MSBit of decoded scalar
};


/* Curve25519 domain parameters */
extern const mcuxClEcc_MontDH_DomainParams_t mcuxClEcc_MontDH_DomainParams_Curve25519;

/* Curve448 domain parameters */
extern const mcuxClEcc_MontDH_DomainParams_t mcuxClEcc_MontDH_DomainParams_Curve448;


/**********************************************************/
/* Declarations for internal MontDH functions             */
/**********************************************************/

MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEcc_MontDH_SetupEnvironment)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEcc_Status_t) mcuxClEcc_MontDH_SetupEnvironment(
    mcuxClSession_Handle_t pSession,
    mcuxClEcc_MontDH_DomainParams_t *pDomainParams,
    uint8_t noOfBuffers
    );

MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEcc_MontDH_DecodeScalar)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEcc_Status_t) mcuxClEcc_MontDH_DecodeScalar(
    mcuxClEcc_MontDH_DomainParams_t *pDomainParameters
    );

MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEcc_MontDH_DecodeCoordinate)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEcc_Status_t) mcuxClEcc_MontDH_DecodeCoordinate(
    mcuxClEcc_MontDH_DomainParams_t *pDomainParameters,
    const uint8_t *pCoordinateEnc
    );

MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEcc_Mont_SecureScalarMult_XZMontLadder)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEcc_Status_t) mcuxClEcc_Mont_SecureScalarMult_XZMontLadder(
    mcuxClSession_Handle_t pSession,
    uint8_t iScalar,
    uint32_t scalarBitLength,
    uint32_t optionAffineOrProjective
    );

MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEcc_MontDH_X)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEcc_Status_t) mcuxClEcc_MontDH_X(
    mcuxClSession_Handle_t pSession,
    mcuxClEcc_MontDH_DomainParams_t *pDomainParameters,
    const uint8_t *pCoordinateUEnc
    );


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MCUXCLECC_MONT_INTERNAL_H_ */
