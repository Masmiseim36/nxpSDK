/*--------------------------------------------------------------------------*/
/* Copyright 2018-2024 NXP                                                  */
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

/** @file  mcuxClOsccaSm2_Decrypt.c
 *  @brief mcuxClOsccaSm2: implementation of SM2 Decrypt function
 */

#include <mcuxClMemory.h>
#include <mcuxClHash.h>
#include <mcuxClSession.h>
#include <mcuxClOsccaSm2.h>
#include <mcuxClOsccaSm3.h>
#include <mcuxClOsccaPkc.h>
#include <mcuxCsslFlowProtection.h>
#include <mcuxCsslSecureCounter.h>
#include <mcuxClOscca_FunctionIdentifiers.h>
#include <internal/mcuxClOsccaSm2_Internal.h>
#include <internal/mcuxClOsccaSm2_Internal_Hash.h>
#include <internal/mcuxClSession_Internal.h>
#include <internal/mcuxClOsccaPkc_Macros.h>
#include <internal/mcuxClOsccaPkc_Operations.h>
#include <internal/mcuxClOsccaPkc_FupMacros.h>

MCUX_CSSL_FP_FUNCTION_DEF(mcuxClOsccaSm2_Decrypt, mcuxClOsccaSm2_DecEngine_t)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClOsccaSm2_Status_t) mcuxClOsccaSm2_Decrypt(mcuxClSession_Handle_t session, mcuxClOsccaSm2_Decrypt_Param_t *pParams)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClOsccaSm2_Decrypt);

    MCUX_CSSL_ANALYSIS_START_PATTERN_CAST_TO_MORE_SPECIFIC_ALIGNED_TYPE()
    mcuxClOsccaSm2_Internal_EncDecCtx_t* pCtx = (mcuxClOsccaSm2_Internal_EncDecCtx_t*)pParams->pEncDecCtx;
    MCUX_CSSL_ANALYSIS_STOP_PATTERN_CAST_TO_MORE_SPECIFIC_ALIGNED_TYPE()

    /******************************************************************/
    /* Check inputs and initialize                                    */
    /******************************************************************/

    /*Check the correctness of the state option: Options 0 and "INIT | FINALIZE" are not supported.*/
    uint16_t stateOption = (pParams->options & MCUXCLOSCCASM2_OPT_ENC_DEC_STATE_MASK);
    if ((0u == stateOption)  || ((MCUXCLOSCCASM2_OPT_ENC_DEC_STATE_INIT | MCUXCLOSCCASM2_OPT_ENC_DEC_STATE_FINALIZE) == stateOption))
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClOsccaSm2_Decrypt, MCUXCLOSCCASM2_STATUS_FAULT_ATTACK);
    }

    MCUXCLOSCCASM2_FP_REQUEST_PKC(session, mcuxClOsccaSm2_Decrypt);

    if (MCUXCLOSCCASM2_OPT_ENC_DEC_STATE_INIT == (pParams->options & MCUXCLOSCCASM2_OPT_ENC_DEC_STATE_INIT))
    {
        MCUX_CSSL_FP_FUNCTION_CALL(decInitRet,  mcuxClOsccaSm2_Decrypt_Internal_Init(session, pParams));
        if (MCUXCLOSCCASM2_STATUS_DECRYPT_OK != decInitRet)
        {
            MCUXCLOSCCASM2_FP_RELEASE_PKC(session, mcuxClOsccaSm2_Decrypt);
            MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClOsccaSm2_Decrypt, decInitRet,
                                                      MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClOsccaSm2_Decrypt_Internal_Init),
                                                      MCUXCLOSCCASM2_FP_CALLED_REQUEST_PKC,
                                                      MCUXCLOSCCASM2_FP_CALLED_RELEASE_PKC);
        }
    }
    else
    {
        /*Crc check over the EncDecCtx has to be done after each call excluding INIT*/
        MCUX_CSSL_FP_FUNCTION_CALL(DecRet2, mcuxClOsccaSm2_ValidateEncDecCtx(
                MCUX_CSSL_ANALYSIS_START_SUPPRESS_POINTER_INCOMPATIBLE("Reinterpret the external parameter to internal context structure.")
                pCtx,
                MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_POINTER_INCOMPATIBLE()
                (uint32_t)pParams->domainParameters.p.wNumBytes));
        if (MCUXCLOSCCASM2_STATUS_VALIDATE_ENCDEC_CTX_OK != DecRet2)
        {
            MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClOsccaSm2_Decrypt, MCUXCLOSCCASM2_STATUS_FAULT_ATTACK);
        }
    }

    if (MCUXCLOSCCASM2_OPT_ENC_DEC_STATE_UPDATE == (pParams->options & MCUXCLOSCCASM2_OPT_ENC_DEC_STATE_UPDATE))
    {

        /*Perform whole Update phase - functionality extracted as similiarity between encrypt and decrypt functions exist*/

        MCUX_CSSL_FP_FUNCTION_CALL(DecRet3, mcuxClOsccaSm2_EncDec_UpdatePhase(
                session,
        MCUX_CSSL_ANALYSIS_START_SUPPRESS_REINTERPRET_MEMORY("reinterpret the API strcture type to internal structure type.")
                (mcuxClOsccaSm2_EncDec_Common_Param_t *)pParams,
        MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_REINTERPRET_MEMORY()
                MCUXCLOSCCASM2_DECRYPT));
        if (MCUXCLOSCCASM2_STATUS_ENCDEC_PHASE_OK != DecRet3)
        {
            MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClOsccaSm2_Decrypt, MCUXCLOSCCASM2_STATUS_FAULT_ATTACK);
        }

    }

    /**********************************************************************/
    /* If option FINALIZE is chosen, perform the following steps:         */
    /**********************************************************************/
    MCUX_CSSL_FP_FUNCTION_CALL(decfinalRet,  mcuxClOsccaSm2_Decrypt_Internal_Final(session, pParams));

    MCUXCLOSCCASM2_FP_RELEASE_PKC(session, mcuxClOsccaSm2_Decrypt);

    /* Balance security counter */
     MCUX_CSSL_FP_FUNCTION_EXIT_WITH_CHECK(mcuxClOsccaSm2_Decrypt, decfinalRet, MCUXCLOSCCASM2_STATUS_FAULT_ATTACK,
                                                      MCUXCLOSCCASM2_OPT_ENC_DEC_STATE_INIT == (pParams->options & MCUXCLOSCCASM2_OPT_ENC_DEC_STATE_INIT) ?
                    MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClOsccaSm2_Decrypt_Internal_Init) : MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClOsccaSm2_ValidateEncDecCtx),
                                                      MCUXCLOSCCASM2_OPT_ENC_DEC_STATE_UPDATE == (pParams->options & MCUXCLOSCCASM2_OPT_ENC_DEC_STATE_UPDATE) ?
                    MCUXCLOSCCASM2_SECURITYCOUNTER_VAL_ENCDEC_BALANCE_UPDATE_PART : 0u,
                                                      MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClOsccaSm2_Decrypt_Internal_Final),
                                                      MCUXCLOSCCASM2_FP_CALLED_REQUEST_PKC,
                                                      MCUXCLOSCCASM2_FP_CALLED_RELEASE_PKC);

}
