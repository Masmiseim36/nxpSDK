/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#ifndef DEMO_POWER_CPU1_PRINT_ENABLE
#define DEMO_POWER_CPU1_PRINT_ENABLE 1U
#endif
#if DEMO_POWER_CPU1_PRINT_ENABLE
#define DEMO_LOG PRINTF
#else
#define DEMO_LOG(...)
#endif

#define APP_CORE_NAME "CPU1"

#define APP_USER_WAKEUP_KEY_GPIO BOARD_SW6_GPIO
#define APP_USER_WAKEUP_KEY_PIN  BOARD_SW6_GPIO_PIN
#define APP_USER_WAKEUP_KEY_NAME "SW6"
#define APP_SW_IRQ_HANDLER       GPIO80_IRQHandler
#define APP_SW_IRQN              GPIO80_IRQn
#define APP_SW_RESET_RSTn        kGPIO8_RST_SHIFT_RSTn
#define APP_SW_CLOCK_EN          kCLOCK_Gpio8
#define APP_SW_LP_REQ            kPower_GPIO8_LPREQ

#define APP_RTC             RTC1
#define APP_RTC_IRQN        RTC1_ALARM_IRQn
#define APP_RTC_IRQ_HANDLER RTC1_ALARM_IRQHandler

#define APP_USART_RX_ERROR (kLPUART_RxOverrunFlag | kLPUART_RxFifoUnderflowFlag)

/*!< Power down all unnecessary blocks and enable RBB during deep sleep. */
#define APP_DEEPSLEEP_SLEEPCFG    (0U)        /* SLEEPCON->SLEEPCFG */
#define APP_DEEPSLEEP_PDSLEEPCFG0 (0U)        /* PMC->PDSLEEPCFG0 */
#define APP_DSR_PDSLEEPCFG0       (PMC_PDSLEEPCFG0_PMICMODE(2U))        /* PMC->PDSLEEPCFG0, override default PMIC mode. */
#define APP_DEEPSLEEP_RAM_APD     0x3FFC0000U /* PMC->PDSLEEPCFG2, 0x580000 - 0x67FFFF([PT18-PT29]) keep powered */
#define APP_DEEPSLEEP_RAM_PPD     0x0U        /* PMC->PDSLEEPCFG3 */
#define APP_EXCLUDE_FROM_DEEPSLEEP                                                                     \
    (((const uint32_t[]){APP_DEEPSLEEP_SLEEPCFG, APP_DEEPSLEEP_PDSLEEPCFG0, 0U, APP_DEEPSLEEP_RAM_APD, \
                         APP_DEEPSLEEP_RAM_PPD, 0U, 0U}))
#define APP_EXCLUDE_FROM_DSR                                                                     \
    (((const uint32_t[]){APP_DEEPSLEEP_SLEEPCFG, APP_DSR_PDSLEEPCFG0, 0U, APP_DEEPSLEEP_RAM_APD, \
                         APP_DEEPSLEEP_RAM_PPD, 0U, 0U}))
#define APP_EXCLUDE_FROM_DEEP_POWERDOWN      (((const uint32_t[]){0, PMC_PDSLEEPCFG0_PMICMODE(3U), 0, 0, 0, 0, 0})) /* Override default PMIC mode.*/
#define APP_EXCLUDE_FROM_FULL_DEEP_POWERDOWN (((const uint32_t[]){0, 0, 0, 0, 0, 0, 0}))
/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_InitPowerConfig(void);
void BOARD_RunActiveTest(void);
void BOARD_RestorePeripheralsAfterDSR(void);
void BOARD_EnterSleep(void);
void BOARD_EnterDeepSleep(const uint32_t exclude_from_pd[7]);
void BOARD_RequestDSR(const uint32_t exclude_from_pd[7]);
void BOARD_RequestDPD(const uint32_t exclude_from_pd[7]);
void BOARD_RequestFDPD(const uint32_t exclude_from_pd[7]);

/*${prototype:end}*/

#endif /* _APP_H_ */
