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
processor: MK82FN256xxx15
package_id: MK82FN256VLL15
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
  - {pin_num: '86', peripheral: LPUART4, signal: RX, pin_signal: PTC14/LPUART4_RX/FB_AD25/SDRAM_D25/FXIO0_D20}
  - {pin_num: '87', peripheral: LPUART4, signal: TX, pin_signal: PTC15/LPUART4_TX/FB_AD24/SDRAM_D24/FXIO0_D21}
  - {pin_num: '58', peripheral: FLEXIO0, signal: 'D, 4', pin_signal: PTB10/SPI1_PCS0/LPUART3_RX/I2C2_SCL/FB_AD19/SDRAM_D19/FTM0_FLT1/FXIO0_D4}
  - {pin_num: '59', peripheral: FLEXIO0, signal: 'D, 5', pin_signal: PTB11/SPI1_SCK/LPUART3_TX/I2C2_SDA/FB_AD18/SDRAM_D18/FTM0_FLT2/FXIO0_D5}
  - {pin_num: '37', peripheral: I2C3, signal: SDA, pin_signal: TSI0_CH2/PTA1/LPUART0_RX/FTM0_CH6/I2C3_SDA/FXIO0_D11/EMVSIM0_IO/JTAG_TDI}
  - {pin_num: '38', peripheral: I2C3, signal: SCL, pin_signal: TSI0_CH3/PTA2/LPUART0_TX/FTM0_CH7/I2C3_SCL/FXIO0_D12/EMVSIM0_PD/JTAG_TDO/TRACE_SWO}
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
    /* Port B Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    /* PORTA1 (pin 37) is configured as I2C3_SDA */
    PORT_SetPinMux(PORTA, 1U, kPORT_MuxAlt4);

    /* PORTA2 (pin 38) is configured as I2C3_SCL */
    PORT_SetPinMux(PORTA, 2U, kPORT_MuxAlt4);

    /* PORTB10 (pin 58) is configured as FXIO0_D4 */
    PORT_SetPinMux(PORTB, 10U, kPORT_MuxAlt7);

    /* PORTB11 (pin 59) is configured as FXIO0_D5 */
    PORT_SetPinMux(PORTB, 11U, kPORT_MuxAlt7);

    /* PORTC14 (pin 86) is configured as LPUART4_RX */
    PORT_SetPinMux(PORTC, 14U, kPORT_MuxAlt3);

    /* PORTC15 (pin 87) is configured as LPUART4_TX */
    PORT_SetPinMux(PORTC, 15U, kPORT_MuxAlt3);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
