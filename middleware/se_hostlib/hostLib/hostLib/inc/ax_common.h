/**
 * @file ax_common.h
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 *
 * Copyright 2016,2020 NXP
 * SPDX-License-Identifier: Apache-2.0
 *
 * @par Description
 * Common data types and defines of the AX-family host library.
 * @par History
 * 1.0   27-march-2014 : Initial version
 *
 */
#ifndef _AX_COMMON_
#define _AX_COMMON_

#if defined(SSS_USE_FTR_FILE)
#include "fsl_sss_ftr.h"
#else
#include "fsl_sss_ftr_default.h"
#endif

#include "scp.h"
#include "sm_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#if (SSS_HAVE_APPLET_SE05X_IOT)
#define A71XX_CLA A71CH_CLA
#define A71XX_INS_REFLECT A71CH_INS_REFLECT
#define A71XX_INS_ERASE_MODULE A71CH_INS_ERASE_MODULE
#define A71XX_INS_FREEZE_MODULE A71CH_INS_FREEZE_MODULE
#define A71XX_INS_GET_MODULE A71CH_INS_GET_MODULE
#endif

#ifdef __cplusplus
}
#endif
#endif //_AX_COMMON_
