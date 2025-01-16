/*--------------------------------------------------------------------------*/
/* Copyright 2016, 2021, 2023-2024 NXP                                      */
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
 *
 * @file:  mcuxClOscca_Memory.h
 * @brief: Macros for alignment memory
 *
 */

#ifndef MCUXCLOSCCA_MEMORY_H_
#define MCUXCLOSCCA_MEMORY_H_

#include <mcuxClConfig.h> // Exported features flags header
#include <mcuxCsslFlowProtection.h>
#include <mcuxClOscca_FunctionIdentifiers.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MCUXCLOSCCA_SIZE_ALIGN_OFFSET  (sizeof(uint32_t) - 1U)

#define mcuxClOscca_alignAddress(address) \
  MCUX_CSSL_ANALYSIS_START_SUPPRESS_TYPECAST_BETWEEN_INTEGER_AND_POINTER("The resulting address is carefully calculated, the cast to a pointer-type is safe.") \
  ((uint8_t *)( (((uint32_t)(address)) + (sizeof(uint32_t) - 1U)) \
      & ((uint32_t)(~(sizeof(uint32_t) - 1U))) )) \
  MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_TYPECAST_BETWEEN_INTEGER_AND_POINTER()

#define mcuxClOscca_alignSize(size) \
    ((uint32_t)(((uint32_t)(size)) + (sizeof(uint32_t) - 1U)) \
        & ((uint32_t)(~(sizeof(uint32_t) - 1U))) )

#define mcuxClOscca_alignAddressWithOffset(address, offset) \
  MCUX_CSSL_ANALYSIS_START_SUPPRESS_TYPECAST_BETWEEN_INTEGER_AND_POINTER("The resulting address is carefully calculated, the cast to a pointer-type is safe.") \
  ((uint8_t *)( (((uint32_t)(address) + (uint32_t)(offset)) + (sizeof(uint32_t) - 1U)) \
      & ((uint32_t)(~(sizeof(uint32_t) - 1U))) )) \
  MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_TYPECAST_BETWEEN_INTEGER_AND_POINTER()

#define mcuxClOscca_alignAddressToBoundary(address, boundary) \
  ((uint8_t *)( (((uint32_t)(address)) + (boundary - 1U)) \
      & ((uint32_t)(~(boundary - 1U))) ))

MCUX_CSSL_FP_FUNCTION_DECL(mcuxClOscca_FastSecureXor)
MCUX_CSSL_FP_PROTECTED_TYPE(void) mcuxClOscca_FastSecureXor(void *pTgt,
        void *pSrc1,
        void *pSrc2,
        uint32_t length);

MCUX_CSSL_FP_FUNCTION_DECL(mcuxClOscca_switch_endianness)
MCUX_CSSL_FP_PROTECTED_TYPE(void) mcuxClOscca_switch_endianness(uint32_t *ptr, uint32_t length);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MCUXCLOSCCA_MEMORY_H_ */
