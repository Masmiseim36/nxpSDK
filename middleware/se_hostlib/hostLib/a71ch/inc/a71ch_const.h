/**
* @file a71ch_const.h
* @author NXP Semiconductors
* @version 1.0
* @par License
* Copyright 2016 NXP
*
* This software is owned or controlled by NXP and may only be used
* strictly in accordance with the applicable license terms.  By expressly
* accepting such terms or by downloading, installing, activating and/or
* otherwise using the software, you are agreeing that you have read, and
* that you agree to comply with and are bound by, such license terms.  If
* you do not agree to be bound by the applicable license terms, then you
* may not retain, install, activate or otherwise use the software.
*
* @par Description
* Host Library A71CH Constants.
* @par History
 *****************************************************************************/
#ifndef _A71CH_CONST_H_
#define _A71CH_CONST_H_

#ifdef __cplusplus
extern "C" {
#endif

#define APPLET_NAME "a71ch" // 0x61.37.31.63.68

#define A71CH_KEY_PAIR_MAX_A    2 //!< Maximum amount of ECC key pairs that can be stored in A71CH (A device)
#define A71CH_PUBLIC_KEY_MAX_A  2 //!< Maximum amount of ECC public keys that can be stored in A71CH (A device)
#define A71CH_SYM_KEY_MAX_A     4 //!< Maximum amount of Symmetric keys that can be stored in A71CH (A device)
#define A71CH_KEY_PAIR_MAX_B    4 //!< Maximum amount of ECC key pairs that can be stored in A71CH (B device)
#define A71CH_PUBLIC_KEY_MAX_B  3 //!< Maximum amount of ECC public keys that can be stored in A71CH (B device)
#define A71CH_SYM_KEY_MAX_B     8 //!< Maximum amount of Symmetric keys that can be stored in A71CH (A device)
#define A71CH_COUNTER_MAX       2 //!< Maximum amount of monotonic counters that can be stored in A71CH (A&B)

// We cover two A71CH product variants that differ in the amount of credentials that can be stored.
// These two variants are referred to (in this example source code) as
// - TYPE_A (or simply A) - which is the device with lesser storage
// - TYPE_B (or simply B) - which is the device with more storage
// As the example source code depends on the amount of credentials that can be stored
// the ::A71CH_DEVICE_TYPE define must be used to select between either variant at compile time.
#define A71CH_DEVICE_TYPE_A (0x41)  //!< Symbolic constant to represent the Initial A71CH device
#define A71CH_DEVICE_TYPE_B (0x42)  //!< Synbolic constant to represent the A71CH device with more storage
#define A71CH_DEVICE_TYPE A71CH_DEVICE_TYPE_B //!< Indicate the credential storage capabilities of the A71CH


/** @def A71CH_KEY_PAIR_MAX
 * Maximum ECC Key Pairs in the Secure module */
/** @def A71CH_PUBLIC_KEY_MAX
 * Maximum ECC Public Keys in the Secure module */
/** @def A71CH_SYM_KEY_MAX
 * Maximum ECC Symmetric Keys in the Secure module */
/** @def A71CH_GP_STORAGE_SIZE
 * Maximum General Purpose Storage in the Secure module */

#if A71CH_DEVICE_TYPE == A71CH_DEVICE_TYPE_A
//!< Effective value used in examples
#   define A71CH_KEY_PAIR_MAX   A71CH_KEY_PAIR_MAX_A
#   define A71CH_PUBLIC_KEY_MAX A71CH_PUBLIC_KEY_MAX_A
#   define A71CH_SYM_KEY_MAX    A71CH_SYM_KEY_MAX_A
#   define A71CH_GP_STORAGE_SIZE A71CH_GP_STORAGE_SIZE_A
#else
#   define A71CH_KEY_PAIR_MAX   A71CH_KEY_PAIR_MAX_B
#   define A71CH_PUBLIC_KEY_MAX A71CH_PUBLIC_KEY_MAX_B
#   define A71CH_SYM_KEY_MAX    A71CH_SYM_KEY_MAX_B
#   define A71CH_GP_STORAGE_SIZE A71CH_GP_STORAGE_SIZE_B
#endif

#ifdef __cplusplus
}
#endif
#endif //_A71CHCONSTL_H_
