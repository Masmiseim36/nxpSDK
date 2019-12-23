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
processor: MKV11Z128xxx7
package_id: MKV11Z128VLH7
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
  - {pin_num: '39', peripheral: UART0, signal: RX, pin_signal: PTB16/UART0_RX/FTM_CLKIN2/CAN0_TX/EWM_IN}
  - {pin_num: '40', peripheral: UART0, signal: TX, pin_signal: PTB17/UART0_TX/FTM_CLKIN1/CAN0_RX/EWM_OUT_b}
  - {pin_num: '26', peripheral: n/a, signal: disabled, pin_signal: PTA4/LLWU_P3/FTM0_CH1/FTM4_FLT0/FTM0_FLT3/NMI_b}
  - {pin_num: '9', peripheral: ADC0, signal: 'SE, 0', pin_signal: ADC0_SE0/ADC0_DP0/PTE20/FTM1_CH0/UART0_TX}
  - {pin_num: '21', peripheral: GPIOE, signal: 'GPIO, 25', pin_signal: PTE25/LLWU_P21/CAN0_RX/FTM0_CH1/I2C0_SDA/EWM_IN}
  - {pin_num: '61', peripheral: GPIOD, signal: 'GPIO, 4', pin_signal: PTD4/LLWU_P14/SPI0_PCS1/UART0_RTS_b/FTM0_CH4/FTM2_CH0/EWM_IN/SPI0_PCS0}
  - {pin_num: '62', peripheral: GPIOD, signal: 'GPIO, 5', pin_signal: ADC0_SE3/PTD5/SPI0_PCS2/UART0_CTS_b/FTM0_CH5/FTM2_CH1/EWM_OUT_b/SPI0_SCK}
  - {pin_num: '63', peripheral: GPIOD, signal: 'GPIO, 6', pin_signal: ADC1_SE6/PTD6/LLWU_P15/FTM4_CH0/UART0_RX/FTM0_CH0/FTM1_CH0/FTM0_FLT0/SPI0_SOUT}
  - {pin_num: '64', peripheral: GPIOD, signal: 'GPIO, 7', pin_signal: PTD7/FTM4_CH1/UART0_TX/FTM0_CH1/FTM1_CH1/FTM0_FLT1/SPI0_SIN}
  - {pin_num: '44', peripheral: GPIOC, signal: 'GPIO, 1', pin_signal: ADC1_SE3/PTC1/LLWU_P6/SPI0_PCS3/UART1_RTS_b/FTM0_CH0/FTM2_CH0}
  - {pin_num: '45', peripheral: GPIOC, signal: 'GPIO, 2', pin_signal: ADC0_SE11/CMP1_IN0/PTC2/SPI0_PCS2/UART1_CTS_b/FTM0_CH1/FTM2_CH1}
  - {pin_num: '46', peripheral: GPIOC, signal: 'GPIO, 3', pin_signal: CMP1_IN1/PTC3/LLWU_P7/SPI0_PCS1/UART1_RX/FTM0_CH2/CLKOUT/FTM3_FLT0}
  - {pin_num: '49', peripheral: GPIOC, signal: 'GPIO, 4', pin_signal: PTC4/LLWU_P8/SPI0_PCS0/UART1_TX/FTM0_CH3/CMP1_OUT}
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
    /* Port D Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    /* PORTA4 (pin 26) is disabled */
    PORT_SetPinMux(PORTA, 4U, kPORT_PinDisabledOrAnalog);

    /* PORTB16 (pin 39) is configured as UART0_RX */
    PORT_SetPinMux(PORTB, 16U, kPORT_MuxAlt3);

    /* PORTB17 (pin 40) is configured as UART0_TX */
    PORT_SetPinMux(PORTB, 17U, kPORT_MuxAlt3);

    /* PORTC1 (pin 44) is configured as PTC1 */
    PORT_SetPinMux(PORTC, 1U, kPORT_MuxAsGpio);

    /* PORTC2 (pin 45) is configured as PTC2 */
    PORT_SetPinMux(PORTC, 2U, kPORT_MuxAsGpio);

    /* PORTC3 (pin 46) is configured as PTC3 */
    PORT_SetPinMux(PORTC, 3U, kPORT_MuxAsGpio);

    /* PORTC4 (pin 49) is configured as PTC4 */
    PORT_SetPinMux(PORTC, 4U, kPORT_MuxAsGpio);

    /* PORTD4 (pin 61) is configured as PTD4 */
    PORT_SetPinMux(PORTD, 4U, kPORT_MuxAsGpio);

    /* PORTD5 (pin 62) is configured as PTD5 */
    PORT_SetPinMux(PORTD, 5U, kPORT_MuxAsGpio);

    /* PORTD6 (pin 63) is configured as PTD6 */
    PORT_SetPinMux(PORTD, 6U, kPORT_MuxAsGpio);

    /* PORTD7 (pin 64) is configured as PTD7 */
    PORT_SetPinMux(PORTD, 7U, kPORT_MuxAsGpio);

    /* PORTE20 (pin 9) is configured as ADC0_SE0 */
    PORT_SetPinMux(PORTE, 20U, kPORT_PinDisabledOrAnalog);

    /* PORTE25 (pin 21) is configured as PTE25 */
    PORT_SetPinMux(PORTE, 25U, kPORT_MuxAsGpio);

    SIM->SOPT5 = ((SIM->SOPT5 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT5_UART0TXSRC_MASK | SIM_SOPT5_UART0RXSRC_MASK)))

                  /* UART 0 Transmit Data Source Select: UART0_TX pin. */
                  | SIM_SOPT5_UART0TXSRC(SOPT5_UART0TXSRC_UART_TX)

                  /* UART 0 Receive Data Source Select: UART0_RX pin. */
                  | SIM_SOPT5_UART0RXSRC(SOPT5_UART0RXSRC_UART_RX));
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
