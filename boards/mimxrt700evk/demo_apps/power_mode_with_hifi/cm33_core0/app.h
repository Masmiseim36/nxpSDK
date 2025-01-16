/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "power_demo_config.h"
/*${header:end}*/
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#ifndef DEMO_POWER_CPU0_PRINT_ENABLE
#define DEMO_POWER_CPU0_PRINT_ENABLE 1U
#endif
/* Wheter HiFi uses LPUART for PRINTF or not */
#ifndef DEMO_POWER_HIFI_PRINT_ENABLE
#define DEMO_POWER_HIFI_PRINT_ENABLE 0U
#endif
#if DEMO_POWER_CPU0_PRINT_ENABLE
#define DEMO_LOG PRINTF
#else
#define DEMO_LOG(...)
#endif

#define APP_CORE_NAME            "CPU0"
#define BOOT_FLAG                0x1U /* Flag indicates Core1 Boot Up*/
#define APP_USER_WAKEUP_KEY_GPIO BOARD_SW5_GPIO
#define APP_USER_WAKEUP_KEY_PIN  BOARD_SW5_GPIO_PIN
#define APP_USER_WAKEUP_KEY_NAME "SW5"
#define APP_SW_IRQ_HANDLER       GPIO00_IRQHandler
#define APP_SW_IRQN              GPIO00_IRQn
#define APP_SW_RESET_RSTn        kGPIO0_RST_SHIFT_RSTn
#define APP_SW_CLOCK_EN          kCLOCK_Gpio0
#define APP_SW_LP_REQ            kPower_GPIO0_LPREQ

#define APP_RTC             RTC0
#define APP_RTC_IRQN        RTC0_ALARM_IRQn
#define APP_RTC_IRQ_HANDLER RTC0_ALARM_IRQHandler

#define APP_MU MU1_MUA

#define APP_USART_RX_ERROR (kLPUART_RxOverrunFlag | kLPUART_RxFifoUnderflowFlag)

/***********************Example configuration****************************/
/*!< Power down all unnecessary blocks and enable RBB during deep sleep. */
#if (DEMO_POWER_HIFI4_USED != 0U)
#if (DEMO_POWER_HIFI_PRINT_ENABLE != 0U)
#define APP_DEEPSLEEP_SLEEPCFG                                                                                        \
    (SLEEPCON0_SLEEPCFG_FRO0_PD_MASK | SLEEPCON0_SLEEPCFG_FRO0_GATE_MASK | SLEEPCON0_SLEEPCFG_RAM0_CLK_SHUTOFF_MASK | \
     SLEEPCON0_SLEEPCFG_COMP_MAINCLK_SHUTOFF_MASK) /* SLEEPCON->SLEEPCFG */
#else
#define APP_DEEPSLEEP_SLEEPCFG                                             \
    (SLEEPCON0_SLEEPCFG_FRO0_PD_MASK | SLEEPCON0_SLEEPCFG_FRO0_GATE_MASK | \
     SLEEPCON0_SLEEPCFG_RAM0_CLK_SHUTOFF_MASK)                    /* SLEEPCON->SLEEPCFG */
#endif                                                            /* DEMO_POWER_HIFI_PRINT_ENABLE */
#define APP_DEEPSLEEP_PDSLEEPCFG0 (PMC_PDSLEEPCFG0_V2DSP_PD_MASK) /* PMC->PDSLEEPCFG0 */
#define APP_DEEPSLEEP_PDSLEEPCFG1 (0U)                            /* PMC->PDSLEEPCFG1 */
#define APP_DSR_PDSLEEPCFG0       (0U)                            /* PMC->PDSLEEPCFG0 */
#define APP_DEEPSLEEP_RAM_APD     (0x3FFFFU) /* PMC->PDSLEEPCFG2, all keep powered, the unused PT can be power off. */
#define APP_DEEPSLEEP_RAM_PPD     (0x3C000U) /* PMC->PDSLEEPCFG3, keep PT14-17 which used by HIFI. */
#define APP_DEEPSLEEP_PDSLEEPCFG4                                                                        \
    (PMC_PDSLEEPCFG4_CPU0_CCACHE_MASK | PMC_PDSLEEPCFG4_CPU0_SCACHE_MASK | PMC_PDSLEEPCFG4_OCOTP_MASK |  \
     PMC_PDSLEEPCFG4_DSP_ICACHE_MASK | PMC_PDSLEEPCFG4_DSP_DCACHE_MASK | PMC_PDSLEEPCFG4_DSP_ITCM_MASK | \
     PMC_PDSLEEPCFG4_DSP_DTCM_MASK)

#define APP_DEEPSLEEP_PDSLEEPCFG5                                                                        \
    (PMC_PDSLEEPCFG5_DSP_ICACHE_MASK | PMC_PDSLEEPCFG5_DSP_DCACHE_MASK | PMC_PDSLEEPCFG5_DSP_ITCM_MASK | \
     PMC_PDSLEEPCFG5_DSP_DTCM_MASK)
#else
#define APP_DEEPSLEEP_SLEEPCFG    (0U)     /* SLEEPCON->SLEEPCFG */
#define APP_DEEPSLEEP_PDSLEEPCFG0 (0U)     /* PMC->PDSLEEPCFG0 */
#define APP_DEEPSLEEP_PDSLEEPCFG1 (0U)     /* PMC->PDSLEEPCFG1 */
#define APP_DSR_PDSLEEPCFG0       (0U)     /* PMC->PDSLEEPCFG0 */
#define APP_DEEPSLEEP_RAM_APD     0x3FFFFU /* PMC->PDSLEEPCFG2, all keep powered, the unused PT can be power off. */
#define APP_DEEPSLEEP_RAM_PPD     (0U)     /* PMC->PDSLEEPCFG3 */
#define APP_DEEPSLEEP_PDSLEEPCFG4 \
    (PMC_PDSLEEPCFG4_CPU0_CCACHE_MASK | PMC_PDSLEEPCFG4_CPU0_SCACHE_MASK | PMC_PDSLEEPCFG4_OCOTP_MASK)
#define APP_DEEPSLEEP_PDSLEEPCFG5 (0U)
#endif                               /* DEMO_POWER_HIFI4_USED */

#define APP_DSR_SLEEPCFG    (0U)     /* SLEEPCON->SLEEPCFG */
#define APP_DSR_PDSLEEPCFG0 (0U)     /* PMC->PDSLEEPCFG0 */
#define APP_DSR_PDSLEEPCFG1 (0U)     /* PMC->PDSLEEPCFG1 */
#define APP_DSR_RAM_APD     0x3FFFFU /* PMC->PDSLEEPCFG2, all keep powered, the unused PT can be power off. */
#define APP_DSR_RAM_PPD     (0U)     /* PMC->PDSLEEPCFG3 */
#define APP_DSR_PDSLEEPCFG4 (PMC_PDSLEEPCFG4_OCOTP_MASK) /* Cache is not retented in DSR. */
#define APP_DSR_PDSLEEPCFG5 (0U)

#define APP_EXCLUDE_FROM_DEEPSLEEP                                                                     \
    (((const uint32_t[]){APP_DEEPSLEEP_SLEEPCFG, APP_DEEPSLEEP_PDSLEEPCFG0, APP_DEEPSLEEP_PDSLEEPCFG1, \
                         APP_DEEPSLEEP_RAM_APD, APP_DEEPSLEEP_RAM_PPD, APP_DEEPSLEEP_PDSLEEPCFG4,      \
                         APP_DEEPSLEEP_PDSLEEPCFG5}))
#define APP_EXCLUDE_FROM_DSR                                                                          \
    (((const uint32_t[]){APP_DSR_SLEEPCFG, APP_DSR_PDSLEEPCFG0, 0U, APP_DSR_RAM_APD, APP_DSR_RAM_PPD, \
                         APP_DSR_PDSLEEPCFG4, APP_DSR_PDSLEEPCFG5}))
#define APP_EXCLUDE_FROM_DEEP_POWERDOWN      (((const uint32_t[]){0, 0, 0, 0, 0, 0, 0}))
#define APP_EXCLUDE_FROM_FULL_DEEP_POWERDOWN (((const uint32_t[]){0, 0, 0, 0, 0, 0, 0}))
/************************************************************************/
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_RestorePeripheralsAfterDSR(void);
void BOARD_DisablePll(void);
void BOARD_RestorePll(void);
void DEMO_InitDebugConsole(void);
void DEMO_DeinitDebugConsole(void);
void BOARD_RunActiveTest(void);
void BOARD_EnterSleep(void);
void BOARD_EnterDeepSleep(const uint32_t exclude_from_pd[7]);
void BOARD_RequestDSR(const uint32_t exclude_from_pd[7]);
void BOARD_RequestDPD(const uint32_t exclude_from_pd[7]);
void BOARD_RequestFDPD(const uint32_t exclude_from_pd[7]);
void BOARD_BootDSP(void);
void BOARD_StopDSP(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
