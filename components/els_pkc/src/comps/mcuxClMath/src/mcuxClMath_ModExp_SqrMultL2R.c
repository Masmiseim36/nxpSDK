/*--------------------------------------------------------------------------*/
/* Copyright 2020-2024 NXP                                                  */
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
 * @file  mcuxClMath_ModExp_SqrMultL2R.c
 * @brief mcuxClMath: modular exponentiation, left-to-right binary square and multiply
 */


#include <platform_specific_headers.h>
#include <mcuxClCore_Platform.h>
#include <mcuxClCore_FunctionIdentifiers.h>
#include <mcuxCsslFlowProtection.h>
#include <mcuxCsslAnalysis.h>

#include <mcuxClPkc.h>
#include <mcuxClMath_Functions.h>
#include <mcuxClMath_Types.h>

#include <internal/mcuxClPkc_Macros.h>
#include <internal/mcuxClPkc_Operations.h>
#include <internal/mcuxClMath_Internal_Utils.h>


/**
 * [DESIGN]
 * This function calculates exponentiation x^exp mod n by left-to-right binary
 * square-and-multiply algorithm. It scans the exponent from most to least
 * significant bit. If a bit is 0, this algorithm performs a modular squaring;
 * and if a bit is 1, it performs a modular squaring followed by a modular multiplication.
 *
 * Since PKC does not support in-place modular multiplication (squaring), this
 * algorithm needs 2 operands T0 and T1. The computation is controlled by 2-bit "state".
 * The indices of operands and result is stored in an array args[], indexed by "state".
 * When a bit of exponent is 1, the loop of this algorithm will scan that bit twice:
 * performs a modular squaring and a modular multiplication, i.e., state =
 * 0b01 (square) --> 0b10 (multiply), or 0b11 (square) --> 0b00 (multiply).
 * When a bit is 0, only a modular squaring is performed (state = 0b01 or 0b11),
 * and then the loop moves to the next bit.
 */
MCUX_CSSL_ANALYSIS_START_SUPPRESS_DECLARED_BUT_NEVER_DEFINED("It is indeed defined.")
MCUX_CSSL_ANALYSIS_START_PATTERN_SYMBOL_DEFINED_MORE_THAN_ONCE()
MCUX_CSSL_FP_FUNCTION_DEF(mcuxClMath_ModExp_SqrMultL2R)
MCUX_CSSL_FP_PROTECTED_TYPE(void) mcuxClMath_ModExp_SqrMultL2R(const uint8_t *pExp, uint32_t expByteLength, uint32_t iR_iX_iN_iT)
MCUX_CSSL_ANALYSIS_STOP_PATTERN_SYMBOL_DEFINED_MORE_THAN_ONCE()
MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_DECLARED_BUT_NEVER_DEFINED()
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClMath_ModExp_SqrMultL2R);

    /* ASSERT: exponent length is reasonable (exponent fits in PKC workarea). */
    MCUX_CSSL_ANALYSIS_ASSERT_PARAMETER_FP_VOID(expByteLength, 1u, MCUXCLPKC_RAM_SIZE)

    MCUXCLPKC_PKC_CPU_ARBITRATION_WORKAROUND();  // avoid CPU accessing to PKC workarea when PKC is busy

    /* Count Hamming weight of exponent. */
    MCUX_CSSL_FP_COUNTER_STMT(uint32_t hwExp = 0u;)
    uint32_t byteIndex_MSNZByte = 0u;
    uint32_t idx = expByteLength;

    do
    {
        idx -= 1u;
        uint32_t expByte = (uint32_t) pExp[idx];

        /* Find the most significant nonzero byte (= the last nonzero byte when scanning from LSByte to MSByte). */
        if (0u != expByte)
        {
            byteIndex_MSNZByte = (uint32_t) idx;
        }

        /* Count Hamming weight of a byte, shall in in the range [0,8]. */
        MCUX_CSSL_FP_COUNTER_STMT(
            expByte = expByte - ((expByte >> 1u) & 0x55u);  \
            expByte = (expByte & 0x33u) + ((expByte & 0xCCu) >> 2u);  \
            expByte = (expByte & 0x07u) + ((expByte & 0x70u) >> 4u);  \
            MCUX_CSSL_ANALYSIS_ASSERT_PARAMETER_FP_VOID(expByte, 0u, 8u)  \
            MCUX_CSSL_ANALYSIS_START_SUPPRESS_INTEGER_WRAP("hwExp will not exceed 8*expByteLength.") \
            hwExp += expByte;
            MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_INTEGER_WRAP() )
    } while (0u < idx);

    const uint32_t expMSByte = (uint32_t) pExp[byteIndex_MSNZByte];  /* nonzero byte */
    const uint32_t leadingZeros = mcuxClMath_CountLeadingZerosWord(expMSByte);
    MCUX_CSSL_ANALYSIS_ASSERT_PARAMETER_FP_VOID(leadingZeros, 24u, 31u)

    /* Calculate bit index of most significant nonzero bit. */
    const uint32_t bitIndex_MSBit = (8u * (expByteLength - byteIndex_MSNZByte)) - (leadingZeros - 24u) - 1u;
    int32_t bitIndex = (int32_t) bitIndex_MSBit - 1;  /* Skip the most significant nonzero bit. */

    /* Calculate the number of loop iteration for FP. */
    MCUX_CSSL_FP_COUNTER_STMT(
        MCUX_CSSL_ANALYSIS_ASSERT_PARAMETER_FP_VOID(hwExp, 1u, 8u * expByteLength)  /* exponent is nonzero. */ \
        const uint32_t loopIteration = bitIndex_MSBit + (hwExp - 1u); )

    const uint32_t iR = (iR_iX_iN_iT >> 24) & 0xFFu;  /* Also used as T1. */
    const uint32_t iX = (iR_iX_iN_iT >> 16) & 0xFFu;
    const uint32_t iN = (iR_iX_iN_iT >>  8) & 0xFFu;
    const uint32_t iT = iR_iX_iN_iT & 0xFFu;          /* T0 */

    MCUXCLPKC_FP_CALC_OP1_OR_CONST(iR, iX, 0u);  /* Copy X to R (T1). */

    uint32_t args[4];
    args[0] = MCUXCLPKC_PACKARGS4(iT,iR,iX,iN);  /* state=0b00, T0 = T1 * X  mod N (multiply) */
    args[1] = MCUXCLPKC_PACKARGS4(iT,iR,iR,iN);  /* state=0b01, T0 = T1 * T1 mod N (square)   */
    args[2] = MCUXCLPKC_PACKARGS4(iR,iT,iX,iN);  /* state=0b10, T1 = T0 * X  mod N (multiply) */
    args[3] = MCUXCLPKC_PACKARGS4(iR,iT,iT,iN);  /* state=0b11, T1 = T0 * T0 mod N (square)   */

    uint32_t state = 1u;
    uint32_t shift = 1u;  /* shift to next bit (=1) or not (=0) */

    MCUX_CSSL_FP_LOOP_DECL(SquMulLoop);
    while(bitIndex >= 0)
    {
        MCUX_CSSL_FP_LOOP_ITERATION(SquMulLoop,
            MCUXCLPKC_FP_CALLED_CALC_MC1_MM);

        MCUX_CSSL_FP_FUNCTION_CALL_VOID(mcuxClPkc_Calc(MCUXCLPKC_PARAMMODE_MC1(MM), args[state]));

        /* Swap the two temp operands. */
        state = 3u ^ state;
        MCUXCLPKC_PKC_CPU_ARBITRATION_WORKAROUND();  // avoid CPU accessing to PKC workarea when PKC is busy

        /* Calculate byte index of exponent in BE. */
        const uint32_t byteIndex = expByteLength - 1u - ((uint32_t) bitIndex / 8u);
        const uint32_t expByte = (uint32_t) pExp[byteIndex];
        const uint32_t expBit = (expByte >> ((uint32_t) bitIndex & 7u)) & 1u;

        /* If expBit = 1, the while-loop will stay in this bit for one more iteration. */
        shift ^= expBit;
        /* Always squaring in next iteration if shifting to a new bit (state = 0b01 or 0b11). */
        state |= shift;

        bitIndex -= (int32_t) shift;
    }

    /* The result is in T1 (R) if state.bit1 = 0; */
    /*                  T0 (T) if state.bit1 = 1. */
    if (0u != (state >> 1u))
    {
        MCUXCLPKC_FP_CALC_OP1_OR_CONST(iR, iT, 0u);  /* Copy result to R. */
    }

    MCUX_CSSL_FP_FUNCTION_EXIT_VOID(mcuxClMath_ModExp_SqrMultL2R,
        MCUXCLPKC_FP_CALLED_CALC_OP1_OR_CONST,
        MCUX_CSSL_FP_LOOP_ITERATIONS(SquMulLoop, loopIteration),
        MCUX_CSSL_FP_CONDITIONAL((1u == (loopIteration & 0x01u)), MCUXCLPKC_FP_CALLED_CALC_OP1_OR_CONST) );
}
