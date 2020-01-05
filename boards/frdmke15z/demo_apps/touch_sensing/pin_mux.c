/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v3.0
processor: MKE15Z256xxx7
package_id: MKE15Z256VLL7
mcu_data: ksdk2_0
processor_version: 2.0.0
board: FRDM-KE15Z
pin_labels:
- {pin_num: '81', pin_signal: ADC1_SE4/TSI0_CH15/PTC6/LPUART1_RX, label: UART1_RX_TGTMCU}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"

/*FUNCTION**********************************************************************
 * 
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 * 
 *END**************************************************************************/
void BOARD_InitBootPins(void) {
    BOARD_InitPins();
}

#define PIN0_IDX                         0u   /*!< Pin number for pin 0 in a port */
#define PIN6_IDX                         6u   /*!< Pin number for pin 6 in a port */
#define PIN7_IDX                         7u   /*!< Pin number for pin 7 in a port */
#define PIN15_IDX                       15u   /*!< Pin number for pin 15 in a port */
#define PIN16_IDX                       16u   /*!< Pin number for pin 16 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '4', peripheral: FTM0, signal: 'CH, 2', pin_signal: TSI0_CH4/PTD0/FTM0_CH2/LPSPI1_SCK/FTM2_CH0/FXIO_D0/TRGMUX_OUT1}
  - {pin_num: '21', peripheral: FTM0, signal: 'CH, 1', pin_signal: PTD16/FTM0_CH1}
  - {pin_num: '22', peripheral: FTM0, signal: 'CH, 0', pin_signal: PTD15/FTM0_CH0}
  - {pin_num: '81', peripheral: LPUART1, signal: RX, pin_signal: ADC1_SE4/TSI0_CH15/PTC6/LPUART1_RX}
  - {pin_num: '80', peripheral: LPUART1, signal: TX, pin_signal: ADC1_SE5/TSI0_CH16/PTC7/LPUART1_TX}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortC);                           /* Clock Control: 0x01u */
  CLOCK_EnableClock(kCLOCK_PortD);                           /* Clock Control: 0x01u */

  PORT_SetPinMux(PORTC, PIN6_IDX, kPORT_MuxAlt2);            /* PORTC6 (pin 81) is configured as LPUART1_RX */
  PORT_SetPinMux(PORTC, PIN7_IDX, kPORT_MuxAlt2);            /* PORTC7 (pin 80) is configured as LPUART1_TX */
  PORT_SetPinMux(PORTD, PIN0_IDX, kPORT_MuxAlt2);            /* PORTD0 (pin 4) is configured as FTM0_CH2 */
  PORT_SetPinMux(PORTD, PIN15_IDX, kPORT_MuxAlt2);           /* PORTD15 (pin 22) is configured as FTM0_CH0 */
  PORT_SetPinMux(PORTD, PIN16_IDX, kPORT_MuxAlt2);           /* PORTD16 (pin 21) is configured as FTM0_CH1 */
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
