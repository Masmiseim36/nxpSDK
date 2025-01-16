/*
 * Copyright 2024 NXP
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
#define DEMO_LPADC_BASE                ADC0
#define DEMO_LPADC_IRQn                ADC_IRQn
#define DEMO_LPADC_IRQ_HANDLER_FUNC    ADC_IRQHandler
#define DEMO_LPADC_USER_CHANNEL        6U
#define DEMO_LPADC_USER_CMDID          1U /* The available command number are 1-15 */
#define DEMO_LPADC_USE_HIGH_RESOLUTION 1U
#define TSENS_CAL_OTP_FUSE_INDEX \
    77U /* TSENS_CAL is an 8-bit signed calibration constant retrieved from non-volatile memory.*/
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
float DEMO_GetTempsenorValue(void);
status_t DEMO_TempsenorInit(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
