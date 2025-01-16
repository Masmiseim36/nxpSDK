/*--------------------------------------------------------------------------*/
/* Copyright 2023 NXP                                                       */
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

#ifndef MCUXCLCIPHERMODES_COMMON_HELPER_H_
#define MCUXCLCIPHERMODES_COMMON_HELPER_H_

#include <stdint.h>

#include <mcuxClCore_Macros.h>
#include <mcuxCsslFlowProtection.h>

/*
 * Helper functions
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClCipherModes_MemClear)
void mcuxClCipherModes_MemClear(uint8_t *pDst, uint32_t length);

#endif /* MCUXCLCIPHERMODES_COMMON_HELPER_H_ */
