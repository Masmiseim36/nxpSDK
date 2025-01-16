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

/** @file  mcuxClMath_SecModExp_FUP.h
*  @brief defines FUP programs byte arrays
*/
#ifndef MCUXCLMATH_SECMODEXP_FUP_H_
#define MCUXCLMATH_SECMODEXP_FUP_H_
#include <internal/mcuxClPkc_FupMacros.h>

extern const mcuxClPkc_FUPEntry_t mcuxClMath_SecModExp_Fup_CalcQAndInterleave[6];
extern const mcuxClPkc_FUPEntry_t mcuxClMath_SecModExp_Fup_EuclideanSplit_1[8];
extern const mcuxClPkc_FUPEntry_t mcuxClMath_SecModExp_Fup_EuclideanSplit_2[7];
extern const mcuxClPkc_FUPEntry_t mcuxClMath_SecModExp_Fup_ExactDivideLoop[9];
extern const mcuxClPkc_FUPEntry_t mcuxClMath_SecModExp_Fup_Init[3];
extern const mcuxClPkc_FUPEntry_t mcuxClMath_SecModExp_Fup_PrepareFirstExp[5];
extern const mcuxClPkc_FUPEntry_t mcuxClMath_SecModExp_Fup_Rerandomize[5];

#endif /* MCUXCLMATH_SECMODEXP_FUP*/ 
