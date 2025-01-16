/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/

#define EXAMPLE_LED_GPIO     BOARD_USER_LED_GPIO
#define EXAMPLE_LED_GPIO_PIN BOARD_USER_LED_GPIO_PIN

#define EXAMPLE_SSARC_GROUP_ID         0U
#define EXAMPLE_SSARC_DESCRIPTOR_ID    0U
#define EXAMPLE_SSARC_GROUP_CPU_DOMAIN kSSARC_CM4Core

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/
