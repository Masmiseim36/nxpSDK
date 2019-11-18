/*
 * Copyright 2018 NXP
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @addtogroup pin_mux
 * @{
 */

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);                                 /*!< Function assigned for the core: Cortex-M4F(core 0)[m4_0] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void LPI2C2_InitPins(void);                                /*!< Function assigned for the core: Cortex-M4F(core 0)[m4_0] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void LPI2C2_DeinitPins(void);                              /*!< Function assigned for the core: Cortex-M4F(core 0)[m4_0] */

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _PIN_MUX_H_ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
