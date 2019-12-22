/**
 * @file tst_a71ch_util.h
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
 * API of test bench utility functions specific to the a71ch
 * @par History
 * 1.0   2016-Oct-1 : Initial version
 */

#ifndef _TST_A71CH_UTIL_H_
#define _TST_A71CH_UTIL_H_

#include "sm_types.h"
#include "ax_api.h"

#ifdef __cplusplus
extern "C" {
#endif

#define INIT_MODE_RESET                     0x00  //!< Only issue a Debug Reset
#define INIT_MODE_RESET_SELECT              0x01  //!< Issue a Debug Reset with a subsequent applet select
#define INIT_MODE_NO_RESET                  0x02  //!< Skip resetting secure element
#define INIT_MODE_RESET_DO_SCP03            0x80  //!< Issue a Debug Reset, next setup SCP03 channel
#define INIT_MODE_RESET_SELECT_DO_SCP03     0x81  //!< Issue a Debug Reset, do a subsequent applet select, next setup SCP03 channel
#define INIT_MODE_NO_RESET_DO_SCP03         0x82  //!< Setup SCP03 channel

/// @cond
#define INIT_MODE_RESET_MASK                0x03
#define INIT_MODE_PATTERN_RESET             0x00
#define INIT_MODE_PATTERN_RESET_SELECT      0x01
#define INIT_MODE_PATTERN_NO_RESET          0x02

#define INIT_MODE_SCP03_MASK                0x80
#define INIT_MODE_PATTERN_PLAIN_COM         0x00
#define INIT_MODE_PATTERN_DO_SCP03          0x80
/// @endcond

const char* getInitModeAsString(U8 initMode);
int a71chScp03Requested(U8 initMode);

U8 a71chInitModule(U8 initMode);
U8 a71chSetupScp03(void);
U8 a71chShowModuleInfo(U8 *scpState);

#ifdef __cplusplus
}
#endif
#endif // _TST_A71CH_UTIL_H_
