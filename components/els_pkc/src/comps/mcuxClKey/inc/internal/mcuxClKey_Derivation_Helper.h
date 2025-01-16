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
 * @file  mcuxClKey_Derivation_Helper.h
 * @brief Internal helper constants and functions for the KDF algorithms
 */

#ifndef MCUXCLKEY_DERIVATION_HELPER_H_
#define MCUXCLKEY_DERIVATION_HELPER_H_

#include <mcuxClConfig.h> // Exported features flags header
#include <internal/mcuxClKey_Types_Internal.h>

#ifdef __cplusplus
extern "C" {
#endif


/* Inline functions for proper type casts */

/**
 * @brief Cast a pointer to word-aligned data to a pointer to the mcuxClKeyDerivation_WorkArea_t type.
 *
 * @param pWa    The pointer to cast to a proper Wa type. Must be aligned. Must point to an area of enough size.
 */
MCUX_CSSL_FP_FUNCTION_DEF(mcuxClKey_castToKeyDerivationWorkarea)
static inline mcuxClKeyDerivation_WorkArea_t* mcuxClKey_castToKeyDerivationWorkarea(uint32_t* pWa)
{
  MCUX_CSSL_ANALYSIS_START_PATTERN_REINTERPRET_MEMORY_OF_OPAQUE_TYPES()
  return (mcuxClKeyDerivation_WorkArea_t*) pWa;
  MCUX_CSSL_ANALYSIS_STOP_PATTERN_REINTERPRET_MEMORY_OF_OPAQUE_TYPES()
}

/**
 * @brief Cast a pointer to word-aligned data to a pointer to the mcuxClKey_DerivationMode_t type.
 *
 * @param pMode   The pointer to cast to a proper derivationMode type. Must be aligned.
 *                Must point to an area of enough size, see @ref MCUXCLKEY_DERIVATION_MODE_DESCRIPTOR_SIZE_IN_WORDS.
 */
MCUX_CSSL_FP_FUNCTION_DEF(mcuxClKey_castToKeyDerivationMode)
static inline mcuxClKey_DerivationMode_t* mcuxClKey_castToKeyDerivationMode(uint32_t* pMode)
{
  MCUX_CSSL_ANALYSIS_START_PATTERN_REINTERPRET_MEMORY_OF_OPAQUE_TYPES()
  return (mcuxClKey_DerivationMode_t*) pMode;
  MCUX_CSSL_ANALYSIS_STOP_PATTERN_REINTERPRET_MEMORY_OF_OPAQUE_TYPES()
}


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MCUXCLKEY_DERIVATION_HELPER_H_ */