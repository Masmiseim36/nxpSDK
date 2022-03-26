/** @file mlan_main_defs.h
 *
 *  @brief This file contains common definitions for SDIO.
 *
 *  Copyright 2022 NXP
 *
 *  NXP CONFIDENTIAL
 *  The source code contained or described herein and all documents related to
 *  the source code ("Materials") are owned by NXP, its
 *  suppliers and/or its licensors. Title to the Materials remains with NXP,
 *  its suppliers and/or its licensors. The Materials contain
 *  trade secrets and proprietary and confidential information of NXP, its
 *  suppliers and/or its licensors. The Materials are protected by worldwide copyright
 *  and trade secret laws and treaty provisions. No part of the Materials may be
 *  used, copied, reproduced, modified, published, uploaded, posted,
 *  transmitted, distributed, or disclosed in any way without NXP's prior
 *  express written permission.
 *
 *  No license under any patent, copyright, trade secret or other intellectual
 *  property right is granted to or conferred upon you by disclosure or delivery
 *  of the Materials, either expressly, by implication, inducement, estoppel or
 *  otherwise. Any license under such intellectual property rights must be
 *  express and approved by NXP in writing.
 *
 */
/****************************************************
Change log:
****************************************************/

#ifndef _MLAN_MAIN_DEFS_H_
#define _MLAN_MAIN_DEFS_H_

#if defined(SD8801)
/** Maximum numbfer of registers to read for multiple port */
#define MAX_MP_REGS 64
/** Maximum port */
#define MAX_PORT 16
/** Multi port aggregation packet limit */
#define SDIO_MP_AGGR_DEF_PKT_LIMIT (4)
#elif defined(SD8977) || defined(SD8978) || defined(SD8987) || defined(SD8997) || defined(SD9097) || \
    defined(SD9098) || defined(IW61x)
/** Maximum numbfer of registers to read for multiple port */
#define MAX_MP_REGS                196
/** Maximum port */
#define MAX_PORT                   32U
/** Multi port aggregation packet limit */
#define SDIO_MP_AGGR_DEF_PKT_LIMIT (4U)
#endif

#endif //_MLAN_MAIN_DEFS_H_
