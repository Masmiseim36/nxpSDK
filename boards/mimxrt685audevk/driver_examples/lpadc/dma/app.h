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
#define DEMO_LPADC_BASE             ADC0
#define DEMO_LPADC_USER_CHANNEL     0U
#define DEMO_LPADC_USER_CMDID       1U /* The available command number are 1-15 */
#define DEMO_LPADC_RESFIFO_REG_ADDR (uint32_t)(&(ADC0->RESFIFO))
#define DEMO_RESULT_FIFO_READY_FLAG kLPADC_ResultFIFOReadyFlag

#define DEMO_DMA_BASE             DMA0
#define DEMO_DMA_ADC_CHANNEL      0U
#define DEMO_DMA_TRANSFER_TYPE    kDMA_MemoryToMemory
#define DEMO_DMA_HARDWARE_TRIGGER true
#define DEMO_DMA_ADC_CONNECTION   kINPUTMUX_AdcToDma0

#define DEMO_INPUTMUX_BASE INPUTMUX
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
