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
#define APP_CORE_NAME "CPU1"
#define APP_WAIT_INPUT_TO_START \
    0U /* If non-zero, need set DEMO_POWER_CPU1_PRINT_ENABLE to 1 and need input to enter Deep sleep mode. */

#define APP_MU            MU1_MUB
#define APP_MU_IRQHandler MU1_B_IRQHandler

/* Enable the PMC interrupt for other domain entering DS. */
#define APP_USE_PMC_INT         (0U)
#define APP_PMC_INT_ENABLE_MASK PMC_INTRCTRL_DSCOMPIE_MASK
#define APP_PMC_INT_STATUS_MASK PMC_FLAGS_DSCOMPF_MASK

#define APP_USART_RX_ERROR (kLPUART_RxOverrunFlag | kLPUART_RxFifoUnderflowFlag)

/*!< Power down all unnecessary blocks and enable RBB during deep sleep. */
#define APP_DEEPSLEEP_SLEEPCFG    (0U)        /* SLEEPCON->SLEEPCFG */
#define APP_DEEPSLEEP_PDSLEEPCFG0 (0U)
#define APP_DSR_PDSLEEPCFG0       (0U)        /* PMC->PDSLEEPCFG0 */
#define APP_DEEPSLEEP_RAM_APD     0x3FFC0000U /* PMC->PDSLEEPCFG2, all keep powered */
#define APP_DEEPSLEEP_RAM_PPD     0x0U        /* PMC->PDSLEEPCFG3 */
#define APP_EXCLUDE_FROM_DEEPSLEEP                                                                     \
    (((const uint32_t[]){APP_DEEPSLEEP_SLEEPCFG, APP_DEEPSLEEP_PDSLEEPCFG0, 0U, APP_DEEPSLEEP_RAM_APD, \
                         APP_DEEPSLEEP_RAM_PPD, 0U, 0U}))
#define APP_EXCLUDE_FROM_DSR                                                                     \
    (((const uint32_t[]){APP_DEEPSLEEP_SLEEPCFG, APP_DSR_PDSLEEPCFG0, 0U, APP_DEEPSLEEP_RAM_APD, \
                         APP_DEEPSLEEP_RAM_PPD, 0U, 0U}))
#define APP_EXCLUDE_FROM_DEEP_POWERDOWN      (((const uint32_t[]){0, 0, 0, 0, 0, 0, 0}))
#define APP_EXCLUDE_FROM_FULL_DEEP_POWERDOWN (((const uint32_t[]){0, 0, 0, 0, 0, 0, 0}))
/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_NotifyBoot(void);
void BOARD_EnterDeepSleep(const uint32_t exclude_from_pd[7]);
void BOARD_RequestDPD(const uint32_t exclude_from_pd[7]);
void BOARD_RequestFDPD(const uint32_t exclude_from_pd[7]);
/*${prototype:end}*/

#endif /* _APP_H_ */
