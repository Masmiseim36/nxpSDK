/**
* @file a71ch_const.h
* @author NXP Semiconductors
* @version 1.0
* @par License
* Copyright(C) NXP Semiconductors, 2016
* All rights reserved.
*
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* A7-series security ICs.  This software is supplied "AS IS" without any
* warranties of any kind, and NXP Semiconductors and its licensor disclaim any and
* all warranties, express or implied, including all implied warranties of
* merchantability, fitness for a particular purpose and non-infringement of
* intellectual property rights.  NXP Semiconductors assumes no responsibility
* or liability for the use of the software, conveys no license or rights under any
* patent, copyright, mask work right, or any other intellectual property rights in
* or to any products. NXP Semiconductors reserves the right to make changes
* in the software without notification. NXP Semiconductors also makes no
* representation or warranty that such application will be suitable for the
* specified use without further testing or modification.
*
* Permission to use, copy and modify this software is hereby granted,
* under NXP Semiconductors' and its licensor's relevant copyrights in
* the software, without fee, provided that it is used in conjunction with
* NXP Semiconductors products. This copyright, permission, and disclaimer notice
* must appear in all copies of this code.
* @par Description
* Host Library A71CH Constants.
* @par History
 *****************************************************************************/
#ifndef _A71CH_CONST_H_
#define _A71CH_CONST_H_

#ifdef __cplusplus
extern "C" {
#endif


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
