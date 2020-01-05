/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
 
 /*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v3.0
processor: MKL28Z512xxx7
package_id: MKL28Z512VLL7
mcu_data: ksdk2_0
processor_version: 2.0.0
* BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"



#define PIN16_IDX                       16u   /*!< Pin number for pin 16 in a port */
#define PIN17_IDX                       17u   /*!< Pin number for pin 17 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '62', peripheral: LPUART0, signal: RX, pin_signal: TSI0_CH9/PTB16/LPSPI1_SOUT/LPUART0_RX/TPM0_CLKIN/LPSPI2_PCS3/FXIO0_D16}
  - {pin_num: '63', peripheral: LPUART0, signal: TX, pin_signal: TSI0_CH10/PTB17/LPSPI1_SIN/LPUART0_TX/TPM1_CLKIN/LPSPI2_PCS2/FXIO0_D17}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortB);                           /* Clock Gate Control: 0x01u */

  PORT_SetPinMux(PORTB, PIN16_IDX, kPORT_MuxAlt3);           /* PORTB16 (pin 62) is configured as LPUART0_RX */
  PORT_SetPinMux(PORTB, PIN17_IDX, kPORT_MuxAlt3);           /* PORTB17 (pin 63) is configured as LPUART0_TX */
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
