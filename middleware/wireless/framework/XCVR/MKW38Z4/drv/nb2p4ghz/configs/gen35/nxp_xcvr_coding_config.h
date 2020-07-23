/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef NXP_XCVR_CODING_CONFIG_H
/* clang-format off */
#define NXP_XCVR_CODING_CONFIG_H
/* clang-format on */

#include "nxp2p4_xcvr.h"

/*!
 * @addtogroup configs Radio Configuration Files
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 *******************************************************************************/

/*! @brief Mode BLE uncoded specific configuration */
extern const xcvr_coding_config_t xcvr_ble_uncoded_config;
/*! @brief Mode BLE CODED s8 specific configuration */
extern const xcvr_coding_config_t xcvr_ble_coded_s8_config;
/*! @brief Mode BLE CODED s2 specific configuration */
extern const xcvr_coding_config_t xcvr_ble_coded_s2_config;
/*! @brief Mode ANT uncoded specific configuration */
extern const xcvr_coding_config_t xcvr_ant_uncoded_config;


#if defined(__cplusplus)
extern "C" {
#endif





/* @} */

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* NXP_XCVR_CODING_CONFIG_H */



