/**
 * @file a71ch_ex.h
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
 * This file implements the host API examples for the A71CH secure module.
 *
 * @par History
 * 1.0   2016-08-01 : Initial version
 *
 *****************************************************************************/
#ifndef _A71CH_EX_
#define _A71CH_EX_

#include "sm_types.h"
#include "a71ch_api.h"
#include "sm_const.h"

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


#define PLAIN_PSK 0x00 //!< Select RFC4279 example mode
#define ECDH_PSK  0x01 //!< Select RFC5489 example mode


#define DURATION_TEST      (0x01)   //!< Bit pattern to request a duration test (valid on some example functions)
#define MEASURE_EXEC_TIME  (0x02)   //!< Bit pattern to trigger the measurement of execution time (valid on some example functions)
#define EXTENDED_TEST      (0x04)   //!< Bit pattern to trigger an extended test (valid on some example functions)

/// @cond A71CH Example Utility functions
U8 axExAuthenticate(U8 *keyEnc, U8 *keyMac, U8 *keyDek);
U8 axExCreateAndSetInitialHostScpKeys(U8 *keyEnc, U8 *keyMac, U8 *keyDek);
/// @endcond

/** \name Example functions illustrating usage of A71CH functionality
   @{ */
U8 exAes(void);

U8 exAesRfc3394(U8 initMode);
U8 exAesRfc3394Precooked(U8 initMode);
U8 exSymHkdf(U8 initMode);
U8 exSymHmacSha256(U8 initMode);

U8 exConfig(void);
U8 exEccNohc(void);
U8 exGPStorage(U8 tstMode);
U8 exMisc(void);
U8 exPsk(void);
U8 exScp(void);
U8 exSst(void);
U8 exSstKp(void);
U8 exSstKeyPair(U8 initMode);

U8 exPskTls1_2(U8 initMode, U8 pskMode);
/** @}*/

/** \name Walkthrough example (illustrating typical product use)
  @{ */
U8 exWalkthrough(void);
/** @}*/

/** \name Example functions illustrating specific Debug Mode functions (only available in case the Applet is in Debug Mode)
   @{ */
U8 exDebugMode(U8 testMode);
U8 exDbgReflect(int nLower, int nUpper);
U8 exDbgInternalMemory(U8 initMode);

U8 exPermanentlyDisableDebugMode(void);
/** @}*/

/** \name Example function illustrating system boot in combination with SCP03 session key handover
   @{ */
U8 exBoot(U8 bootMode);
/** @}*/

#ifdef __cplusplus
}
#endif
#endif //_A71CH_EX_
