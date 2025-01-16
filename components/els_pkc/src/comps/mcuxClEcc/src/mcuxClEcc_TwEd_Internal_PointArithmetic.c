/*--------------------------------------------------------------------------*/
/* Copyright 2024 NXP                                                       */
/*                                                                          */
/* NXP Confidential and Proprietary. This software is owned or controlled   */
/* by NXP and may only be used strictly in accordance with the applicable   */
/* license terms.  By expressly accepting such terms or by downloading,     */
/* installing, activating and/or otherwise using the software, you are      */
/* agreeing that you have read, and that you agree to comply with and are   */
/* bound by, such license terms.  If you do not agree to be bound by the    */
/* applicable license terms, then you may not retain, install, activate or  */
/* otherwise use the software.                                              */
/*--------------------------------------------------------------------------*/

/**
 * @file  mcuxClEcc_TwEd_Internal_PointArithmetic.c
 * @brief Edwards curve internal point arithmetic
 */


#include <mcuxClCore_Platform.h>
#include <mcuxCsslFlowProtection.h>
#include <mcuxClCore_FunctionIdentifiers.h>

#include <mcuxClEcc.h>

#include <internal/mcuxClPkc_Operations.h>
#include <internal/mcuxClEcc_TwEd_Internal.h>
#include <internal/mcuxClEcc_TwEd_Internal_FUP.h>


MCUX_CSSL_ANALYSIS_START_PATTERN_HYPERLINK_IN_COMMENTS()
/**
 * This function implements a repeated doubling operation of a twisted Edwards curve point in projective coordinates 
 * P = (X:Y:Z) using the formulas provided in Section 6 of https://eprint.iacr.org/2008/013.pdf.
 *
 * Input:
 *  - noOfDoubles   Number of double operations to be performed
 *
 * Prerequisites:
 *  - Buffers TWED_X, TWED_Y and TWED_Z contain the homogeneous coordinates (X:Y:Z) of P in MR
 *  - ps1Len = (operandSize, operandSize)
 *  - Buffer ECC_PFULL contains p'||p
 *  - Buffer ECC_PS contains the shifted modulus associated to p
 *
 * Result:
 *  - The resulting coordinates are stored in buffers TWED_X, TWED_Y and TWED_Z in MR.
 *
 * @attention The PKC calculation might be still on-going, call #MCUXCLPKC_WAITFORFINISH before CPU accesses to the result.
 */
MCUX_CSSL_ANALYSIS_STOP_PATTERN_HYPERLINK_IN_COMMENTS()
MCUX_CSSL_FP_FUNCTION_DEF(mcuxClEcc_TwEd_RepeatedDoubling)
MCUX_CSSL_FP_PROTECTED_TYPE(void) mcuxClEcc_TwEd_RepeatedDoubling(uint16_t noOfDoubles)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClEcc_TwEd_RepeatedDoubling);

    for(uint32_t i = 0u; i < noOfDoubles; i++)
    {
        MCUXCLPKC_FP_CALCFUP(mcuxClEcc_FUP_TwEd_PointDouble, mcuxClEcc_FUP_TwEd_PointDouble_LEN);
    }

    MCUX_CSSL_FP_FUNCTION_EXIT_VOID(mcuxClEcc_TwEd_RepeatedDoubling,
        noOfDoubles * MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClPkc_CalcFup));
}