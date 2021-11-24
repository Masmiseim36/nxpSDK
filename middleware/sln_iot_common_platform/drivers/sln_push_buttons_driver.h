/*
 * Copyright 2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef _SLN_PUSH_BUTTONS_DRIVER_H_
#define _SLN_PUSH_BUTTONS_DRIVER_H_

#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*!
 * @brief Push Button Initialization
 *
 * @returns Status of initialization
 */
status_t PUSH_BUTTONS_Init(void);

/*!
 * @brief Checks whether or not the MSD button is pressed
 *
 * @returns true if pressed, false if not
 */
bool PUSH_BUTTONS_MSDPressed(void);

/*!
 * @brief Checks whether or not the OTW button is pressed
 *
 * @returns true if pressed, false if not
 */
bool PUSH_BUTTONS_OTWPressed(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */
#endif /* _SLN_PUSH_BUTTONS_DRIVER_H_ */
