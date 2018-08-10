/**
 * @file a71ch_ex_hlse.h
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
 *
 * @par Description
 * This file implements the host API examples for the A71CH secure module.
 *  Using Generic API
 *
 * @par History
 * 1.0   2016-08-01 : Initial version
 *
 *****************************************************************************/
#ifndef _A71CH_EX_HLSE_
#define _A71CH_EX_HLSE_

#include "sm_types.h"
#include "a71ch_const.h"

#ifdef __cplusplus
extern "C" {
#endif

/// @cond
#define REDUCED_PACKETSIZE_FALSE (0x00)
#define REDUCED_PACKETSIZE_TRUE  (0x01)

// When A71CH_ALLOW_DISABLE_DEBUG_MODE is defined, the example code
// will permanently disable the DEBUG_MODE in case exDebugPermanentlyDisableDebugMode() is invoked.
//
// #define A71CH_ALLOW_DISABLE_DEBUG_MODE
/// @endcond

#define DURATION_TEST      (0x01)   //!< Bit pattern to request a duration test (valid on some example functions)
#define MEASURE_EXEC_TIME  (0x02)   //!< Bit pattern to trigger the measurement of execution time (valid on some example functions)
#define EXTENDED_TEST      (0x04)   //!< Bit pattern to trigger an extended test (valid on some example functions)

#define BOOT_SKIP_EXAMPLE       (0x00)  //!< Argument relevant to ::exBoot
#define BOOT_SIMULATED_CYCLE    (0x01)  //!< Argument relevant to ::exBoot
#define BOOT_BOOTLOADER_ROLE    (0x02)  //!< Argument relevant to ::exBoot
#define BOOT_HOST_OS_RESUME     (0x04)  //!< Argument relevant to ::exBoot

/// @cond A71CH Example Utility functions
U8 axExHlseAuthenticate(U8 *keyEnc, U8 *keyMac, U8 *keyDek);
U8 axExHlseCreateAndSetInitialHostScpKeys(U8 *keyEnc, U8 *keyMac, U8 *keyDek);
/// @endcond

/** \name Example functions illustrating usage of A71CH functionality
   @{ */
U8 exHlseAes(void);
U8 exHlseConfig(void);
U8 exHlseEccNohc(void);
U8 exHlseGPStorage(U8 tstMode);
U8 exHlseMisc(void);
U8 exHlsePsk(void);
U8 exHlseScp(void);
U8 exHlseSst(U16 appletVersion);
U8 exHlseSstKp(void);
/** @}*/

/** \name Walkthrough example (illustrating typical product use)
  @{ */
U8 exHlseWalkthrough(void);
/** @}*/

/** \name Example functions illustrating specific Debug Mode functions (only available in case the Applet is in Debug Mode)
   @{ */
U8 exHlseDebugMode(U8 testMode);
U8 exHlsePermanentlyDisableDebugMode(void);
/** @}*/

/** \name Example function illustrating system boot in combination with SCP03 session key handover
@{ */
U8 exHlseBoot(U8 bootMode);
/** @}*/

/*
 * New examples
 */

/** \name Example function illustrating support for logical Certificate objects.
@{ */
U8 exHlseCert(void);
/** @}*/

#ifdef __cplusplus
}
#endif
#endif //_A71CH_EX_HLSE_
