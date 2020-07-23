/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef NXP_XCVR_MSK_CONFIG_H
/* clang-format off */
#define NXP_XCVR_MSK_CONFIG_H
/* clang-format on */

#include "nxp2p4_xcvr.h"
#include "nxp_xcvr_common_config.h" /* make common configs available to the mode config file for pointer struct assembly */

/*!
 * @addtogroup configs Radio Configuration Files
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * API
 ******************************************************************************/

/*! @brief Mode MSK 1mbps specific configuration */
extern const xcvr_mode_datarate_config_t xcvr_msk_1mbps_config;

/*! @brief Mode MSK 500kbps specific configuration */
extern const xcvr_mode_datarate_config_t xcvr_msk_500kbps_config;

/*! @brief Mode MSK 250kbps specific configuration */
extern const xcvr_mode_datarate_config_t xcvr_msk_250kbps_config;

/* COMPLETE CONFIG STRUCTURES */
/*! @brief  MSK mode & datarate 1Mbps specific configuration. */
extern const xcvr_config_t xcvr_msk_1mbps_full_config;

/*! @brief  MSK mode & datarate 500Kbps specific configuration. */
extern const xcvr_config_t xcvr_msk_500kbps_full_config;

/*! @brief  MSK mode & datarate 250Kbps specific configuration. */
extern const xcvr_config_t xcvr_msk_250kbps_full_config;

#if defined(__cplusplus)
extern "C" {
#endif





/* @} */

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* NXP_XCVR_MSK_CONFIG_H */


