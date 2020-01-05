/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v3.0
processor: MKL28Z512xxx7
package_id: MKL28Z512VLL7
mcu_data: ksdk2_0
processor_version: 0.0.8
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"



/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '35', peripheral: LPUART0, signal: RX, pin_signal: TSI0_CH2/PTA1/LPUART0_RX/TPM2_CH0}
  - {pin_num: '36', peripheral: LPUART0, signal: TX, pin_signal: TSI0_CH3/PTA2/LPUART0_TX/TPM2_CH1}
  - {pin_num: '70', peripheral: TSI0, signal: 'CH, 13', pin_signal: ADC0_SE14/TSI0_CH13/PTC0/LPSPI2_PCS1/USB_SOF_OUT/CMP0_OUT/I2S0_TXD0}
  - {pin_num: '72', peripheral: TSI0, signal: 'CH, 15', pin_signal: ADC0_SE11/CMP1_IN0/TSI0_CH15/PTC2/LPI2C1_SDA/LPUART1_CTS_b/TPM0_CH1/I2S0_TX_FS}
  - {pin_num: '91', peripheral: GPIOC, signal: 'GPIO, 17', pin_signal: PTC17/EMVSIM0_IO/LPSPI0_PCS3}
  - {pin_num: '92', peripheral: GPIOC, signal: 'GPIO, 18', pin_signal: PTC18/EMVSIM0_PD/LPSPI0_PCS2}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitPins(void)
{
    /* Clock Gate Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortA);
    /* Clock Gate Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortC);

    /* PORTA1 (pin 35) is configured as LPUART0_RX */
    PORT_SetPinMux(PORTA, 1U, kPORT_MuxAlt2);

    /* PORTA2 (pin 36) is configured as LPUART0_TX */
    PORT_SetPinMux(PORTA, 2U, kPORT_MuxAlt2);

    /* PORTC0 (pin 70) is configured as TSI0_CH13 */
    PORT_SetPinMux(PORTC, 0U, kPORT_PinDisabledOrAnalog);

    /* PORTC17 (pin 91) is configured as PTC17 */
    PORT_SetPinMux(PORTC, 17U, kPORT_MuxAsGpio);

    /* PORTC18 (pin 92) is configured as PTC18 */
    PORT_SetPinMux(PORTC, 18U, kPORT_MuxAsGpio);

    /* PORTC2 (pin 72) is configured as TSI0_CH15 */
    PORT_SetPinMux(PORTC, 2U, kPORT_PinDisabledOrAnalog);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
