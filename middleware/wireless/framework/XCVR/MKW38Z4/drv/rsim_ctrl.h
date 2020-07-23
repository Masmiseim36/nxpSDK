/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef RSIM_CTRL_H
/* clang-format off */
#define RSIM_CTRL_H
/* clang-format on */

#include "fsl_common.h"

/*!
 * @addtogroup RSIM Routines
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define RSIM_POWERUP_SUPPORTED     (false) /* RSIM doesn't support Radio Powerup/down */



/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @brief Function to Startup RF OSC.
 *
 * This function enables RF OSC and waits for ready indication.
 *
 * @note
 *  This function may wait an indeterminate amount of time if RF OSC enable doesn't ever indicate ready.
 */
void RSIM_rf_osc_startup(void);

/*!
 * @brief Function to shutdown RF OSC.
 *
 * This function disables RF OSC.
 *
 */
void RSIM_rf_osc_shutdown(void);

/*!
 * @brief Function to reset the radio.
 *
 * This function resets the entire radio to it's POR state.
 *
 */
void RSIM_radio_reset(void);

#if (RSIM_POWERUP_SUPPORTED)
void RSIM_radio_powerup(void);

void RSIM_radio_powerdown(void);
#endif /* RSIM_POWERUP_SUPPORTED */

#if defined(__cplusplus)
extern "C" {
#endif





/* @} */

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* RSIM_CTRL_H */



