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
 * @file  mcuxClEcc_TwEd_Internal_PointArithmetic_FUP.c
 * @brief mcuxClEcc: FUP programs used in ECC functions for Twisted Edwards curves
 */

#include <mcuxCsslAnalysis.h>

#include <internal/mcuxClPkc_FupMacros.h>
#include <internal/mcuxClEcc_TwEd_Internal_PkcWaLayout.h>
#include <internal/mcuxClEcc_TwEd_Internal_FUP.h>

MCUX_CSSL_ANALYSIS_START_PATTERN_HYPERLINK_IN_COMMENTS()
/* FUP program to perform point doubling on a twisted Edwards curve in      */
/* homogeneous coordinates following https://eprint.iacr.org/2008/013.pdf.  */
/*                                                                          */
/* Prerequisites:                                                           */
/* - Buffers TWED_X, TWED_Y, TWED_Z contain the homogeneous coordinates     */
/*   (X:Y:Z) of P in MR                                                     */
/* - ps1Len = (operandSize, operandSize)                                    */
/* - Buffer ECC_PFULL contains p'||p                                        */
/* - Buffer ECC_PS contains the shifted modulus associated to p             */
/*                                                                          */
/* Result:                                                                  */
/* - The resulting coordinates (Xres:Yres:Zres) are stored in buffers       */
/*   TWED_X, TWED_Y and TWED_Z in MR.                                       */
MCUX_CSSL_ANALYSIS_STOP_PATTERN_HYPERLINK_IN_COMMENTS()
MCUX_CSSL_ANALYSIS_START_PATTERN_FUP_PROGRAM()
const mcuxClPkc_FUPEntry_t mcuxClEcc_FUP_TwEd_PointDouble[16] MCUX_FUP_ATTRIBUTE = {{0x10u,0x00u,0xdeu,0x93u,0xadu,0xa3u},{0x80u,0x21u,0x00u,0x20u,0x21u,0x1bu},{0x80u,0x00u,0x1bu,0x1bu,0x00u,0x19u},{0x80u,0x00u,0x20u,0x20u,0x00u,0x1bu},{0x80u,0x00u,0x21u,0x21u,0x00u,0x1du},{0x80u,0x00u,0x12u,0x1bu,0x00u,0x1fu},{0x80u,0x21u,0x10u,0x1fu,0x1du,0x20u},{0x80u,0x2au,0x10u,0x1fu,0x1du,0x1fu},{0x80u,0x00u,0x20u,0x1fu,0x00u,0x21u},{0x80u,0x00u,0x22u,0x22u,0x00u,0x1fu},{0x80u,0x21u,0x10u,0x1fu,0x1fu,0x1fu},{0x80u,0x2au,0x10u,0x20u,0x1fu,0x1fu},{0x80u,0x00u,0x20u,0x1fu,0x00u,0x22u},{0x80u,0x2au,0x10u,0x19u,0x1bu,0x19u},{0x80u,0x2au,0x10u,0x19u,0x1du,0x19u},{0x80u,0x00u,0x19u,0x1fu,0x00u,0x20u}};
MCUX_CSSL_ANALYSIS_STOP_PATTERN_FUP_PROGRAM()

