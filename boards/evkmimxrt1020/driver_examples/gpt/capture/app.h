/*
 * Copyright 2019 NXP
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
#define DEMO_GPT_IRQn GPT2_IRQn
#define DEMO_GPT_BASE GPT2
/* GPT channel used for input capture */
#define BOARD_GPT_INPUT_CAPTURE_CHANNEL kGPT_InputCapture_Channel1
/* Interrupt to enable and flag to read; depends on the GPT channel used */
#define EXAMPLE_GPT_CAPTURE_IRQHandler     GPT2_IRQHandler
#define BOARD_GPT_CHANNEL_INTERRUPT_ENABLE kGPT_InputCapture1InterruptEnable
#define BOARD_GPT_CHANNEL_FLAG             kGPT_InputCapture1Flag
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
