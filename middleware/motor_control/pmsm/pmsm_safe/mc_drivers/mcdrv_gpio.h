/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _MCDRV_GPIO_H_
#define _MCDRV_GPIO_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*!
 * @brief   Functions enables the DC-bus brake.
 *
 * @param   void
 *
 * @return  none
 */
void MCDRV_fsBrakeOn(void);

/*!
 * @brief   Functions disables the DC-bus brake.
 *
 * @param   void
 *
 * @return  none
 */
void MCDRV_fsBrakeOff(void);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MCDRV_GPIO_H_ */

