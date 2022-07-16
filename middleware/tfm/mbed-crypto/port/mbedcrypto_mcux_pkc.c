/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mbedtls/build_info.h"

#if defined(PKC) && MBEDTLS_MCUX_PKC
#include <mcuxClPkc.h>
#include <mcuxClEcc.h>
#include <mcuxClMemory.h>
#include <fsl_pkc.h>
#include <mbedtls/platform.h>
#include <mbedtls/platform_util.h>
#include <mbedtls/ecp.h>
#include <mbedtls/error.h>
#include <mbedtls/ctr_drbg.h>
#include <mcuxClSession.h>

/*!
 * @brief PKC initialization.
 */
status_t mbecrypto_mcux_pkc_init(void)
{
    static bool pkc_init_is_done = false;
    status_t    status;

    if(pkc_init_is_done == false)
    {
        /* Enable PKC related clocks and RAM zeroize */
        status = PKC_PowerDownWakeupInit(PKC);
        if (status == kStatus_Success)
        {
            pkc_init_is_done = true;
        }
    }
    else
    {
        status = kStatus_Success;
    }
    
    return status;
}

/**
 * \brief                 This function sets up the domain parameters to be used in the
 *                        mcuxCl implementation, which requires big-endian input parameters
 *                        (the endianess of parameters is inverted).
 *
 * \param grp             The ECP group to use. This must be initialized and have
 *                        domain parameters loaded, for example through
 *                        mbedtls_ecp_load() or mbedtls_ecp_tls_read_group().
 * \param pDomainParams   Structure to hold the domain parameters in suitable format
 *                        for the mcuxCl implementation. The buffers for the parameters must be alocated
 *                        before this function is called.
 * 
 * \retval                \c 0 on success.
 * \retval                \c MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED error code on failure.
 */
static int mbedtls_ecp_setupDomainParams(mbedtls_ecp_group *grp, mcuxClEcc_DomainParam_t *pDomainParams)
{
    /* Byte-length of prime p. */
    const uint32_t pByteLength = (grp->pbits + 7u) / 8u;
    /* Byte-length of group-order n. */
    const uint32_t nByteLength = (grp->nbits + 7u) / 8u;

    pDomainParams->misc = (pByteLength & 0xFFu) | ((nByteLength << 8) & 0xFF00u);
    
    /* Convert endianess of domain parameters. */
    if(0 != mbedtls_mpi_write_binary(&grp->P, (unsigned char *)pDomainParams->pP, pByteLength))
    {
        return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    }
    /* For SECP R1 curves, MBEDTLS does not provide curve parameter a; calculate it here as follows: a = p - 3 */
    if((MBEDTLS_ECP_DP_SECP192R1 == grp->id) || (MBEDTLS_ECP_DP_SECP256R1 == grp->id) || (MBEDTLS_ECP_DP_SECP521R1 == grp->id) || (MBEDTLS_ECP_DP_SECP384R1 == grp->id))
    {
        // only correct for secp192r1, secp256r1, secp384r1, secp521r1
        if(0 != mbedtls_mpi_write_binary(&grp->P, (unsigned char *)pDomainParams->pA, pByteLength))
        {
            return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
        }
        uint8_t last_byte = pDomainParams->pA[pByteLength-1u] - 3u;
        (void) memcpy( (void *) (pDomainParams->pA + pByteLength - 1u) , (void const *) &last_byte, 1 );
    }
    else if(MBEDTLS_ECP_DP_SECP224R1 == grp->id)
    {
        // can also be done by memcpy P[0:16]*2 -> A and A[16] -= 1, A[pByteLength-1] -= 1
        const uint8_t A[28u] = {0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFEU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFEU};
        (void) memcpy( (void *) pDomainParams->pA , (void const *) A, pByteLength );
    }
    else
    {
        if(0 != mbedtls_mpi_write_binary(&grp->A, (unsigned char *)pDomainParams->pA, pByteLength))
        {
            return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
        }
    }
    if(0 != mbedtls_mpi_write_binary(&grp->B, (unsigned char *)pDomainParams->pB, pByteLength))
    {
        return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    }
    if(0 != mbedtls_mpi_write_binary(&grp->G.MBEDTLS_PRIVATE(X), (unsigned char *)pDomainParams->pG, pByteLength))
    {
        return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    }
    if(0 != mbedtls_mpi_write_binary(&grp->G.MBEDTLS_PRIVATE(Y), (unsigned char *)pDomainParams->pG + pByteLength, pByteLength))
    {
        return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    }
    if(0 != mbedtls_mpi_write_binary(&grp->N, (unsigned char *)pDomainParams->pN, nByteLength))
    {
        return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    }

    return 0;
}

/******************************************************************************/
/*************************** ECDH *********************************************/
/******************************************************************************/
#if defined(MBEDTLS_ECDH_C) && defined(MBEDTLS_MCUX_PKC_ECDH) && MBEDTLS_MCUX_PKC_ECDH

/* Check configuration */
#if defined(MBEDTLS_ECP_DP_CURVE25519_ENABLED) || defined(MBEDTLS_ECP_DP_CURVE448_ENABLED)
#error "MBEDTLS_ECP_DP_CURVE25519 and MBEDTLS_ECP_DP_CURVE448 not supported by alternative implementation"
#endif
#if defined(MBEDTLS_ECP_RESTARTABLE)
#error "MBEDTLS_ECP_RESTARTABLE not supported"
#endif

/* Parameter validation macros based on platform_util.h */
#define ECDH_VALIDATE_RET( cond )    \
    MBEDTLS_INTERNAL_VALIDATE_RET( cond, MBEDTLS_ERR_ECP_BAD_INPUT_DATA )
#define ECDH_VALIDATE( cond )        \
    MBEDTLS_INTERNAL_VALIDATE( cond )

static void mbedtls_ecp_free_ecdh(mcuxClEcc_DomainParam_t* pDomainParams, mcuxClEcc_PointMult_Param_t* pPointMultParams)
{
    /* Avoid double free in mbedtls_ecdh_gen_public */
    if(pDomainParams != NULL && pPointMultParams != NULL)
    {
        if(pDomainParams->pG != pPointMultParams->pPoint)
        {
            mbedtls_free((void*)pPointMultParams->pPoint);
        }
    }

    /* Avoid accessing a NULL pointer. Freeing a NULL pointer is fine. */
    if(pDomainParams != NULL)
    {
        mbedtls_free((void*)pDomainParams->pA);
        mbedtls_free((void*)pDomainParams->pB);
        mbedtls_free((void*)pDomainParams->pP);
        mbedtls_free((void*)pDomainParams->pG);
        mbedtls_free((void*)pDomainParams->pN);
    }

    /* Avoid accessing a NULL pointer. Freeing a NULL pointer is fine. */
    if(pPointMultParams != NULL)
    {
        mbedtls_free((void*)pPointMultParams->pScalar);
        mbedtls_free((void*)pPointMultParams->pResult);
    }

}

#if defined(MBEDTLS_ECDH_GEN_PUBLIC_ALT)
/*
 * Generate public key
 */
int mbedtls_ecdh_gen_public( mbedtls_ecp_group *grp, mbedtls_mpi *d, mbedtls_ecp_point *Q,
                     int (*f_rng)(void *, unsigned char *, size_t),
                     void *p_rng )
{
    /* Check input parameters. */
    ECDH_VALIDATE_RET( grp != NULL );
    ECDH_VALIDATE_RET( d != NULL );
    ECDH_VALIDATE_RET( Q != NULL );
    ECDH_VALIDATE_RET( f_rng != NULL );

    /* Initialize PKC */
    status_t ret_hw_init = mbecrypto_mcux_pkc_init();
    if( 0 != ret_hw_init )
    {
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }

    /* Byte-length of prime p. */
    const uint32_t pByteLength = (grp->pbits + 7u) / 8u;
    /* Byte-length of group-order n. */
    const uint32_t nByteLength = (grp->nbits + 7u) / 8u;

    /*Setup session. */
    mcuxClSession_Descriptor_t session;
    const uint32_t wordSizePkcWa = MCUXCLECC_POINTMULT_WAPKC_SIZE(pByteLength, nByteLength);
    (void) mcuxClSession_init(&session,
                             NULL, /* CPU workarea size for point multiplication is zero */
                             MCUXCLECC_POINTMULT_WACPU_SIZE,
                             (uint32_t *) MCUXCLPKC_RAM_START_ADDRESS + 2,
                             wordSizePkcWa);

    /* Set up domain parameters. */
    mcuxClEcc_DomainParam_t pDomainParams =
    {
        .pA = mbedtls_calloc(pByteLength, sizeof(uint8_t)),
        .pB = mbedtls_calloc(pByteLength, sizeof(uint8_t)),
        .pP = mbedtls_calloc(pByteLength, sizeof(uint8_t)),
        .pG = mbedtls_calloc(pByteLength*2u, sizeof(uint8_t)),
        .pN = mbedtls_calloc(nByteLength, sizeof(uint8_t)),
        .misc = 0
    };
    if(0u != mbedtls_ecp_setupDomainParams(grp, &pDomainParams))
    {
        mbedtls_ecp_free_ecdh(&pDomainParams, NULL);
        return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    }

    /* Set up ECC point multiplication parameters. */
    mbedtls_ctr_drbg_context rng_ctx;
    rng_ctx.MBEDTLS_PRIVATE(prediction_resistance) = 0u;
    uint8_t* pScalar = mbedtls_calloc(nByteLength, sizeof(uint8_t));

    if(0u != f_rng(&rng_ctx, pScalar, nByteLength))
    {
        return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    }

    uint8_t* pResult = mbedtls_calloc(pByteLength*2u, sizeof(uint8_t));
    mcuxClEcc_PointMult_Param_t PointMultParams =
    {
        .curveParam = pDomainParams,
        .pScalar = pScalar,
        .pPoint =  pDomainParams.pG,
        .pResult = pResult,
        .optLen = 0u
    };

    /* Call ECC point multiplication. */
    MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(retEccPointMult, tokenEccPointMult,mcuxClEcc_PointMult(&session, &PointMultParams));
    if (MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEcc_PointMult) != tokenEccPointMult)
    {
        mbedtls_ecp_free_ecdh(&pDomainParams, &PointMultParams);
        return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    }
    if(MCUXCLECC_STATUS_POINTMULT_INVALID_PARAMS == retEccPointMult)
    {
        mbedtls_ecp_free_ecdh(&pDomainParams, &PointMultParams);
        return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
    }
    else if(MCUXCLECC_STATUS_POINTMULT_RNG_ERROR == retEccPointMult)
    {
        mbedtls_ecp_free_ecdh(&pDomainParams, &PointMultParams);
        return MBEDTLS_ERR_ECP_RANDOM_FAILED;
    }
    else if(MCUXCLECC_STATUS_POINTMULT_OK != retEccPointMult)
    {
        mbedtls_ecp_free_ecdh(&pDomainParams, &PointMultParams);
        return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    }
    else
    {
        /* Convert generated point from big-endian representation to mbedtls_mpi. */
        mbedtls_mpi_read_binary(d, pScalar, nByteLength);
        mbedtls_mpi_read_binary(&Q->MBEDTLS_PRIVATE(X), PointMultParams.pResult, pByteLength);
        mbedtls_mpi_read_binary(&Q->MBEDTLS_PRIVATE(Y), PointMultParams.pResult + pByteLength, pByteLength);

        mbedtls_mpi_lset(&Q->MBEDTLS_PRIVATE(Z), 1);
        
        /* Free allocated memory */
        mbedtls_ecp_free_ecdh(&pDomainParams, &PointMultParams);
        
        /* Clean session. */
        (void) mcuxClSession_cleanup(&session);
        (void) mcuxClSession_destroy(&session);
    }

    return 0;
}
#endif /* MBEDTLS_ECDH_GEN_PUBLIC_ALT */


#if defined(MBEDTLS_ECDH_COMPUTE_SHARED_ALT)
/*
 * Compute shared secret (SEC1 3.3.1)
 */
int mbedtls_ecdh_compute_shared( mbedtls_ecp_group *grp, mbedtls_mpi *z,
                         const mbedtls_ecp_point *Q, const mbedtls_mpi *d,
                         int (*f_rng)(void *, unsigned char *, size_t),
                         void *p_rng )
{
    /* Check input parameters. */
    ECDH_VALIDATE_RET( grp != NULL );
    ECDH_VALIDATE_RET( Q != NULL );
    ECDH_VALIDATE_RET( d != NULL );
    ECDH_VALIDATE_RET( z != NULL );

    /* Initialize PKC */
    status_t ret_hw_init = mbecrypto_mcux_pkc_init();
    if( 0 != ret_hw_init )
    {
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }

    /* Byte-length of prime p. */
    const uint32_t pByteLength = (grp->pbits + 7u) / 8u;
    /* Byte-length of group-order n. */
    const uint32_t nByteLength = (grp->nbits + 7u) / 8u;

    /*Setup session. */
    mcuxClSession_Descriptor_t session;
    const uint32_t wordSizePkcWa = MCUXCLECC_POINTMULT_WAPKC_SIZE(pByteLength, nByteLength);
    (void) mcuxClSession_init(&session,
                             NULL, /* CPU workarea size for point multiplication is zero */
                             MCUXCLECC_POINTMULT_WACPU_SIZE,
                             (uint32_t *) MCUXCLPKC_RAM_START_ADDRESS + 2,
                             wordSizePkcWa);

    /* Set up domain parameters. */
    mcuxClEcc_DomainParam_t pDomainParams =
    {
        .pA = mbedtls_calloc(pByteLength, sizeof(uint8_t)),
        .pB = mbedtls_calloc(pByteLength, sizeof(uint8_t)),
        .pP = mbedtls_calloc(pByteLength, sizeof(uint8_t)),
        .pG = mbedtls_calloc(pByteLength*2u, sizeof(uint8_t)),
        .pN = mbedtls_calloc(nByteLength, sizeof(uint8_t)),
        .misc = 0
    };
    if(0u != mbedtls_ecp_setupDomainParams(grp, &pDomainParams))
    {
        mbedtls_ecp_free_ecdh(&pDomainParams, NULL);
        return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    }

    /* Set up ECC point multiplication parameters. */
    uint8_t* pResult = mbedtls_calloc(pByteLength*2u, sizeof(uint8_t));
    mcuxClEcc_PointMult_Param_t PointMultParams =
    {
        .curveParam = pDomainParams,
        .pScalar = mbedtls_calloc(nByteLength, sizeof(uint8_t)),
        .pPoint =  mbedtls_calloc(pByteLength*2, sizeof(uint8_t)),
        .pResult = pResult,
        .optLen = 0u
    };
    if(0u != mbedtls_mpi_write_binary(&Q->MBEDTLS_PRIVATE(X), (unsigned char *)PointMultParams.pPoint, pByteLength))
    {
        mbedtls_ecp_free_ecdh(&pDomainParams, &PointMultParams);
        return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    }
    if(0u != mbedtls_mpi_write_binary(&Q->MBEDTLS_PRIVATE(Y), (unsigned char *)PointMultParams.pPoint + pByteLength, pByteLength))
    {
        mbedtls_ecp_free_ecdh(&pDomainParams, &PointMultParams);
        return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    }
    if(0u != mbedtls_mpi_write_binary(d, (unsigned char *)PointMultParams.pScalar, nByteLength))
    {
        mbedtls_ecp_free_ecdh(&pDomainParams, &PointMultParams);
        return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    }
   
    /* Call ECC point multiplication. */
    MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(retEccPointMult, tokenEccPointMult,mcuxClEcc_PointMult(&session, &PointMultParams));
    if (MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEcc_PointMult) != tokenEccPointMult)
    {
        mbedtls_ecp_free_ecdh(&pDomainParams, &PointMultParams);
        return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    }
    if(MCUXCLECC_STATUS_POINTMULT_INVALID_PARAMS == retEccPointMult)
    {
        mbedtls_ecp_free_ecdh(&pDomainParams, &PointMultParams);
        return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
    }
    else if(MCUXCLECC_STATUS_POINTMULT_RNG_ERROR == retEccPointMult)
    {
        mbedtls_ecp_free_ecdh(&pDomainParams, &PointMultParams);
        return MBEDTLS_ERR_ECP_RANDOM_FAILED;
    }
    else if(MCUXCLECC_STATUS_POINTMULT_OK != retEccPointMult)
    {
        mbedtls_ecp_free_ecdh(&pDomainParams, &PointMultParams);
        return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    }
    else
    {
        /* Convert shared secret from big-endian representation to mbedtls_mpi. */
        mbedtls_mpi_read_binary(z, PointMultParams.pResult, pByteLength);
        
        /* Free allocated memory */
        mbedtls_ecp_free_ecdh(&pDomainParams, &PointMultParams);

        /* Clean session. */
        (void) mcuxClSession_cleanup(&session);
        (void) mcuxClSession_destroy(&session);
    }
    
    return 0;
}
#endif /* MBEDTLS_ECDH_COMPUTE_SHARED_ALT */

#endif /* MBEDTLS_ECDH_C && MBEDTLS_MCUX_PKC_ECDH */

/******************************************************************************/
/*************************** ECDSA ********************************************/
/******************************************************************************/
#if defined(MBEDTLS_ECDSA_C) && defined(MBEDTLS_MCUX_PKC_ECDSA) && MBEDTLS_MCUX_PKC_ECDSA
#include <mbedtls/ecdsa.h>

/* Parameter validation macros based on platform_util.h */
#define ECDSA_VALIDATE_RET( cond )    \
    MBEDTLS_INTERNAL_VALIDATE_RET( cond, MBEDTLS_ERR_ECP_BAD_INPUT_DATA )
#define ECDSA_VALIDATE( cond )        \
    MBEDTLS_INTERNAL_VALIDATE( cond )

static void mbedtls_ecp_free_ecdsa(mcuxClEcc_DomainParam_t* pDomainParams, mcuxClEcc_PointMult_Param_t* pPointMultParams, 
                            mcuxClEcc_Verify_Param_t* pVerifyParams, mcuxClEcc_Sign_Param_t* pSignParams)
{
    /* Avoid accessing a NULL pointer. Freeing a NULL pointer is fine. */
    if(pDomainParams != NULL)
    {
        mbedtls_free((void*)pDomainParams->pA);
        mbedtls_free((void*)pDomainParams->pB);
        mbedtls_free((void*)pDomainParams->pP);
        mbedtls_free((void*)pDomainParams->pG);
        mbedtls_free((void*)pDomainParams->pN);
    }

    /* Avoid accessing a NULL pointer. Freeing a NULL pointer is fine. */
    if(pPointMultParams != NULL)
    {
        mbedtls_free((void*)pPointMultParams->pScalar);
        mbedtls_free((void*)pPointMultParams->pResult);
    }

    /* Avoid accessing a NULL pointer. Freeing a NULL pointer is fine. */
    if(pVerifyParams != NULL)
    {
        mbedtls_free((void*)pVerifyParams->pSignature);
        mbedtls_free((void*)pVerifyParams->pPublicKey);
        mbedtls_free((void*)pVerifyParams->pOutputR);
    }

    /* Avoid accessing a NULL pointer. Freeing a NULL pointer is fine. */
    if(pSignParams != NULL)
    {
        mbedtls_free((void*)pSignParams->pPrivateKey);
        mbedtls_free((void*)pSignParams->pSignature);
    }
}

/*
 * Compute ECDSA signature of a hashed message (SEC1 4.1.3)
 * Obviously, compared to SEC1 4.1.3, we skip step 4 (hash message)
 */
#if defined(MBEDTLS_ECDSA_SIGN_ALT)
/*
 * Compute ECDSA signature of a hashed message
 */
int mbedtls_ecdsa_sign( mbedtls_ecp_group *grp, mbedtls_mpi *r, mbedtls_mpi *s,
                const mbedtls_mpi *d, const unsigned char *buf, size_t blen,
                int (*f_rng)(void *, unsigned char *, size_t), void *p_rng )
{
    /* Check input parameters. */
    ECDSA_VALIDATE_RET( grp   != NULL );
    ECDSA_VALIDATE_RET( r     != NULL );
    ECDSA_VALIDATE_RET( s     != NULL );
    ECDSA_VALIDATE_RET( d     != NULL );
    ECDSA_VALIDATE_RET( f_rng != NULL );
    ECDSA_VALIDATE_RET( buf   != NULL || blen == 0 );

    /* Initialize PKC */
    status_t ret_hw_init = mbecrypto_mcux_pkc_init();
    if( 0 != ret_hw_init )
    {
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }

    /* Byte-length of prime p. */
    const uint32_t pByteLength = (grp->pbits + 7u) / 8u;
    /* Byte-length of group-order n. */
    const uint32_t nByteLength = (grp->nbits + 7u) / 8u;
    
    /* Setup session */
    mcuxClSession_Descriptor_t session;
    const uint32_t wordSizePkcWa = MCUXCLECC_POINTMULT_WAPKC_SIZE(pByteLength, nByteLength);
    (void) mcuxClSession_init(&session,
                             NULL, /* CPU workarea size for point multiplication is zero */
                             MCUXCLECC_POINTMULT_WACPU_SIZE,
                             (uint32_t *) MCUXCLPKC_RAM_START_ADDRESS + 2,
                             wordSizePkcWa);

    /* Set up domain parameters. */
    mcuxClEcc_DomainParam_t pDomainParams =
    {
        .pA = mbedtls_calloc(pByteLength, sizeof(uint8_t)),
        .pB = mbedtls_calloc(pByteLength, sizeof(uint8_t)),
        .pP = mbedtls_calloc(pByteLength, sizeof(uint8_t)),
        .pG = mbedtls_calloc(pByteLength*2u, sizeof(uint8_t)),
        .pN = mbedtls_calloc(nByteLength, sizeof(uint8_t)),
        .misc = 0
    };
    if(0u != mbedtls_ecp_setupDomainParams(grp, &pDomainParams))
    {
        mbedtls_ecp_free_ecdsa(&pDomainParams, NULL, NULL, NULL);
        return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    }

    /* Set up ECC sign parameters. */
    uint8_t* pPrivateKey = mbedtls_calloc(nByteLength, sizeof(uint8_t));

    if(0 != mbedtls_mpi_write_binary(d, (unsigned char *)pPrivateKey, nByteLength))
    {
        mbedtls_ecp_free_ecdsa(&pDomainParams, NULL, NULL, NULL);
        mbedtls_free(pPrivateKey);
        return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    }
    
    uint8_t* pSignature = mbedtls_calloc(nByteLength*2u, sizeof(uint8_t));

    mcuxClEcc_Sign_Param_t paramSign =
    {
        .curveParam = pDomainParams,
        .pHash = buf,
        .pPrivateKey = pPrivateKey,
        .pSignature = pSignature,
        .optLen = mcuxClEcc_Sign_Param_optLen_Pack(blen)
    };
    
    /* Call ECC sign. */
    MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(retEccSign, tokenEccSign,mcuxClEcc_Sign(&session, &paramSign));
    if (MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEcc_Sign) != tokenEccSign)
    {
        mbedtls_ecp_free_ecdsa(&pDomainParams, NULL, NULL, &paramSign);
        return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    }
    if(MCUXCLECC_STATUS_SIGN_INVALID_PARAMS == retEccSign)
    {
        mbedtls_ecp_free_ecdsa(&pDomainParams, NULL, NULL, &paramSign);
        return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
    }
    else if(MCUXCLECC_STATUS_SIGN_RNG_ERROR == retEccSign)
    {
        mbedtls_ecp_free_ecdsa(&pDomainParams, NULL, NULL, &paramSign);
        return MBEDTLS_ERR_ECP_RANDOM_FAILED;
    }
    else if(MCUXCLECC_STATUS_SIGN_OK != retEccSign)
    {
        mbedtls_ecp_free_ecdsa(&pDomainParams, NULL, NULL, &paramSign);
        return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    }
    else /* MCUXCLECC_STATUS_SIGN_OK */
    {
        /* Convert signature from big-endian representation to mbedtls_mpi. */
        (void) mbedtls_mpi_read_binary(r, paramSign.pSignature, nByteLength);
        (void) mbedtls_mpi_read_binary(s, paramSign.pSignature + nByteLength, nByteLength);

        /* Free allocated memory */
        mbedtls_ecp_free_ecdsa(&pDomainParams, NULL, NULL, &paramSign);

        /* Clean session. */
        (void) mcuxClSession_cleanup(&session);
        (void) mcuxClSession_destroy(&session);

        return 0;
    }
}

int mbedtls_ecdsa_can_do( mbedtls_ecp_group_id gid )
{
    switch( gid )
    {
#ifdef MBEDTLS_ECP_DP_CURVE25519_ENABLED
        case MBEDTLS_ECP_DP_CURVE25519: return 0;
#endif
#ifdef MBEDTLS_ECP_DP_CURVE448_ENABLED
        case MBEDTLS_ECP_DP_CURVE448: return 0;
#endif
    default: return 1;
    }
}
#endif /* MBEDTLS_ECDSA_SIGN_ALT */

#if defined(MBEDTLS_ECDSA_VERIFY_ALT)
/*
 * Verify ECDSA signature of hashed message
 */
int mbedtls_ecdsa_verify( mbedtls_ecp_group *grp,
                          const unsigned char *buf, size_t blen,
                          const mbedtls_ecp_point *Q,
                          const mbedtls_mpi *r,
                          const mbedtls_mpi *s)
{
    /* Check input parameters. */
    ECDSA_VALIDATE_RET( grp != NULL );
    ECDSA_VALIDATE_RET( Q   != NULL );
    ECDSA_VALIDATE_RET( r   != NULL );
    ECDSA_VALIDATE_RET( s   != NULL );
    ECDSA_VALIDATE_RET( buf != NULL || blen == 0 );

    /* Initialize PKC */
    status_t ret_hw_init = mbecrypto_mcux_pkc_init();
    if( 0 != ret_hw_init )
    {
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }

    /* Byte-length of prime p. */
    const uint32_t pByteLength = (grp->pbits + 7u) / 8u;
    /* Byte-length of group-order n. */
    const uint32_t nByteLength = (grp->nbits + 7u) / 8u;
    
    /* Setup session */
    mcuxClSession_Descriptor_t session;
    const uint32_t wordSizePkcWa = MCUXCLECC_POINTMULT_WAPKC_SIZE(pByteLength, nByteLength);
    (void) mcuxClSession_init(&session,
                             NULL, /* CPU workarea size for point multiplication is zero */
                             MCUXCLECC_POINTMULT_WACPU_SIZE,
                             (uint32_t *) MCUXCLPKC_RAM_START_ADDRESS + 2,
                             wordSizePkcWa);


    /* Set up domain parameters. */
    mcuxClEcc_DomainParam_t pDomainParams =
    {
        .pA = mbedtls_calloc(pByteLength, sizeof(uint8_t)),
        .pB = mbedtls_calloc(pByteLength, sizeof(uint8_t)),
        .pP = mbedtls_calloc(pByteLength, sizeof(uint8_t)),
        .pG = mbedtls_calloc(pByteLength*2u, sizeof(uint8_t)),
        .pN = mbedtls_calloc(nByteLength, sizeof(uint8_t)),
        .misc = 0
    };
    if(0u != mbedtls_ecp_setupDomainParams(grp, &pDomainParams))
    {
        mbedtls_ecp_free_ecdsa(&pDomainParams, NULL, NULL, NULL);
        return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    }

    /* Prepare the scalar to compute PrecG. The formula for the scalar is: 2 ^ (4 * nByteLength). */
    uint8_t* pScalarPrecG = mbedtls_calloc(nByteLength, sizeof(uint8_t));

    uint32_t scalarBitIndex = 4u * nByteLength;
    pScalarPrecG[nByteLength - 1u - (scalarBitIndex / 8u)] = (uint8_t) 1u << (scalarBitIndex & 7u);

    /* Set up ECC point multiplication parameters for the precomputed point PrecG required by mcuxClEcc_Verify. */
    uint8_t* pResult = mbedtls_calloc(pByteLength*2u, sizeof(uint8_t));
    mcuxClEcc_PointMult_Param_t pointMultParams =
    {
     .curveParam = pDomainParams,
     .pScalar = pScalarPrecG,
     .pPoint = pDomainParams.pG,
     .pResult = pResult,
     .optLen = 0u
    };

    /* Call ECC point multiplication. */
    MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(retEccPointMult, tokenEccPointMult,mcuxClEcc_PointMult(&session, &pointMultParams));
    if (MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEcc_PointMult) != tokenEccPointMult)
    {
        mbedtls_ecp_free_ecdsa(&pDomainParams, &pointMultParams, NULL, NULL);
        return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    }
    if(MCUXCLECC_STATUS_POINTMULT_INVALID_PARAMS == retEccPointMult)
    {
        mbedtls_ecp_free_ecdsa(&pDomainParams, &pointMultParams, NULL, NULL);
        return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
    }
    else if(MCUXCLECC_STATUS_POINTMULT_OK != retEccPointMult)
    {
        mbedtls_ecp_free_ecdsa(&pDomainParams, &pointMultParams, NULL, NULL);
        return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    }
    else /* MCUXCLECC_STATUS_POINTMULT_OK */
    {
        /* Set up ECC verify parameters. */
        uint8_t* pSignature = mbedtls_calloc(nByteLength*2u, sizeof(uint8_t));
        if(0 != mbedtls_mpi_write_binary(r, (unsigned char *)pSignature, nByteLength))
        {
            mbedtls_ecp_free_ecdsa(&pDomainParams, &pointMultParams, NULL, NULL);
            mbedtls_free(pSignature);
            return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
        }
        if(0 != mbedtls_mpi_write_binary(s, (unsigned char *)pSignature + nByteLength, nByteLength))
        {
            mbedtls_ecp_free_ecdsa(&pDomainParams, &pointMultParams, NULL, NULL);
            mbedtls_free(pSignature);
            return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
        }

        uint8_t* pPublicKey = mbedtls_calloc(pByteLength*2u, sizeof(uint8_t));
        if(0 != mbedtls_mpi_write_binary(&Q->MBEDTLS_PRIVATE(X), (unsigned char *)pPublicKey, pByteLength))
        {
            mbedtls_ecp_free_ecdsa(&pDomainParams, &pointMultParams, NULL, NULL);
            mbedtls_free(pSignature);
            mbedtls_free(pPublicKey);
            return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
        }
        if(0 != mbedtls_mpi_write_binary(&Q->MBEDTLS_PRIVATE(Y), (unsigned char *)pPublicKey + pByteLength, pByteLength))
        {
            mbedtls_ecp_free_ecdsa(&pDomainParams, &pointMultParams, NULL, NULL);
            mbedtls_free(pSignature);
            mbedtls_free(pPublicKey);
            return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
        }

        uint8_t* pOutputR = mbedtls_calloc(nByteLength, sizeof(uint8_t));
        mcuxClEcc_Verify_Param_t paramVerify =
        {
           .curveParam = pDomainParams,
           .pPrecG = pResult,
           .pHash = (const uint8_t *) buf,
           .pSignature = pSignature,
           .pPublicKey = pPublicKey,
           .pOutputR = pOutputR,
           .optLen = mcuxClEcc_Verify_Param_optLen_Pack(blen)
        };
        
        /* Call ECC verify. */
        MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(retEccVerify, tokenEccVerify,mcuxClEcc_Verify(&session, &paramVerify));
        /* Note: according to mbedtls headers, the return code at failure is indeed MBEDTLS_ERR_ECP_BAD_INPUT_DATA and not MBEDTLS_ERR_ECP_VERIFY_FAILED. */
        if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEcc_Verify) != tokenEccVerify) || (MCUXCLECC_STATUS_VERIFY_OK != retEccVerify))
        {
            mbedtls_ecp_free_ecdsa(&pDomainParams, &pointMultParams, &paramVerify, NULL);
            return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
        }

        /* Free allocated memory */
        mbedtls_ecp_free_ecdsa(&pDomainParams, &pointMultParams, &paramVerify, NULL);

        /* Note: mcuxClEcc_Verify outputs the calculated signature R if verification is successful, but mbedtls has no such output, so it is dropped. */

        /* Clean session. */
        (void) mcuxClSession_cleanup(&session);
        (void) mcuxClSession_destroy(&session);

        return 0;
    }
}
#endif /* MBEDTLS_ECDSA_VERIFY_ALT */

#if defined(MBEDTLS_ECDSA_GENKEY_ALT)
/*
 * Generate key pair
 */
int mbedtls_ecdsa_genkey( mbedtls_ecdsa_context *ctx, mbedtls_ecp_group_id gid,
                  int (*f_rng)(void *, unsigned char *, size_t), void *p_rng )
{
    /* Check input parameters. */
    ECDSA_VALIDATE_RET( ctx   != NULL );
    ECDSA_VALIDATE_RET( f_rng != NULL );

    /* Set up the group context from the given gid. */
    int ret = mbedtls_ecp_group_load( &ctx->MBEDTLS_PRIVATE(grp), gid );
    if( ret != 0 )
    {
        return( ret );
    }

    /* Initialize PKC */
    status_t ret_hw_init = mbecrypto_mcux_pkc_init();
    if( 0 != ret_hw_init )
    {
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }

    /* Byte-length of prime p. */
    const uint32_t pByteLength = (ctx->MBEDTLS_PRIVATE(grp).pbits + 7u) / 8u;
    /* Byte-length of group-order n. */
    const uint32_t nByteLength = (ctx->MBEDTLS_PRIVATE(grp).nbits + 7u) / 8u;

    /* Setup session */
    mcuxClSession_Descriptor_t session;
    const uint32_t wordSizePkcWa = MCUXCLECC_POINTMULT_WAPKC_SIZE(pByteLength, nByteLength);
    (void) mcuxClSession_init(&session,
                             NULL, /* CPU workarea size for point multiplication is zero */
                             MCUXCLECC_POINTMULT_WACPU_SIZE,
                             (uint32_t *) MCUXCLPKC_RAM_START_ADDRESS + 2,
                             wordSizePkcWa);

    /* Set up domain parameters. */
    mcuxClEcc_DomainParam_t pDomainParams =
    {
        .pA = mbedtls_calloc(pByteLength, sizeof(uint8_t)),
        .pB = mbedtls_calloc(pByteLength, sizeof(uint8_t)),
        .pP = mbedtls_calloc(pByteLength, sizeof(uint8_t)),
        .pG = mbedtls_calloc(pByteLength*2u, sizeof(uint8_t)),
        .pN = mbedtls_calloc(nByteLength, sizeof(uint8_t)),
        .misc = 0
    };
    if(0u != mbedtls_ecp_setupDomainParams(&ctx->MBEDTLS_PRIVATE(grp), &pDomainParams))
    {
        mbedtls_ecp_free_ecdsa(&pDomainParams, NULL, NULL, NULL);
        return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    }

    /* Set up ECC point multiplication parameters. */
    mbedtls_ctr_drbg_context rng_ctx;
    rng_ctx.MBEDTLS_PRIVATE(prediction_resistance) = 0u;
    uint8_t* pScalar = mbedtls_calloc(nByteLength, sizeof(uint8_t));

    if(0u != f_rng(&rng_ctx, pScalar, nByteLength))
    {
        return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    }

    uint8_t* pResult = mbedtls_calloc(pByteLength*2u, sizeof(uint8_t));
    mcuxClEcc_PointMult_Param_t PointMultParams =
    {
        .curveParam = pDomainParams,
        .pScalar = pScalar,
        .pPoint =  pDomainParams.pG,
        .pResult = pResult,
        .optLen = 0u
    };

    /* Call ECC point multiplication. */
    MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(retEccPointMult, tokenEccPointMult,mcuxClEcc_PointMult(&session, &PointMultParams));
    if (MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEcc_PointMult) != tokenEccPointMult)
    {
        mbedtls_ecp_free_ecdsa(&pDomainParams, &PointMultParams, NULL, NULL);
        return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    }
    if(MCUXCLECC_STATUS_POINTMULT_INVALID_PARAMS == retEccPointMult)
    {
        mbedtls_ecp_free_ecdsa(&pDomainParams, &PointMultParams, NULL, NULL);
        return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
    }
    else if(MCUXCLECC_STATUS_POINTMULT_RNG_ERROR == retEccPointMult)
    {
        mbedtls_ecp_free_ecdsa(&pDomainParams, &PointMultParams, NULL, NULL);
        return MBEDTLS_ERR_ECP_RANDOM_FAILED;
    }
    else if(MCUXCLECC_STATUS_POINTMULT_OK != retEccPointMult)
    {
        mbedtls_ecp_free_ecdsa(&pDomainParams, &PointMultParams, NULL, NULL);
        return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    }
    else
    {
        /* Convert generated point from big-endian representation to mbedtls_mpi. */
        mbedtls_mpi_read_binary(&ctx->MBEDTLS_PRIVATE(d), pScalar, nByteLength);
        mbedtls_mpi_read_binary(&ctx->MBEDTLS_PRIVATE(Q).MBEDTLS_PRIVATE(X), PointMultParams.pResult, pByteLength);
        mbedtls_mpi_read_binary(&ctx->MBEDTLS_PRIVATE(Q).MBEDTLS_PRIVATE(Y), PointMultParams.pResult + pByteLength, pByteLength);

        /* Free allocated memory */
        mbedtls_ecp_free_ecdsa(&pDomainParams, &PointMultParams, NULL, NULL);
        
        /* Clean session. */
        (void) mcuxClSession_cleanup(&session);
        (void) mcuxClSession_destroy(&session);
    }

    return 0;
}
#endif /* MBEDTLS_ECDSA_GENKEY_ALT */

#endif /* MBEDTLS_ECDSA_C && MBEDTLS_MCUX_PKC_ECDSA */

/******************************************************************************/
/*************************** RSA *********************************************/
/******************************************************************************/
#if defined(MBEDTLS_RSA_C) && defined(MBEDTLS_MCUX_PKC_RSA) && MBEDTLS_MCUX_PKC_RSA

#include <mcuxClRsa.h>              // Interface to the entire mcuxClRsa component
#include <mbedtls/rsa.h>

/* Parameter validation macros */
#define RSA_VALIDATE_RET( cond )                                       \
    MBEDTLS_INTERNAL_VALIDATE_RET( cond, MBEDTLS_ERR_RSA_BAD_INPUT_DATA )
#define RSA_VALIDATE( cond )                                           \
    MBEDTLS_INTERNAL_VALIDATE( cond )

extern int rsa_check_context( mbedtls_rsa_context const *ctx, int is_priv,
                              int blinding_needed );

#if defined(MBEDTLS_RSA_PUBLIC_ALT)
/*
 * Do an RSA public key operation
 */
int mbedtls_rsa_public( mbedtls_rsa_context *ctx,
                const unsigned char *input,
                unsigned char *output )
{
    RSA_VALIDATE_RET( ctx != NULL );
    RSA_VALIDATE_RET( input != NULL );
    RSA_VALIDATE_RET( output != NULL );

    if( rsa_check_context( ctx, 0 /* public */, 0 /* no blinding */ ) )
    {
        return( MBEDTLS_ERR_RSA_BAD_INPUT_DATA );
    }

    /**************************************************************************/
    /* Preparation                                                            */
    /**************************************************************************/

    /* Initialize PKC */
    status_t ret_hw_init = mbecrypto_mcux_pkc_init();
    if( 0 != ret_hw_init )
    {
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }

    /* Create session handle to be used by verify function */

    /* Get the byte-length of modulus n */
    const uint32_t nByteLength = ctx->MBEDTLS_PRIVATE(len);

    /* CPU buffer */
    uint32_t cpuWaBuffer[MCUXCLRSA_VERIFY_OPTIONNOVERIFY_WACPU_SIZE / sizeof(uint32_t)];

    /* PKC buffer and size */
    uint8_t *pPkcRam = (uint8_t *) MCUXCLPKC_RAM_START_ADDRESS;
    const uint32_t pkcWaSize = MCUXCLPKC_ROUNDUP_SIZE(nByteLength /* modulus */
                                                   + nByteLength /* exponent */
                                                   + nByteLength /* result buffer */);

    mcuxClSession_Descriptor_t sessionDesc;
    mcuxClSession_Handle_t session = &sessionDesc;

    MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(si_status, si_token, mcuxClSession_init(
                /* mcuxClSession_Handle_t session:      */ session,
                /* uint32_t * const cpuWaBuffer:       */ cpuWaBuffer,
                /* uint32_t cpuWaSize:                 */ MCUXCLRSA_VERIFY_OPTIONNOVERIFY_WACPU_SIZE / sizeof(uint32_t),
                /* uint32_t * const pkcWaBuffer:       */ (uint32_t *) pPkcRam,
                /* uint32_t pkcWaSize:                 */ (pkcWaSize + MCUXCLRSA_VERIFY_OPTIONNOVERIFY_WAPKC_SIZE(nByteLength * 8u)) / sizeof(uint32_t)
                ));

    if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClSession_init) != si_token) || (MCUXCLSESSION_STATUS_OK != si_status))
    {
        return MBEDTLS_ERR_RSA_PUBLIC_FAILED;
    }

    /* Set pointers in PKC */
    uint8_t *pMod = pPkcRam;
    uint8_t *pExp = pMod + nByteLength;
    uint8_t *pBuf = pExp + nByteLength;

    /* Create key struct of type MCUXCLRSA_KEY_PUBLIC */

    /* Get actual parameter lengths */
    size_t modByteLength = (mbedtls_mpi_bitlen(&ctx->MBEDTLS_PRIVATE(N)) + 7u) / 8u;
    size_t expByteLength = (mbedtls_mpi_bitlen(&ctx->MBEDTLS_PRIVATE(E)) + 7u) / 8u;

    /* Check actual length with length given in the context. */
    if( nByteLength != modByteLength )
    {
        return( MBEDTLS_ERR_RSA_BAD_INPUT_DATA );
    }

    /* Use mbedtls function to extract key parameters in big-endian order */
    mbedtls_mpi_write_binary(&ctx->MBEDTLS_PRIVATE(N), pMod, modByteLength);
    mbedtls_mpi_write_binary(&ctx->MBEDTLS_PRIVATE(E), pExp, expByteLength);

    const mcuxClRsa_KeyEntry_t kMod = {
                       .pKeyEntryData = (uint8_t *) pMod,
                       .keyEntryLength = (uint32_t) modByteLength };

    const mcuxClRsa_KeyEntry_t kExp = {
                       .pKeyEntryData = (uint8_t *) pExp,
                       .keyEntryLength = (uint32_t) expByteLength };

    const mcuxClRsa_Key public_key = {
                                     .keytype = MCUXCLRSA_KEY_PUBLIC,
                                     .pMod1 = (mcuxClRsa_KeyEntry_t *)&kMod,
                                     .pMod2 = NULL,
                                     .pQInv = NULL,
                                     .pExp1 = (mcuxClRsa_KeyEntry_t *)&kExp,
                                     .pExp2 = NULL,
                                     .pExp3 = NULL };


    session->pkcWa.used += pkcWaSize / sizeof(uint32_t);

    /**************************************************************************/
    /* RSA verify call                                                        */
    /**************************************************************************/

    MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(verify_result, verify_token, mcuxClRsa_verify(
                /* mcuxClSession_Handle_t           pSession: */           session,
                /* const mcuxClRsa_Key * const      pKey: */               &public_key,
                /* const uint8_t * const           pMessageOrDigest: */   NULL,
                /* const uint32_t                  messageLength: */      0u,
                /* uint8_t * const                 pSignature: */         (uint8_t *)input,
                /* const mcuxClRsa_SignVerifyMode   pVerifyMode: */        (mcuxClRsa_SignVerifyMode_t *)&mcuxClRsa_Mode_Verify_NoVerify,
                /* const uint32_t                  saltLength: */         0u,
                /* uint32_t                        options: */            0u,
                /* uint8_t * const                 pOutput: */            (uint8_t *)pBuf));

    if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRsa_verify) != verify_token) || (MCUXCLRSA_STATUS_VERIFYPRIMITIVE_OK != verify_result))
    {
        return MBEDTLS_ERR_RSA_PUBLIC_FAILED;
    }

    session->pkcWa.used -= pkcWaSize / sizeof(uint32_t);

    /* Copy result buffer to output */
    MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(retMemCpy, tokenMemCpy,
            mcuxClMemory_copy((uint8_t *) output, pBuf, nByteLength, nByteLength) );

    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMemory_copy) != tokenMemCpy) && (0u != retMemCpy) )
    {
        return MBEDTLS_ERR_RSA_PUBLIC_FAILED;
    }

    /**************************************************************************/
    /* Session clean-up                                                       */
    /**************************************************************************/

    MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(cleanup_result, cleanup_token, mcuxClSession_cleanup(
                /* mcuxClSession_Handle_t           pSession: */           session));

    if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClSession_cleanup) != cleanup_token) || (MCUXCLSESSION_STATUS_OK != cleanup_result))
    {
        return MBEDTLS_ERR_RSA_PUBLIC_FAILED;
    }

    MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(destroy_result, destroy_token, mcuxClSession_destroy(
                /* mcuxClSession_Handle_t           pSession: */           session));

    if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClSession_destroy) != destroy_token) || (MCUXCLSESSION_STATUS_OK != destroy_result))
    {
        return MBEDTLS_ERR_RSA_PUBLIC_FAILED;
    }

    return( 0 );
}
#endif /* MBEDTLS_RSA_PUBLIC_ALT */

#if defined(MBEDTLS_RSA_PRIVATE_ALT)
/*
 * Do an RSA private key operation
 */
int mbedtls_rsa_private( mbedtls_rsa_context *ctx,
                 int (*f_rng)(void *, unsigned char *, size_t),
                 void *p_rng,
                 const unsigned char *input,
                 unsigned char *output )
{
    RSA_VALIDATE_RET( ctx != NULL );
    RSA_VALIDATE_RET( input != NULL );
    RSA_VALIDATE_RET( output != NULL );

    if( rsa_check_context( ctx, 1 /* private */, 1 /* blinding */ ) != 0 )
    {
        return( MBEDTLS_ERR_RSA_BAD_INPUT_DATA );
    }

    /**************************************************************************/
    /* Preparation                                                            */
    /**************************************************************************/

    /* Initialize PKC */
    status_t ret_hw_init = mbecrypto_mcux_pkc_init();
    if( 0 != ret_hw_init )
    {
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }

    /* Create session handle to be used by sign function */

    /* Get the byte-length of modulus n */
    const uint32_t nByteLength = ctx->MBEDTLS_PRIVATE(len);
    const uint32_t pqByteLength = (nByteLength+1) / 2u;

    /* CPU buffer */
    uint32_t cpuWaBuffer[MCUXCLRSA_SIGN_CRT_OPTIONNOENCODE_2048_WACPU_SIZE / sizeof(uint32_t)];

    /* PKC buffer and size */
    uint8_t *pPkcRam = (uint8_t *) MCUXCLPKC_RAM_START_ADDRESS;
    const uint32_t pkcWaSize = MCUXCLPKC_ROUNDUP_SIZE((2u * pqByteLength) /* p and q (2 * 1/2 * nByteLength) */
                                                   + (pqByteLength)      /* q_inv */
                                                   + (2u * pqByteLength) /* dp and dq (2 * 1/2 * nByteLength) */
                                                   + (nByteLength)      /* e */
                                                   + (nByteLength)      /* result buffer */);

    mcuxClSession_Descriptor_t sessionDesc;
    mcuxClSession_Handle_t session = &sessionDesc;

    MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(si_status, si_token, mcuxClSession_init(
                /* mcuxClSession_Handle_t session:      */ session,
                /* uint32_t * const cpuWaBuffer:       */ cpuWaBuffer,
                /* uint32_t cpuWaSize:                 */ MCUXCLRSA_SIGN_CRT_OPTIONNOENCODE_WACPU_SIZE(nByteLength * 8u) / sizeof(uint32_t),
                /* uint32_t * const pkcWaBuffer:       */ (uint32_t *) pPkcRam,
                /* uint32_t pkcWaSize:                 */ (pkcWaSize + MCUXCLRSA_SIGN_CRT_OPTIONNOENCODE_WAPKC_SIZE(nByteLength * 8u)) / sizeof(uint32_t)
                ));

    if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClSession_init) != si_token) || (MCUXCLSESSION_STATUS_OK != si_status))
    {
        return MBEDTLS_ERR_RSA_PRIVATE_FAILED;
    }

    /* Set pointers in PKC */
    uint8_t *pP = pPkcRam;
    uint8_t *pQ = pP + pqByteLength;
    uint8_t *pQ_inv = pQ + pqByteLength;
    uint8_t *pDP = pQ_inv + pqByteLength;
    uint8_t *pDQ = pDP + pqByteLength;
    uint8_t *pE = pDQ + pqByteLength;
    uint8_t *pBuf = pE + nByteLength;

    /* Create key struct of type MCUXCLRSA_KEY_PRIVATECRT */

    /* Get actual parameter lengths */
    size_t pByteLength     = (mbedtls_mpi_bitlen(&ctx->MBEDTLS_PRIVATE(P)) + 7u) / 8u;
    size_t qByteLength     = (mbedtls_mpi_bitlen(&ctx->MBEDTLS_PRIVATE(Q)) + 7u) / 8u;
    size_t q_invByteLength = (mbedtls_mpi_bitlen(&ctx->MBEDTLS_PRIVATE(QP)) + 7u) / 8u;
    size_t dpByteLength    = (mbedtls_mpi_bitlen(&ctx->MBEDTLS_PRIVATE(DP)) + 7u) / 8u;
    size_t dqByteLength    = (mbedtls_mpi_bitlen(&ctx->MBEDTLS_PRIVATE(DQ)) + 7u) / 8u;
    size_t eByteLength     = (mbedtls_mpi_bitlen(&ctx->MBEDTLS_PRIVATE(E)) + 7u) / 8u;

    /* Check actual length with length given in the context. */
    if( (pqByteLength != pByteLength) || (pqByteLength != qByteLength) )
    {
        return( MBEDTLS_ERR_RSA_BAD_INPUT_DATA );
    }

    /* Use mbedtls function to extract key parameters in big-endian order */
    mbedtls_mpi_write_binary(&ctx->MBEDTLS_PRIVATE(P), pP, pByteLength);
    mbedtls_mpi_write_binary(&ctx->MBEDTLS_PRIVATE(Q), pQ, qByteLength);
    mbedtls_mpi_write_binary(&ctx->MBEDTLS_PRIVATE(QP), pQ_inv, q_invByteLength);
    mbedtls_mpi_write_binary(&ctx->MBEDTLS_PRIVATE(DP), pDP, dpByteLength);
    mbedtls_mpi_write_binary(&ctx->MBEDTLS_PRIVATE(DQ), pDQ, dqByteLength);
    mbedtls_mpi_write_binary(&ctx->MBEDTLS_PRIVATE(E), pE, eByteLength);

    const mcuxClRsa_KeyEntry_t kP = {
                       .pKeyEntryData = (uint8_t *) pP,
                       .keyEntryLength = (uint32_t) pByteLength };

    const mcuxClRsa_KeyEntry_t kQ = {
                       .pKeyEntryData = (uint8_t *) pQ,
                       .keyEntryLength = (uint32_t) qByteLength };

    const mcuxClRsa_KeyEntry_t kQ_inv = {
                       .pKeyEntryData = (uint8_t *) pQ_inv,
                       .keyEntryLength = (uint32_t) q_invByteLength };

    const mcuxClRsa_KeyEntry_t kDP = {
                       .pKeyEntryData = (uint8_t *) pDP,
                       .keyEntryLength = (uint32_t) dpByteLength };

    const mcuxClRsa_KeyEntry_t kDQ = {
                       .pKeyEntryData = (uint8_t *) pDQ,
                       .keyEntryLength = (uint32_t) dqByteLength };

    const mcuxClRsa_KeyEntry_t kE = {
                       .pKeyEntryData = (uint8_t *) pE,
                       .keyEntryLength = (uint32_t) eByteLength };

    const mcuxClRsa_Key private_key = {
                                     .keytype = MCUXCLRSA_KEY_PRIVATECRT,
                                     .pMod1 = (mcuxClRsa_KeyEntry_t *)&kP,
                                     .pMod2 = (mcuxClRsa_KeyEntry_t *)&kQ,
                                     .pQInv = (mcuxClRsa_KeyEntry_t *)&kQ_inv,
                                     .pExp1 = (mcuxClRsa_KeyEntry_t *)&kDP,
                                     .pExp2 = (mcuxClRsa_KeyEntry_t *)&kDQ,
                                     .pExp3 = (mcuxClRsa_KeyEntry_t *)&kE };

    session->pkcWa.used += pkcWaSize / sizeof(uint32_t);

    /**************************************************************************/
    /* RSA sign call                                                          */
    /**************************************************************************/

    MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(sign_result, sign_token, mcuxClRsa_sign(
                /*  mcuxClSession_Handle_t           pSession,  */           session,
                /*  const mcuxClRsa_Key * const      pKey,  */               &private_key,
                /*  const uint8_t * const           pMessageOrDigest,  */   (uint8_t *)input,
                /*  const uint32_t                  messageLength,  */      0u,
                /*  const mcuxClRsa_SignVerifyMode   pPaddingMode,  */       (mcuxClRsa_SignVerifyMode_t *)&mcuxClRsa_Mode_Sign_NoEncode,
                /*  const uint32_t                  saltLength,  */         0u,
                /*  const uint32_t                  options,  */            0u,
                /*  uint8_t * const                 pSignature)  */         (uint8_t *)pBuf));

    if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRsa_sign) != sign_token) || (MCUXCLRSA_STATUS_SIGN_OK != sign_result))
    {
        if (MCUXCLRSA_STATUS_INVALID_INPUT == sign_result)
        {
            return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
        }
        else
        {
            return MBEDTLS_ERR_RSA_PRIVATE_FAILED;
        }
    }

    session->pkcWa.used -= pkcWaSize / sizeof(uint32_t);

    /* Copy result buffer to output */
    MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(retMemCpy, tokenMemCpy,
            mcuxClMemory_copy((uint8_t *) output, pBuf, nByteLength, nByteLength) );

    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMemory_copy) != tokenMemCpy) && (0u != retMemCpy) )
    {
        return MBEDTLS_ERR_RSA_PRIVATE_FAILED;
    }

    /**************************************************************************/
    /* Session clean-up                                                       */
    /**************************************************************************/

    MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(cleanup_result, cleanup_token, mcuxClSession_cleanup(
                /* mcuxClSession_Handle_t           pSession: */           session));

    if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClSession_cleanup) != cleanup_token) || (MCUXCLSESSION_STATUS_OK != cleanup_result))
    {
        return MBEDTLS_ERR_RSA_PRIVATE_FAILED;
    }

    MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(destroy_result, destroy_token, mcuxClSession_destroy(
                /* mcuxClSession_Handle_t           pSession: */           session));

    if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClSession_destroy) != destroy_token) || (MCUXCLSESSION_STATUS_OK != destroy_result))
    {
        return MBEDTLS_ERR_RSA_PRIVATE_FAILED;
    }

    return( 0 );
}
#endif /* MBEDTLS_RSA_PRIVATE_ALT */

#endif /* MBEDTLS_RSA_C && MBEDTLS_MCUX_PKC_RSA */

#endif /* PKC && MBEDTLS_MCUX_PKC */