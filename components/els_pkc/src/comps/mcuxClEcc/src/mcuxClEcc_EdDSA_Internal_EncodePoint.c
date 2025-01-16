/*--------------------------------------------------------------------------*/
/* Copyright 2024 NXP                                                       */
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
 * @file  mcuxClEcc_EdDSA_Internal_EncodePoint.c
 * @brief Function to encode a point
 */


#include <mcuxCsslFlowProtection.h>
#include <mcuxClCore_FunctionIdentifiers.h>
#include <mcuxClEcc.h>

#include <internal/mcuxClPkc_Macros.h>
#include <internal/mcuxClPkc_Operations.h>
#include <internal/mcuxClEcc_EdDSA_Internal.h>


/**
 * @brief Perform EdDSA encoding of a twisted Edwards curve point
 *
 * @param[in] encodedLen Length of encoded point
 *
 * @pre
 *  - ECC_COORD00 and ECC_COORD01 must contain the affine coordinates of the point to encode
 *  - @p encodedLen must be the length of the buffer at ECC_COORD02
 *  - PS2 (OP)LEN must be set to encodedLenPkc
 *
 * @post
 *  - buffer at ECC_COORD02 contains the encoded data
 *
*/
MCUX_CSSL_FP_FUNCTION_DEF(mcuxClEcc_EdDSA_EncodePoint)
MCUX_CSSL_FP_PROTECTED_TYPE(void) mcuxClEcc_EdDSA_EncodePoint(
    const uint32_t encodedLen
)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClEcc_EdDSA_EncodePoint);

    uint16_t *pOperands = MCUXCLPKC_GETUPTRT();
    MCUXCLPKC_FP_CALC_OP2_CONST(ECC_COORD02, 0u);                   /* Clear encodedLenPkc bytes of result buffer */
    MCUXCLPKC_FP_CALC_OP1_OR_CONST(ECC_COORD02, ECC_COORD01, 0u);   /* Copy operandSize < encodedLenPkc bytes of the y-coordinate from ECC_COORD01 to ECC_COORD02 */
    uint32_t *pX = MCUXCLPKC_OFFSET2PTRWORD(pOperands[ECC_COORD00]);
    MCUX_CSSL_ANALYSIS_COVERITY_ASSERT_FP_VOID(encodedLen, 1u, MCUXCLPKC_RAM_SIZE)
    uint8_t *pEncLsbXByte = &MCUXCLPKC_OFFSET2PTR(pOperands[ECC_COORD02])[encodedLen - 1u];
    MCUXCLPKC_WAITFORFINISH();
    uint32_t lsbX = (*pX) & (uint32_t)0x01u;    /* Loading a word is usually cheaper than loading a byte */
    *pEncLsbXByte |= ((uint8_t)lsbX << 7u);

    MCUX_CSSL_FP_FUNCTION_EXIT_VOID(mcuxClEcc_EdDSA_EncodePoint);
}
