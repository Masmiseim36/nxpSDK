/*--------------------------------------------------------------------------*/
/* Copyright 2023 NXP                                                       */
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
 * @file  mcuxClEcc_EdDSA_Internal_SignatureMechanisms.c
 * @brief implementation of functions for signature modes
 */

#include <mcuxClCore_Platform.h>
#include <mcuxClSession.h>
#include <mcuxClBuffer.h>
#include <mcuxClMemory.h>
#include <mcuxCsslFlowProtection.h>
#include <mcuxCsslAnalysis.h>
#include <mcuxClCore_FunctionIdentifiers.h>
#include <mcuxClHash.h>
#include <mcuxClEcc.h>

#include <internal/mcuxClSession_Internal.h>
#include <internal/mcuxClBuffer_Internal.h>
#include <internal/mcuxClEcc_EdDSA_Internal.h>
#include <internal/mcuxClHash_Internal.h>

MCUX_CSSL_FP_FUNCTION_DEF(mcuxClEcc_EdDSA_GenerateHashPrefix)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEcc_Status_t) mcuxClEcc_EdDSA_GenerateHashPrefix(
    const mcuxClEcc_EdDSA_DomainParams_t *pDomainParams,
    uint32_t phflag,
    mcuxCl_InputBuffer_t pContext,
    uint32_t contextLen,
    uint8_t *pHashPrefix)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClEcc_EdDSA_GenerateHashPrefix);

    uint8_t *pHashPrefixTmp = pHashPrefix;

    /* Check whether the pContext is not NULL if the contextLen is set */
    if (((0u < contextLen) && (NULL == pContext)) || (255u < contextLen) || (2u < phflag))
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEcc_EdDSA_GenerateHashPrefix, MCUXCLECC_STATUS_INVALID_PARAMS);
    }

    /* Check whether the pDomPrefix is not NULL if the domPrefixLen is set */
    /* pHashPrefixTmp is not NULL and allocate the buffer pHashPrefix with sufficient size */
    if ((MCUXCLECC_EDDSA_ED25519_DOMPREFIXLEN < pDomainParams->domPrefixLen) || (NULL == pDomainParams->pDomPrefix)
        || (NULL == pHashPrefixTmp))
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEcc_EdDSA_GenerateHashPrefix, MCUXCLECC_STATUS_INVALID_PARAMS);
    }

    /* Write the fixed prefix string for dom2/dom4 to the output buffer */
    MCUXCLMEMORY_FP_MEMORY_COPY_WITH_BUFF(pHashPrefixTmp,
                                        (const uint8_t*)pDomainParams->pDomPrefix,
                                        pDomainParams->domPrefixLen,
                                        MCUXCLECC_EDDSA_SIZE_HASH_PREFIX(pDomainParams->domPrefixLen, contextLen) );
    MCUX_CSSL_ANALYSIS_COVERITY_START_FALSE_POSITIVE(INTEGER_OVERFLOW, "pHashPrefixTmp will be in the valid range pHashPrefix[0 ~ pDomainParams->domPrefixLen+contextLen+1u].")
    pHashPrefixTmp += pDomainParams->domPrefixLen;

    /* Write phflag to the output buffer */
    *pHashPrefixTmp = (uint8_t)phflag;
    pHashPrefixTmp++;
    /* Write contextLen to the output buffer */
    *pHashPrefixTmp = (uint8_t)contextLen;
    pHashPrefixTmp++;
    /* Write pContext to the output buffer */
    if (0u != contextLen)
    {
        MCUX_CSSL_FP_FUNCTION_CALL(retBufferRead, mcuxClBuffer_read(pContext, 0u, pHashPrefixTmp, contextLen));
        if (MCUXCLBUFFER_STATUS_OK != retBufferRead)
        {
            MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEcc_EdDSA_GenerateHashPrefix, MCUXCLECC_STATUS_FAULT_ATTACK);
        }
    }

    MCUX_CSSL_ANALYSIS_COVERITY_STOP_FALSE_POSITIVE(INTEGER_OVERFLOW)
    MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEcc_EdDSA_GenerateHashPrefix, MCUXCLECC_STATUS_OK,
        MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMemory_copy),
        MCUX_CSSL_FP_CONDITIONAL((0u != contextLen),
            MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClBuffer_read)) );
}


MCUX_CSSL_FP_FUNCTION_DEF(mcuxClEcc_EdDSA_PreHashMessage)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEcc_Status_t) mcuxClEcc_EdDSA_PreHashMessage(
    mcuxClSession_Handle_t pSession,
    mcuxClEcc_EdDSA_DomainParams_t *pDomainParams,
    mcuxClEcc_CpuWa_t * const pCpuWorkarea,
    uint32_t phflag,
    mcuxCl_InputBuffer_t pIn,
    uint32_t inSize,
    const uint8_t **pMessage,
    uint32_t *messageSize)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClEcc_EdDSA_PreHashMessage);

    MCUX_CSSL_FP_BRANCH_DECL(phflagSet);
    if (MCUXCLECC_EDDSA_PHFLAG_ONE == phflag)
    {
        /* phflag is set, pre-hash the message */
        uint8_t *pMessageTmp = (uint8_t*) mcuxClSession_allocateWords_cpuWa(pSession, (uint32_t)pDomainParams->algoHash->hashSize / sizeof(uint32_t));
        if (NULL == pMessageTmp)
        {
            MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEcc_EdDSA_PreHashMessage, MCUXCLECC_STATUS_FAULT_ATTACK);
        }
        MCUX_CSSL_ANALYSIS_START_SUPPRESS_INTEGER_WRAP("the result does not wrap ")
        pCpuWorkarea->wordNumCpuWa += pDomainParams->algoHash->hashSize / sizeof(uint32_t);
        MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_INTEGER_WRAP()
        uint32_t hashOutputSize = 0u;

        MCUXCLBUFFER_INIT(buffMessageTemp, NULL, pMessageTmp, (uint32_t) pDomainParams->algoHash->hashSize);
        MCUX_CSSL_FP_FUNCTION_CALL(hash_result, mcuxClHash_compute(pSession, pDomainParams->algoHash, pIn, inSize, buffMessageTemp, &hashOutputSize));

        if (MCUXCLHASH_STATUS_OK != hash_result)
        {
            MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEcc_EdDSA_PreHashMessage, MCUXCLECC_STATUS_FAULT_ATTACK);
        }

        *pMessage = (const uint8_t *) pMessageTmp;
        *messageSize = MCUXCLECC_EDDSA_MESSAGE_DIGEST_SIZE;

        MCUX_CSSL_FP_BRANCH_POSITIVE(phflagSet, MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClHash_compute) );
    }
    else if (MCUXCLECC_EDDSA_PHFLAG_ZERO == phflag)
    {
        /* phflag is not set, the message is not modified */
        *messageSize = inSize;
    }
    else
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEcc_EdDSA_PreHashMessage, MCUXCLECC_STATUS_FAULT_ATTACK);
    }

    MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEcc_EdDSA_PreHashMessage, MCUXCLECC_STATUS_OK,
        MCUX_CSSL_FP_BRANCH_TAKEN_POSITIVE(phflagSet, MCUXCLECC_EDDSA_PHFLAG_ONE == phflag) );
}
