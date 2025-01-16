/*--------------------------------------------------------------------------*/
/* Copyright 2021, 2023-2024 NXP                                            */
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
 * @file  mcuxClRsa_Internal_Macros.h
 * @brief Internal macros of the mcuxClRsa component
 */

#ifndef MCUXCLRSA_INTERNAL_MACROS_H_
#define MCUXCLRSA_INTERNAL_MACROS_H_

#include <mcuxClConfig.h> // Exported features flags header
#include <stdint.h>
#include <mcuxClCore_Macros.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup MCUXCLRSA_INTERNAL_DEFINES_ MCUXCLRSA_INTERNAL_DEFINES_
 * @brief Internal macros of the mcuxClRsa component.
 * @ingroup mcuxClRsa_Internal_Defines
 * @{
 */
#define MCUXCLRSA_PSS_PADDING1_LEN (8u)
    ///< Define for the PSS padding1 length.

#define MCUXCLRSA_HASH_MAX_SIZE (64u)
    ///< Defines the maximum sizes of the hash algorithms

#define MCUXCLRSA_MAX_MODLEN (512u)
/** @} */


/**
 * @defgroup MCUXCLRSA_INTERNAL_MACROS_ MCUXCLRSA_INTERNAL_MACROS_
 * @brief Internal macro of the mcuxClRsa component.
 * @ingroup mcuxClRsa_Internal_Macros
 * @{
 */

#define MCUXCLRSA_GET_MINIMUM_SECURITY_STRENGTH(keyBitLength) \
        (((keyBitLength) <= 2048u) ? 112u : \
        (((keyBitLength) <= 3072u) ? 128u : \
                                     152u))   ///< Macro to determine the minimal security strength that
                                              ///< needs to be provided by the RNG for RSA keys with
                                              ///< lengths from 2048 bits to 4096 bits.
                                              ///< Numbers taken from NIST SP 800-56B REV. 2, Table 2

/** @} */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MCUXCLRSA_INTERNAL_MACROS_H_ */

