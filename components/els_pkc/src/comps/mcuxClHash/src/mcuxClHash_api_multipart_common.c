/*--------------------------------------------------------------------------*/
/* Copyright 2021-2023 NXP                                                  */
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

#include <mcuxClToolchain.h>
#include <mcuxClHash.h>
#include <internal/mcuxClHash_Internal.h>
#include <mcuxClSession.h>
#include <mcuxCsslFlowProtection.h>
#include <mcuxClCore_FunctionIdentifiers.h>
#include <mcuxCsslSecureCounter_Cfg.h>

#include <internal/mcuxClSession_Internal_EntryExit.h>


MCUX_CSSL_FP_FUNCTION_DEF(mcuxClHash_init)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClHash_Status_t) mcuxClHash_init(
    mcuxClSession_Handle_t session UNUSED_PARAM,
    mcuxClHash_Context_t pContext,
    mcuxClHash_Algo_t algorithm
)
{
    MCUXCLSESSION_ENTRY(session, mcuxClHash_init, diRefValue, MCUXCLHASH_STATUS_FAULT_ATTACK)

    pContext->unprocessedLength = 0u;
    pContext->processedLength[0] = 0u;
    pContext->processedLength[1] = 0u;
    pContext->processedLength[2] = 0u;
    pContext->processedLength[3] = 0u;
    pContext->algo = algorithm;

    MCUXCLSESSION_EXIT(session, mcuxClHash_init, diRefValue, MCUXCLHASH_STATUS_OK, MCUXCLHASH_STATUS_FAULT_ATTACK)
}

MCUX_CSSL_FP_FUNCTION_DEF(mcuxClHash_process)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClHash_Status_t) mcuxClHash_process(
    mcuxClSession_Handle_t session,
    mcuxClHash_Context_t pContext,
    mcuxCl_InputBuffer_t pIn,
    uint32_t inSize
)
{
    MCUXCLSESSION_ENTRY(session, mcuxClHash_process, diRefValue, MCUXCLHASH_STATUS_FAULT_ATTACK)

    if((NULL == pContext->algo) || (NULL == pContext->algo->processSkeleton)
#if(1 == MCUX_CSSL_SC_USE_SW_LOCAL)
            || (0u == pContext->algo->protection_token_processSkeleton)
#endif /* (1 == MCUX_CSSL_SC_USE_SW_LOCAL) */
            )
    {
        MCUXCLSESSION_EXIT(session, mcuxClHash_process, diRefValue, MCUXCLHASH_STATUS_INVALID_PARAMS, MCUXCLHASH_STATUS_FAULT_ATTACK)
    }

    MCUX_CSSL_FP_FUNCTION_CALL(result, pContext->algo->processSkeleton(session, pContext, pIn, inSize));

    MCUXCLSESSION_EXIT(session,
        mcuxClHash_process,
        diRefValue,
        result,
        MCUXCLHASH_STATUS_FAULT_ATTACK,
        pContext->algo->protection_token_processSkeleton)
}
