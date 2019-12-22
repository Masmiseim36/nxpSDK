/**
 * @file ax_common.h
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

#if		defined(TGT_A71CL)
#include "ax_common_a71cl.h"
#define A71XX_CLA A71CL_CLA
#define A71XX_INS_REFLECT A71CL_INS_REFLECT
#define A71XX_INS_ERASE_MODULE A71CL_INS_ERASE_MODULE
#define A71XX_INS_FREEZE_MODULE A71CL_INS_FREEZE_MODULE
#define A71XX_INS_GET_MODULE A71CL_INS_GET_MODULE

#elif (SSS_HAVE_A71CH || SSS_HAVE_SE050_EAR_CH || SSS_HAVE_SE05X)
#include "ax_common_a71ch.h"
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
