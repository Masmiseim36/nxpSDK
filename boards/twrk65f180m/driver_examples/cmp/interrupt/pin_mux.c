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
processor: MK65FN2M0xxx18
package_id: MK65FN2M0VMI18
mcu_data: ksdk2_0
processor_version: 0.0.10
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
  - {pin_num: H3, peripheral: UART2, signal: TX, pin_signal: ADC0_SE4a/PTE16/SPI0_PCS0/UART2_TX/FTM_CLKIN0/FTM0_FLT3/TPM_CLKIN0}
  - {pin_num: F5, peripheral: UART2, signal: RX, pin_signal: ADC0_SE5a/PTE17/LLWU_P19/SPI0_SCK/UART2_RX/FTM_CLKIN1/LPTMR0_ALT3/TPM_CLKIN1}
  - {pin_num: H12, peripheral: GPIOA, signal: 'GPIO, 28', pin_signal: PTA28/MII0_TXER/FB_A25}
  - {pin_num: L12, peripheral: CMP2, signal: 'IN, 0', pin_signal: CMP2_IN0/PTA12/CAN0_TX/FTM1_CH0/RMII0_RXD1/MII0_RXD1/I2C2_SCL/I2S0_TXD0/FTM1_QD_PHA/TPM1_CH0}
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
    /* Port A Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    /* PORTA12 (pin L12) is configured as CMP2_IN0 */
    PORT_SetPinMux(PORTA, 12U, kPORT_PinDisabledOrAnalog);

    /* PORTA28 (pin H12) is configured as PTA28 */
    PORT_SetPinMux(PORTA, 28U, kPORT_MuxAsGpio);

    /* PORTE16 (pin H3) is configured as UART2_TX */
    PORT_SetPinMux(PORTE, 16U, kPORT_MuxAlt3);

    /* PORTE17 (pin F5) is configured as UART2_RX */
    PORT_SetPinMux(PORTE, 17U, kPORT_MuxAlt3);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
