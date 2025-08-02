/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "board.h"
#include "fsl_rgpio.h"
#include "fsl_lpuart.h"
#include "fsl_tpm.h"
/*${header:end}*/

#define DEMO_LPUART            LPUART12
#define DEMO_LPUART_IRQn       LPUART12_IRQn
#define DEMO_LPUART_CLK_FREQ   BOARD_DebugConsoleSrcFreq()
#define DEMO_LPUART_IRQHandler LPUART12_IRQHandler

#define BOARD_LED_RGPIO                 BOARD_USER_LED_GPIO
#define BOARD_LED_RGPIO_PIN1            26U
#define BOARD_LED_RGPIO_PIN2            BOARD_USER_LED_GPIO_PIN

#define BOARD_TPM TPM1
/* Interrupt number and interrupt handler for the TPM instance used */
#define BOARD_TPM_IRQ_NUM TPM1_IRQn
#define BOARD_TPM_HANDLER TPM1_IRQHandler
/* Get source clock for TPM driver */
#define TPM_SOURCE_CLOCK CLOCK_GetRootClockFreq(kCLOCK_Root_Bus_Aon)

#define TIME_PERIOD_US  1750U
#ifndef TPM_PRESCALER
/* Calculate the clock division based on the PWM frequency to be obtained */
#define TPM_PRESCALER TPM_CalculateCounterClkDiv(BOARD_TPM, 1000000U / TIME_PERIOD_US, TPM_SOURCE_CLOCK);
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */