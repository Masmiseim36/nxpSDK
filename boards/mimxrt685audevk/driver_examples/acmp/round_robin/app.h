/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_ACMP_BASEADDR CMP

#define DEMO_ACMP_IRQ_ID           ACMP_IRQn
#define DEMO_ACMP_IRQ_HANDLER_FUNC ACMP_IRQHandler

/* Select which channels is used to do round robin checker.
 * Example sets positive port as fixed port with DAC output as comparison reference. So Pre-state mask bit high
 * represents the pre-state of corresponding channel's input voltage is higher than DAC output voltage. If the round
 * robin check result shows that corresponding channel's actual input voltage is lower than DAC output voltage, wakeup
 * event will be generated. The case of pre-state mask bit low is contrary to the case of pre-state mask bit high.
 */
#define DEMO_ACMP_ROUND_ROBIN_CHANNELS_CHECKER_MASK   0x01U /* ACMP CHAN-A. */
#define DEMO_ACMP_ROUND_ROBIN_CHANNELS_PRE_STATE_MASK 0x01U /* ACMP CHAN-A. */
#define DEMO_ACMP_ROUND_ROBIN_PERIOD_MILLISECONDS     1000U
#define DEMO_ACMP_ROUND_ROBIN_FIXED_CHANNEL           0x07U /* DAC output */

/*!< Power down all unnecessary blocks and enable RBB during deep sleep. */
#define EXAMPLE_DEEPSLEEP_RUNCFG0 (SYSCTL0_PDRUNCFG0_ACMP_PD_MASK | SYSCTL0_PDSLEEPCFG0_RBB_PD_MASK)
#define EXAMPLE_DEEPSLEEP_RAM_APD 0xFFFFF8U
#define EXAMPLE_DEEPSLEEP_RAM_PPD 0x0U
#define EXAMPLE_EXCLUDE_FROM_DEEPSLEEP                                                                            \
    (((const uint32_t[]){EXAMPLE_DEEPSLEEP_RUNCFG0,                                                               \
                         (SYSCTL0_PDSLEEPCFG1_FLEXSPI_SRAM_APD_MASK | SYSCTL0_PDSLEEPCFG1_FLEXSPI_SRAM_PPD_MASK), \
                         EXAMPLE_DEEPSLEEP_RAM_APD, EXAMPLE_DEEPSLEEP_RAM_PPD}))
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_InitAcmpRoundRobinTrigger(uint32_t triggerPeroid_Ms);
void BOARD_ClearAcmpRoundRobinTrigger(void);
void BOARD_EnterStopMode(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
