/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_pca9420.h"
/*${header:end}*/
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/*
 * The option is used for measurement of the deep sleep wake-up latency. When the application calls
 * POWER_EnterDeepSleep(), the power library will control the PLL power depending on the
 * exclude_from_pd parameter and the PLL running status. If the PLL is not in the exclude_from_pd list
 * and is running, it will be powered off before WFI and restored after WFI by the power library. Otherwise,
 * the PLL control will not be changed in POWER_EnterDeepSleep().
 * As the PLL initialization costs time to stabilize the clock, user will see the duration of
 * POWER_EnterDeepSleep() is longer than expectation.
 * To get rid of the PLL clock initialization time from deep sleep wake-up latency measurement, we can set
 * POWER_DOWN_PLL_BEFORE_DEEP_SLEEP to 1, then the demo itself will disable the PLL before
 * calling POWER_EnterDeepSleep(), and restore PLL after that. Thus we get the real duration of system
 * deep sleep wake-up time.
 */
#define POWER_DOWN_PLL_BEFORE_DEEP_SLEEP \
    0 /* By default, we keep the application as simple as possible to make good OOBE. */

#define APP_USART_RX_ERROR               kUSART_RxError
#define APP_USER_WAKEUP_KEY_NAME         "SW1"
#define APP_USER_WAKEUP_KEY_GPIO         BOARD_SW1_GPIO
#define APP_USER_WAKEUP_KEY_PORT         BOARD_SW1_GPIO_PORT
#define APP_USER_WAKEUP_KEY_PIN          BOARD_SW1_GPIO_PIN
#define APP_USER_WAKEUP_KEY_INPUTMUX_SEL kINPUTMUX_GpioPort1Pin1ToPintsel
/*!< Power down all unnecessary blocks and enable RBB during deep sleep. */
#define APP_DEEPSLEEP_RUNCFG0 (SYSCTL0_PDSLEEPCFG0_RBB_PD_MASK)
#define APP_DEEPSLEEP_RAM_APD 0xFFFFF8U
#define APP_DEEPSLEEP_RAM_PPD 0x0U
#define APP_EXCLUDE_FROM_DEEPSLEEP                                                                                \
    (((const uint32_t[]){APP_DEEPSLEEP_RUNCFG0,                                                                   \
                         (SYSCTL0_PDSLEEPCFG1_FLEXSPI_SRAM_APD_MASK | SYSCTL0_PDSLEEPCFG1_FLEXSPI_SRAM_PPD_MASK), \
                         APP_DEEPSLEEP_RAM_APD, APP_DEEPSLEEP_RAM_PPD}))

#define APP_EXCLUDE_FROM_DEEP_POWERDOWN      (((const uint32_t[]){0, 0, 0, 0}))
#define APP_EXCLUDE_FROM_FULL_DEEP_POWERDOWN (((const uint32_t[]){0, 0, 0, 0}))
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_ConfigPMICModes(pca9420_modecfg_t *cfg, uint32_t num);
#if POWER_DOWN_PLL_BEFORE_DEEP_SLEEP
void BOARD_DisablePll();
void BOARD_RestorePll();
#endif
/*${prototype:end}*/

#endif /* _APP_H_ */
