/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"

#include "controller.h"
#if (defined(WIFI_IW416_BOARD_AW_AM457_USD) || defined(WIFI_88W8987_BOARD_AW_CM358_USD) || defined(WIFI_88W8987_BOARD_AW_CM358MA))
#include "controller_wifi_nxp.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Weak function. */
#if defined(__GNUC__)
#define __WEAK_FUNC __attribute__((weak))
#elif defined(__ICCARM__)
#define __WEAK_FUNC __weak
#elif defined(__CC_ARM) || defined(__ARMCC_VERSION)
#define __WEAK_FUNC __attribute__((weak))
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
__WEAK_FUNC int controller_hci_uart_get_configuration(controller_hci_uart_config_t *config);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/* Initialize the platform */
void controller_init(void)
{
#if (defined(WIFI_IW416_BOARD_AW_AM457_USD) || defined(WIFI_88W8987_BOARD_AW_CM358_USD) || defined(WIFI_88W8987_BOARD_AW_CM358MA))
    controller_wifi_nxp_init();
#endif
}

__WEAK_FUNC int controller_hci_uart_get_configuration(controller_hci_uart_config_t *config)
{
    return -1;
}
