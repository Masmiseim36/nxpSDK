/*--------------------------------------------------------------------------*/
/* Copyright 2022-2024 NXP                                                  */
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
 * @file  mcuxClRandomModes_CtrDrbg_AES256_DRG3_example.c
 * @brief Example for the mcuxClRandomModes component
 *
 * @example mcuxClRandomModes_CtrDrbg_AES256_DRG3_example.c
 * @brief   Example for the mcuxClRandomModes component
 */

#include <mcuxClToolchain.h>
#include <mcuxClRandom.h>
#include <mcuxClRandomModes.h>
#include <mcuxClSession.h>
#include <mcuxCsslFlowProtection.h>
#include <mcuxClCore_FunctionIdentifiers.h> // Code flow protection
#include <mcuxClExample_Session_Helper.h>
#include <mcuxClCore_Examples.h> // Defines and assertions for examples
#include <mcuxClEls.h> // Interface to the entire mcuxClEls component
#include <mcuxClExample_ELS_Helper.h>

/** Performs an example usage of the mcuxClRandom and mcuxClRandomModes components
 * @retval true  The example code completed successfully
 * @retval false The example code failed */
MCUXCLEXAMPLE_FUNCTION(mcuxClRandomModes_CtrDrbg_AES256_DRG3_example)
{
    /**************************************************************************/
    /* Preparation                                                            */
    /**************************************************************************/

    /** Initialize ELS, Enable the ELS **/
    if(!mcuxClExample_Els_Init(MCUXCLELS_RESET_DO_NOT_CANCEL))
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    mcuxClSession_Descriptor_t sessionDesc;
    mcuxClSession_Handle_t session = &sessionDesc;
    MCUXCLEXAMPLE_ALLOCATE_AND_INITIALIZE_SESSION(session, MCUXCLRANDOMMODES_MAX_CPU_WA_BUFFER_SIZE, 0u);


    /**************************************************************************/
    /* DRBG selftest.                                                         */
    /**************************************************************************/
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(rs_status, selftest_token, mcuxClRandom_selftest(
                                        session,
                                        mcuxClRandomModes_Mode_CtrDrbg_AES256_DRG3
                                   ));

    if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandom_selftest) != selftest_token) || (MCUXCLRANDOM_STATUS_OK != rs_status))
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    uint32_t context[MCUXCLRANDOMMODES_CTR_DRBG_AES256_CONTEXT_SIZE_IN_WORDS] = {0};
    MCUX_CSSL_ANALYSIS_START_PATTERN_REINTERPRET_MEMORY_OF_OPAQUE_TYPES()
    mcuxClRandom_Context_t pRng_ctx = (mcuxClRandom_Context_t)context;
    MCUX_CSSL_ANALYSIS_STOP_PATTERN_REINTERPRET_MEMORY_OF_OPAQUE_TYPES()
    /**************************************************************************/
    /* DRBG initialization                                                    */
    /**************************************************************************/

    /* Initialize an AES-256 CTR_DRBG DRG.3 */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(ri_status, init_token, mcuxClRandom_init(
                                        session,
        MCUX_CSSL_ANALYSIS_START_SUPPRESS_POINTER_INCOMPATIBLE("pRng_ctx has the correct type (mcuxClRandom_Context_t), the cast was valid.")
                                        pRng_ctx,
        MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_POINTER_INCOMPATIBLE()
                                        mcuxClRandomModes_Mode_CtrDrbg_AES256_DRG3
                                   ));

    if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandom_init) != init_token) || (MCUXCLRANDOM_STATUS_OK != ri_status))
    {
      return MCUXCLEXAMPLE_STATUS_ERROR;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    /**************************************************************************/
    /* Generate several random byte strings and reseed the DRBG in between.   */
    /**************************************************************************/

    /* Buffers to store the generated random values in. */
    ALIGNED uint8_t drbg_data1[3u];
    MCUXCLBUFFER_INIT(drbgBuf1, NULL, &drbg_data1[0], 3u);
    ALIGNED uint8_t drbg_data2[16u];
    MCUXCLBUFFER_INIT(drbgBuf2, NULL, &drbg_data2[0], 16u);
    ALIGNED uint8_t drbg_data3[31u];
    MCUXCLBUFFER_INIT(drbgBuf3, NULL, &drbg_data3[0], 31u);


    /* Generate random values of smaller amount than one word size. */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(rg1_status, generate1_token, mcuxClRandom_generate(
                                        session,
                                        drbgBuf1,
                                        sizeof(drbg_data1)));

    if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandom_generate) != generate1_token) || (MCUXCLRANDOM_STATUS_OK != rg1_status))
    {
      return MCUXCLEXAMPLE_STATUS_ERROR;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    /* Generate random values of multiple of word size. */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(rg2_status, generate2_token, mcuxClRandom_generate(
                                        session,
                                        drbgBuf2,
                                        sizeof(drbg_data2)));

    if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandom_generate) != generate2_token) || (MCUXCLRANDOM_STATUS_OK != rg2_status))
    {
      return MCUXCLEXAMPLE_STATUS_ERROR;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    /* Reseed */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(rr_status, reseed_token, mcuxClRandom_reseed(session));

    if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandom_reseed) != reseed_token) || (MCUXCLRANDOM_STATUS_OK != rr_status))
    {
      return MCUXCLEXAMPLE_STATUS_ERROR;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    /* Generate random values of larger amount than but not multiple of one word size. */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(rg3_status, generate3_token, mcuxClRandom_generate(
                                        session,
                                        drbgBuf3,
                                        sizeof(drbg_data3)));

    if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandom_generate) != generate3_token) || (MCUXCLRANDOM_STATUS_OK != rg3_status))
    {
      return MCUXCLEXAMPLE_STATUS_ERROR;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    /**************************************************************************/
    /* Cleanup                                                                */
    /**************************************************************************/

    /* Random uninit. */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(ru_status, uninit_token, mcuxClRandom_uninit(session));

    if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandom_uninit) != uninit_token) || (MCUXCLRANDOM_STATUS_OK != ru_status))
    {
      return MCUXCLEXAMPLE_STATUS_ERROR;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    /** Destroy Session and cleanup Session **/
    if(!mcuxClExample_Session_Clean(session))
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    /** Disable the ELS **/
    if(!mcuxClExample_Els_Disable())
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    return MCUXCLEXAMPLE_STATUS_OK;
}
