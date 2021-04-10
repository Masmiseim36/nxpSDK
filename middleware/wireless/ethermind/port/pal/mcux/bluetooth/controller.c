/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"

#include "controller.h"
#if defined(WIFI_BOARD_AW_AM457) || defined(WIFI_BOARD_AW_CM358MA)
#include "controller_wifi_nxp.h"
#endif

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
 ******************************************************************************/

/* Initialize the platform */
void controller_init(void)
{
#if defined(WIFI_BOARD_AW_AM457) || defined(WIFI_BOARD_AW_CM358MA)
    controller_wifi_nxp_init();
#endif
}

