/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
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
#define EXAMPLE_INT_IRQn                (PIN_INT0_IRQn)
#define EXAMPLE_SENSOR_INT_GPIO         GPIO
#define EXAMPLE_SENSOR_INT_PORT         0
#define EXAMPLE_SENSOR_INT_PIN          22
#define EXAMPLE_SENSOR_INT_INPUTMUX_SEL kINPUTMUX_GpioPort0Pin22ToPintsel

/*!< Power down all unnecessary blocks during deep sleep*/
#define EXAMPLE_DEEPSLEEP_RUNCFG0 (SYSCTL0_PDRUNCFG0_RBBSRAM_PD_MASK | SYSCTL0_PDRUNCFG0_RBB_PD_MASK)
#define EXAMPLE_DEEPSLEEP_RAM_APD 0xFFC00000U /* 0x280000 - 0x4FFFFF keep powered */
#define EXAMPLE_DEEPSLEEP_RAM_PPD 0x0U
#define EXAMPLE_EXCLUDE_FROM_DEEPSLEEP                                                                       \
    (((const uint32_t[]){EXAMPLE_DEEPSLEEP_RUNCFG0,                                                          \
                         (SYSCTL0_PDSLEEPCFG1_FLEXSPI0_SRAM_APD_MASK | SYSCTL0_PDSLEEPCFG1_SRAM_SLEEP_MASK), \
                         EXAMPLE_DEEPSLEEP_RAM_APD, EXAMPLE_DEEPSLEEP_RAM_PPD}))
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void EXAMPLE_InitWakeupPin(void);
void EXAMPLE_EnterDeepSleep(void);
void BOARD_InitHardware(void);
void APP_ACCEL_IntHandler(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
