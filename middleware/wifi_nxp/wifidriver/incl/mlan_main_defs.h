/** @file mlan_main_defs.h
 *
 *  @brief This file contains common definitions for SDIO.
 *
 *  Copyright 2022 NXP
 *
 *  Licensed under the LA_OPT_NXP_Software_License.txt (the "Agreement")
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
#elif defined(SD8978) || defined(SD8987) || defined(SD8997) || defined(SD9097) || defined(SD9098) || defined(IW61x)
/** Maximum numbfer of registers to read for multiple port */
#define MAX_MP_REGS                196
/** Maximum port */
#define MAX_PORT                   32U
/** Multi port aggregation packet limit */
#define SDIO_MP_AGGR_DEF_PKT_LIMIT (4U)
#endif

#endif //_MLAN_MAIN_DEFS_H_
