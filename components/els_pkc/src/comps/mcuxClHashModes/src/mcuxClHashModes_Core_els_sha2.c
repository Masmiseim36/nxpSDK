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

#include <mcuxClHash.h>
#include <internal/mcuxClHash_Internal.h>
#include <internal/mcuxClHashModes_Core_els_sha2.h>
#include <mcuxClEls.h>
#include <internal/mcuxClEls_Internal.h>


MCUX_CSSL_FP_FUNCTION_DEF(mcuxClHashModes_els_core_sha2, mcuxClHashModes_els_AlgoCore_t)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClHash_Status_t) mcuxClHashModes_els_core_sha2(
                        uint32_t options,
                        mcuxCl_InputBuffer_t pIn,
                        uint32_t inSize,
                        mcuxCl_Buffer_t pOut)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClHashModes_els_core_sha2,
                               MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_Hash_Async));

    mcuxClEls_HashOption_t hash_options;
    hash_options.word.value = options;

    MCUX_CSSL_FP_FUNCTION_CALL(result_hash, mcuxClEls_Hash_Async(hash_options,
                                                               pIn,
                                                               inSize,
                                                               pOut));

    if (MCUXCLELS_STATUS_OK_WAIT != result_hash)
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClHashModes_els_core_sha2, MCUXCLHASH_STATUS_FAILURE);
    }

    MCUX_CSSL_FP_FUNCTION_CALL(result, mcuxClEls_WaitForOperation(MCUXCLELS_ERROR_FLAGS_CLEAR));

    if (MCUXCLELS_STATUS_OK != result)
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClHashModes_els_core_sha2, MCUXCLHASH_STATUS_FAILURE,
                                  MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_WaitForOperation));
    }

    MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClHashModes_els_core_sha2, MCUXCLHASH_STATUS_OK,
                              MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_WaitForOperation));
}

#ifdef MCUXCL_FEATURE_ELS_SHA_DIRECT
MCUX_CSSL_FP_FUNCTION_DEF(mcuxClHashModes_els_core_sha2_direct, mcuxClHashModes_els_AlgoCore_t)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClHash_Status_t) mcuxClHashModes_els_core_sha2_direct(
                        uint32_t options,
                        mcuxCl_InputBuffer_t pIn,
                        uint32_t inSize,
                        mcuxCl_Buffer_t pOut)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClHashModes_els_core_sha2_direct, MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_Hash_ShaDirect));

    mcuxClEls_HashOption_t hash_options;
    hash_options.word.value = options;

    MCUX_CSSL_ANALYSIS_START_PATTERN_NULL_POINTER_CONSTANT()
    MCUX_CSSL_FP_FUNCTION_CALL(result, mcuxClEls_Hash_ShaDirect(hash_options,
                                                              pIn,
                                                              inSize,
                                                              pOut,
                                                              NULL,
                                                              NULL));
    MCUX_CSSL_ANALYSIS_STOP_PATTERN_NULL_POINTER_CONSTANT()

    if(MCUXCLELS_STATUS_SW_INVALID_PARAM == result)
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClHashModes_els_core_sha2_direct, MCUXCLHASH_STATUS_INVALID_PARAMS);
    }
    else if (MCUXCLELS_STATUS_OK != result)
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClHashModes_els_core_sha2_direct, MCUXCLHASH_STATUS_FAILURE);
    }
    else
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClHashModes_els_core_sha2_direct, MCUXCLHASH_STATUS_OK);
    }
}
#endif
