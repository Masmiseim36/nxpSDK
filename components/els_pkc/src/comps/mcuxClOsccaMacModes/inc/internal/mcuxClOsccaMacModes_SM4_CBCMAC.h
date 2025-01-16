/*--------------------------------------------------------------------------*/
/* Copyright 2022-2023 NXP                                                  */
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

#ifndef MCUXCLOSCCAMACMODES_SM4_CBCMAC_H_
#define MCUXCLOSCCAMACMODES_SM4_CBCMAC_H_

#include <mcuxClConfig.h> // Exported features flags header
#include <mcuxClSession_Types.h>
#include <mcuxClMac_Types.h>
#include <mcuxClBuffer.h>
#include <internal/mcuxClOsccaMacModes_Ctx.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Engine functions
 */

MCUX_CSSL_FP_FUNCTION_DECL(mcuxClOsccaMacModes_Engine_CBCMAC_Oneshot, mcuxClOsccaMac_ComputeEngine_t)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClMac_Status_t) mcuxClOsccaMacModes_Engine_CBCMAC_Oneshot(
    mcuxClSession_Handle_t session,                          /*! CBC-MAC session handle */
    mcuxClMac_Mode_t mode,
    mcuxClKey_Handle_t key,
    mcuxCl_InputBuffer_t pIn,                                /*! CBC-MAC input */
    uint32_t inLength,                                      /*! Input size */
    mcuxCl_Buffer_t pOut,                                    /*! CBC-MAC output */
    uint32_t *const pOutLength                              /*! Output size */
);


MCUX_CSSL_FP_FUNCTION_DECL(mcuxClOsccaMacModes_Engine_CBCMAC_Init, mcuxClOsccaMac_InitEngine_t)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClMac_Status_t) mcuxClOsccaMacModes_Engine_CBCMAC_Init(
    mcuxClSession_Handle_t session,                          /*! CBC-MAC session handle */
    mcuxClOsccaMacModes_Context_t * const pContext           /*! CBC-MAC context */
);

MCUX_CSSL_FP_FUNCTION_DECL(mcuxClOsccaMacModes_Engine_CBCMAC_Update, mcuxClOsccaMac_UpdateEngine_t)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClMac_Status_t) mcuxClOsccaMacModes_Engine_CBCMAC_Update(
    mcuxClSession_Handle_t session,                          /*! CBC-MAC session handle */
    mcuxClOsccaMacModes_Context_t * const pContext,          /*! CBC-MAC context */
    mcuxCl_InputBuffer_t pIn,                                /*! CBC-MAC input */
    uint32_t inLength                                       /*! Input size */
);

MCUX_CSSL_FP_FUNCTION_DECL(mcuxClOsccaMacModes_Engine_CBCMAC_Finalize, mcuxClOsccaMac_FinishEngine_t)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClMac_Status_t) mcuxClOsccaMacModes_Engine_CBCMAC_Finalize(
    mcuxClSession_Handle_t session,                          /*! CBC-MAC session handle */
    mcuxClOsccaMacModes_Context_t * const pContext,          /*! CBC-MAC context */
    mcuxCl_Buffer_t pOut,                                    /*! CBC-MAC output */
    uint32_t *const pOutLength                              /*! Output size */
);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MCUXCLOSCCAMACMODES_SM4_CBCMAC_H_ */
