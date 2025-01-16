/*--------------------------------------------------------------------------*/
/* Copyright 2020-2023 NXP                                                  */
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
 * @file  mcuxClMath_QDash_FUP.c
 * @brief mcuxClMath: FUP programs used in mcuxClMath_QDash
 */


#include <internal/mcuxClPkc_FupMacros.h>
#include <internal/mcuxClMath_QDash_FUP.h>
#include <internal/mcuxClMath_Internal_QDash.h>


/* ps1Len = (          -, operandSize) */
MCUX_CSSL_ANALYSIS_START_PATTERN_FUP_PROGRAM()
const mcuxClPkc_FUPEntry_t mcuxClMath_QDash_Fup_Init[3] MCUX_FUP_ATTRIBUTE = {{0x10u,0x00u,0x21u,0xccu,0x17u,0x06u},{0x00u,0x09u,0x01u,0x01u,0x01u,0x00u},{0x80u,0x21u,0x02u,0x00u,0x00u,0x03u}};
MCUX_CSSL_ANALYSIS_STOP_PATTERN_FUP_PROGRAM()



/**
 * [DESIGN] Square only
 * CAUTION:
 * According to PKC specification, when calculating MM (Modular Multiplication)
 * with OPLEN = MCUXCLPKC_WORDSIZE, PKC will read the least significant PKC word
 * of the result buffer in PKC workarea (T[0] in this FUP program) before writing
 * any intermediate result to it. This behavior will not affect the correctness,
 * but the PKC word T[0] needs to be initialized before this FUP program,
 * if the platform requires explicit memory initialization.
 *
 * ps, T[0] has been initialized when calling the FUP program above (QDash_Fup_init).
 */
/* ps1Len = (operandSize, operandSize) */
MCUX_CSSL_ANALYSIS_START_PATTERN_FUP_PROGRAM()
const mcuxClPkc_FUPEntry_t mcuxClMath_QDash_Fup_Loop0[3] MCUX_FUP_ATTRIBUTE = {{0x10u,0x00u,0xddu,0x52u,0x99u,0x5du},{0x80u,0x00u,0x03u,0x03u,0x01u,0x00u},{0x00u,0x1eu,0x00u,0x00u,0x04u,0x03u}};
MCUX_CSSL_ANALYSIS_STOP_PATTERN_FUP_PROGRAM()



/**
 * [DESIGN] Square and multiply
 * CAUTION: see the above FUP program, mcuxClMath_QDash_Fup_Loop0.
 */
/* ps1Len = (operandSize, operandSize) */
MCUX_CSSL_ANALYSIS_START_PATTERN_FUP_PROGRAM()
const mcuxClPkc_FUPEntry_t mcuxClMath_QDash_Fup_Loop1[3] MCUX_FUP_ATTRIBUTE = {{0x10u,0x00u,0xa1u,0xc5u,0x15u,0x04u},{0x80u,0x00u,0x03u,0x03u,0x01u,0x00u},{0x80u,0x21u,0x02u,0x00u,0x00u,0x03u}};
MCUX_CSSL_ANALYSIS_STOP_PATTERN_FUP_PROGRAM()

