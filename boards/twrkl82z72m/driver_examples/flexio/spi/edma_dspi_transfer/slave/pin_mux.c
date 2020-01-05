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
processor: MKL82Z128xxx7
package_id: MKL82Z128VMC7
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
  - {pin_num: B8, peripheral: LPUART1, signal: RX, pin_signal: PTC3/LLWU_P7/SPI0_PCS1/LPUART1_RX/TPM0_CH2/CLKOUT}
  - {pin_num: A8, peripheral: LPUART1, signal: TX, pin_signal: PTC4/LLWU_P8/SPI0_PCS0/LPUART1_TX/TPM0_CH3}
  - {pin_num: D10, peripheral: FLEXIO0, signal: 'D, 4', pin_signal: PTB10/SPI1_PCS0/FXIO0_D4}
  - {pin_num: C10, peripheral: FLEXIO0, signal: 'D, 5', pin_signal: PTB11/SPI1_SCK/FXIO0_D5}
  - {pin_num: G10, peripheral: FLEXIO0, signal: 'D, 1', pin_signal: ADC0_SE9/TSI0_CH6/PTB1/I2C0_SDA/TPM1_CH1/FXIO0_D1}
  - {pin_num: G11, peripheral: FLEXIO0, signal: 'D, 0', pin_signal: ADC0_SE8/TSI0_CH0/PTB0/LLWU_P5/I2C0_SCL/TPM1_CH0/FXIO0_D0}
  - {pin_num: C7, peripheral: SPI0, signal: SOUT, pin_signal: CMP0_IN0/PTC6/LLWU_P10/SPI0_SOUT/EXTRG_IN/FXIO0_D14}
  - {pin_num: B7, peripheral: SPI0, signal: SIN, pin_signal: CMP0_IN1/PTC7/SPI0_SIN/USB0_SOF_OUT/FXIO0_D15}
  - {pin_num: D4, peripheral: SPI0, signal: PCS0_SS, pin_signal: PTD0/LLWU_P12/SPI0_PCS0/LPUART2_RTS_b/FXIO0_D22}
  - {pin_num: D3, peripheral: SPI0, signal: SCK, pin_signal: ADC0_SE5b/PTD1/SPI0_SCK/LPUART2_CTS_b/FXIO0_D23}
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
    /* PTB Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);
    /* PTC Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);
    /* PTD Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);

    /* PORTB0 (pin G11) is configured as FXIO0_D0 */
    PORT_SetPinMux(PORTB, 0U, kPORT_MuxAlt7);

    /* PORTB1 (pin G10) is configured as FXIO0_D1 */
    PORT_SetPinMux(PORTB, 1U, kPORT_MuxAlt7);

    /* PORTB10 (pin D10) is configured as FXIO0_D4 */
    PORT_SetPinMux(PORTB, 10U, kPORT_MuxAlt7);

    /* PORTB11 (pin C10) is configured as FXIO0_D5 */
    PORT_SetPinMux(PORTB, 11U, kPORT_MuxAlt7);

    /* PORTC3 (pin B8) is configured as LPUART1_RX */
    PORT_SetPinMux(PORTC, 3U, kPORT_MuxAlt3);

    /* PORTC4 (pin A8) is configured as LPUART1_TX */
    PORT_SetPinMux(PORTC, 4U, kPORT_MuxAlt3);

    /* PORTC6 (pin C7) is configured as SPI0_SOUT */
    PORT_SetPinMux(PORTC, 6U, kPORT_MuxAlt2);

    /* PORTC7 (pin B7) is configured as SPI0_SIN */
    PORT_SetPinMux(PORTC, 7U, kPORT_MuxAlt2);

    /* PORTD0 (pin D4) is configured as SPI0_PCS0 */
    PORT_SetPinMux(PORTD, 0U, kPORT_MuxAlt2);

    /* PORTD1 (pin D3) is configured as SPI0_SCK */
    PORT_SetPinMux(PORTD, 1U, kPORT_MuxAlt2);

    SIM->SOPT5 = ((SIM->SOPT5 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT5_LPUART1TXSRC_MASK | SIM_SOPT5_LPUART1RXSRC_MASK)))

                  /* LPUART1 transmit data source select: LPUART1_TX pin. */
                  | SIM_SOPT5_LPUART1TXSRC(SOPT5_LPUART1TXSRC_LPUART_TX)

                  /* LPUART1 receive data source select: LPUART1_RX pin. */
                  | SIM_SOPT5_LPUART1RXSRC(SOPT5_LPUART1RXSRC_LPUART_RX));
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
