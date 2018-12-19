/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _USER_SETTINGS_H_
#define _USER_SETTINGS_H_

#include "fsl_common.h"

/* settings used for lwip_httpssrv_wolfssl_bm and lwip_httpssrv_wolfssl_freertos apps */

#define FSL_HW_CRYPTO_MANUAL_SELECTION

#if defined(FSL_FEATURE_SOC_MMCAU_COUNT) && FSL_FEATURE_SOC_MMCAU_COUNT
#define FREESCALE_USE_MMCAU
#endif

#if defined(FSL_FEATURE_SOC_LTC_COUNT) && FSL_FEATURE_SOC_LTC_COUNT
#define FREESCALE_USE_LTC
#endif

#if defined(FSL_FEATURE_SOC_AES_COUNT) && FSL_FEATURE_SOC_AES_COUNT
#define FREESCALE_USE_LPC_AES
#endif

/*
 * Do not use LPC SHA engine since WolfSSL interleaves calculation of multiple SHAs
 * and LPC does not support to set initial digest value.
 *
 * #if defined (FSL_FEATURE_SOC_SHA_COUNT) && FSL_FEATURE_SOC_SHA_COUNT
 *   #define FREESCALE_USE_LPC_SHA
 * #endif
 */

#endif /* _USER_SETTINGS_H_ */
