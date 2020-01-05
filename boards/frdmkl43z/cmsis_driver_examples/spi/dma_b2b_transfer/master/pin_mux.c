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
processor: MKL43Z256xxx4
package_id: MKL43Z256VLH4
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
- options: {callFromInitBoot: 'true', prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '23', peripheral: LPUART0, signal: RX, pin_signal: PTA1/LPUART0_RX/TPM2_CH0}
  - {pin_num: '24', peripheral: LPUART0, signal: TX, pin_signal: PTA2/LPUART0_TX/TPM2_CH1}
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

    /* PORTA1 (pin 23) is configured as LPUART0_RX */
    PORT_SetPinMux(PORTA, 1U, kPORT_MuxAlt2);

    /* PORTA2 (pin 24) is configured as LPUART0_TX */
    PORT_SetPinMux(PORTA, 2U, kPORT_MuxAlt2);

    SIM->SOPT5 = ((SIM->SOPT5 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT5_LPUART0TXSRC_MASK | SIM_SOPT5_LPUART0RXSRC_MASK)))

                  /* LPUART0 Transmit Data Source Select: LPUART0_TX pin. */
                  | SIM_SOPT5_LPUART0TXSRC(SOPT5_LPUART0TXSRC_LPUART_TX)

                  /* LPUART0 Receive Data Source Select: LPUART_RX pin. */
                  | SIM_SOPT5_LPUART0RXSRC(SOPT5_LPUART0RXSRC_LPUART_RX));
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
SPI0_InitPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '53', peripheral: SPI0, signal: PCS0, pin_signal: LCD_P24/PTC4/LLWU_P8/SPI0_SS/LPUART1_TX/TPM0_CH3/I2S0_MCLK}
  - {pin_num: '54', peripheral: SPI0, signal: SCK, pin_signal: LCD_P25/PTC5/LLWU_P9/SPI0_SCK/LPTMR0_ALT2/I2S0_RXD0/CMP0_OUT}
  - {pin_num: '55', peripheral: SPI0, signal: MOSI, pin_signal: LCD_P26/CMP0_IN0/PTC6/LLWU_P10/SPI0_MOSI/EXTRG_IN/I2S0_RX_BCLK/SPI0_MISO/I2S0_MCLK}
  - {pin_num: '56', peripheral: SPI0, signal: MISO, pin_signal: LCD_P27/CMP0_IN1/PTC7/SPI0_MISO/USB_SOF_OUT/I2S0_RX_FS/SPI0_MOSI}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : SPI0_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void SPI0_InitPins(void)
{
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    /* PORTC4 (pin 53) is configured as SPI0_SS */
    PORT_SetPinMux(PORTC, 4U, kPORT_MuxAlt2);

    /* PORTC5 (pin 54) is configured as SPI0_SCK */
    PORT_SetPinMux(PORTC, 5U, kPORT_MuxAlt2);

    /* PORTC6 (pin 55) is configured as SPI0_MOSI */
    PORT_SetPinMux(PORTC, 6U, kPORT_MuxAlt2);

    /* PORTC7 (pin 56) is configured as SPI0_MISO */
    PORT_SetPinMux(PORTC, 7U, kPORT_MuxAlt2);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
SPI0_DeinitPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '53', peripheral: LCD, signal: 'P, 24', pin_signal: LCD_P24/PTC4/LLWU_P8/SPI0_SS/LPUART1_TX/TPM0_CH3/I2S0_MCLK}
  - {pin_num: '54', peripheral: LCD, signal: 'P, 25', pin_signal: LCD_P25/PTC5/LLWU_P9/SPI0_SCK/LPTMR0_ALT2/I2S0_RXD0/CMP0_OUT}
  - {pin_num: '55', peripheral: LCD, signal: 'P, 26', pin_signal: LCD_P26/CMP0_IN0/PTC6/LLWU_P10/SPI0_MOSI/EXTRG_IN/I2S0_RX_BCLK/SPI0_MISO/I2S0_MCLK}
  - {pin_num: '56', peripheral: LCD, signal: 'P, 27', pin_signal: LCD_P27/CMP0_IN1/PTC7/SPI0_MISO/USB_SOF_OUT/I2S0_RX_FS/SPI0_MOSI}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : SPI0_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void SPI0_DeinitPins(void)
{
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    /* PORTC4 (pin 53) is configured as LCD_P24 */
    PORT_SetPinMux(PORTC, 4U, kPORT_PinDisabledOrAnalog);

    /* PORTC5 (pin 54) is configured as LCD_P25 */
    PORT_SetPinMux(PORTC, 5U, kPORT_PinDisabledOrAnalog);

    /* PORTC6 (pin 55) is configured as LCD_P26 */
    PORT_SetPinMux(PORTC, 6U, kPORT_PinDisabledOrAnalog);

    /* PORTC7 (pin 56) is configured as LCD_P27 */
    PORT_SetPinMux(PORTC, 7U, kPORT_PinDisabledOrAnalog);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
