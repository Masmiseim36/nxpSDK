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
  - {pin_num: '44', peripheral: SPI0, signal: PCS0_SS, pin_signal: PTA14/SPI0_PCS0/LPUART0_TX/TRACE_D2/FXIO0_D20/I2S0_RX_BCLK/I2S0_TXD1}
  - {pin_num: '45', peripheral: SPI0, signal: SCK, pin_signal: PTA15/SPI0_SCK/LPUART0_RX/TRACE_D1/FXIO0_D21/I2S0_RXD0}
  - {pin_num: '46', peripheral: SPI0, signal: SOUT, pin_signal: PTA16/SPI0_SOUT/LPUART0_CTS_b/TRACE_D0/FXIO0_D22/I2S0_RX_FS/I2S0_RXD1}
  - {pin_num: '47', peripheral: SPI0, signal: SIN, pin_signal: PTA17/SPI0_SIN/LPUART0_RTS_b/FXIO0_D23/I2S0_MCLK}
  - {pin_num: '95', peripheral: SPI1, signal: PCS0_SS, pin_signal: PTD4/LLWU_P14/SPI0_PCS1/LPUART0_RTS_b/FTM0_CH4/FB_AD2/SDRAM_A10/EWM_IN/SPI1_PCS0}
  - {pin_num: '96', peripheral: SPI1, signal: SCK, pin_signal: ADC0_SE6b/PTD5/SPI0_PCS2/LPUART0_CTS_b/FTM0_CH5/FB_AD1/SDRAM_A9/EWM_OUT_b/SPI1_SCK}
  - {pin_num: '97', peripheral: SPI1, signal: SOUT, pin_signal: ADC0_SE7b/PTD6/LLWU_P15/SPI0_PCS3/LPUART0_RX/FTM0_CH6/FB_AD0/FTM0_FLT0/SPI1_SOUT}
  - {pin_num: '100', peripheral: SPI1, signal: SIN, pin_signal: PTD7/CMT_IRO/LPUART0_TX/FTM0_CH7/SDRAM_CKE/FTM0_FLT1/SPI1_SIN}
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
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);
    /* Port D Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);

    /* PORTA14 (pin 44) is configured as SPI0_PCS0 */
    PORT_SetPinMux(PORTA, 14U, kPORT_MuxAlt2);

    /* PORTA15 (pin 45) is configured as SPI0_SCK */
    PORT_SetPinMux(PORTA, 15U, kPORT_MuxAlt2);

    /* PORTA16 (pin 46) is configured as SPI0_SOUT */
    PORT_SetPinMux(PORTA, 16U, kPORT_MuxAlt2);

    /* PORTA17 (pin 47) is configured as SPI0_SIN */
    PORT_SetPinMux(PORTA, 17U, kPORT_MuxAlt2);

    /* PORTC14 (pin 86) is configured as LPUART4_RX */
    PORT_SetPinMux(PORTC, 14U, kPORT_MuxAlt3);

    /* PORTC15 (pin 87) is configured as LPUART4_TX */
    PORT_SetPinMux(PORTC, 15U, kPORT_MuxAlt3);

    /* PORTD4 (pin 95) is configured as SPI1_PCS0 */
    PORT_SetPinMux(PORTD, 4U, kPORT_MuxAlt7);

    /* PORTD5 (pin 96) is configured as SPI1_SCK */
    PORT_SetPinMux(PORTD, 5U, kPORT_MuxAlt7);

    /* PORTD6 (pin 97) is configured as SPI1_SOUT */
    PORT_SetPinMux(PORTD, 6U, kPORT_MuxAlt7);

    /* PORTD7 (pin 100) is configured as SPI1_SIN */
    PORT_SetPinMux(PORTD, 7U, kPORT_MuxAlt7);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
