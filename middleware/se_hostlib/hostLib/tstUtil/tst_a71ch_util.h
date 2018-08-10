/**
 * @file tst_a71ch_util.h
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
