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
  - {pin_num: '61', peripheral: FTM0, signal: 'CH, 4', pin_signal: PTD4/LLWU_P14/SPI0_PCS1/UART0_RTS_b/FTM0_CH4/FTM2_CH0/EWM_IN/SPI0_PCS0, direction: OUTPUT}
  - {pin_num: '62', peripheral: FTM0, signal: 'CH, 5', pin_signal: ADC0_SE3/PTD5/SPI0_PCS2/UART0_CTS_b/FTM0_CH5/FTM2_CH1/EWM_OUT_b/SPI0_SCK, direction: OUTPUT}
  - {pin_num: '20', peripheral: FTM0, signal: 'CH, 0', pin_signal: PTE24/CAN0_TX/FTM0_CH0/I2C0_SCL/EWM_OUT_b, direction: OUTPUT}
  - {pin_num: '21', peripheral: FTM0, signal: 'CH, 1', pin_signal: PTE25/LLWU_P21/CAN0_RX/FTM0_CH1/I2C0_SDA/EWM_IN, direction: OUTPUT}
  - {pin_num: '17', peripheral: FTM0, signal: 'CH, 2', pin_signal: CMP1_IN5/CMP0_IN5/PTE29/FTM0_CH2/FTM_CLKIN0, direction: OUTPUT}
  - {pin_num: '18', peripheral: FTM0, signal: 'CH, 3', pin_signal: ADC1_SE4/CMP1_IN4/DAC0_OUT/PTE30/FTM0_CH3/FTM_CLKIN1, direction: OUTPUT}
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
    /* Port B Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);
    /* Port D Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    /* PORTB16 (pin 39) is configured as UART0_RX */
    PORT_SetPinMux(PORTB, 16U, kPORT_MuxAlt3);

    /* PORTB17 (pin 40) is configured as UART0_TX */
    PORT_SetPinMux(PORTB, 17U, kPORT_MuxAlt3);

    /* PORTD4 (pin 61) is configured as FTM0_CH4 */
    PORT_SetPinMux(PORTD, 4U, kPORT_MuxAlt4);

    /* PORTD5 (pin 62) is configured as FTM0_CH5 */
    PORT_SetPinMux(PORTD, 5U, kPORT_MuxAlt4);

    /* PORTE24 (pin 20) is configured as FTM0_CH0 */
    PORT_SetPinMux(PORTE, 24U, kPORT_MuxAlt3);

    /* PORTE25 (pin 21) is configured as FTM0_CH1 */
    PORT_SetPinMux(PORTE, 25U, kPORT_MuxAlt3);

    /* PORTE29 (pin 17) is configured as FTM0_CH2 */
    PORT_SetPinMux(PORTE, 29U, kPORT_MuxAlt3);

    /* PORTE30 (pin 18) is configured as FTM0_CH3 */
    PORT_SetPinMux(PORTE, 30U, kPORT_MuxAlt3);

    SIM->SOPT5 = ((SIM->SOPT5 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT5_UART0TXSRC_MASK | SIM_SOPT5_UART0RXSRC_MASK)))

                  /* UART 0 Transmit Data Source Select: UART0_TX pin. */
                  | SIM_SOPT5_UART0TXSRC(SOPT5_UART0TXSRC_UART_TX)

                  /* UART 0 Receive Data Source Select: UART0_RX pin. */
                  | SIM_SOPT5_UART0RXSRC(SOPT5_UART0RXSRC_UART_RX));

    SIM->SOPT8 =
        ((SIM->SOPT8 &
          /* Mask bits to zero which are setting */
          (~(SIM_SOPT8_FTM0OCH0SRC_MASK | SIM_SOPT8_FTM0OCH1SRC_MASK | SIM_SOPT8_FTM0OCH2SRC_MASK | SIM_SOPT8_FTM0OCH3SRC_MASK | SIM_SOPT8_FTM0OCH4SRC_MASK | SIM_SOPT8_FTM0OCH5SRC_MASK)))

         /* FTM0 channel 0 output PWM/OCMP mode source select: FTM0CH0 pin is the output of FTM0 channel 0
          * PWM/OCMP. */
         | SIM_SOPT8_FTM0OCH0SRC(SOPT8_FTM0OCH0SRC_NO_MODULATION)

         /* FTM0 channel 1 output PWM/OCMP mode source select: FTM0CH1 pin is the output of FTM0 channel 1
          * PWM/OCMP. */
         | SIM_SOPT8_FTM0OCH1SRC(SOPT8_FTM0OCH1SRC_NO_MODULATION)

         /* FTM0 channel 2 output PWM/OCMP mode source select: FTM0CH2 pin is the output of FTM0 channel 2
          * PWM/OCMP. */
         | SIM_SOPT8_FTM0OCH2SRC(SOPT8_FTM0OCH2SRC_NO_MODULATION)

         /* FTM0 channel 3 output PWM/OCMP mode source select: FTM0CH3 pin is the output of FTM0 channel 3
          * PWM/OCMP. */
         | SIM_SOPT8_FTM0OCH3SRC(SOPT8_FTM0OCH3SRC_NO_MODULATION)

         /* FTM0 channel 4 output PWM/OCMP mode source select: FTM0CH4 pin is the output of FTM0 channel 4
          * PWM/OCMP. */
         | SIM_SOPT8_FTM0OCH4SRC(SOPT8_FTM0OCH4SRC_NO_MODULATION)

         /* FTM0 channel 5 output PWM/OCMP mode source select: FTM0CH5 pin is the output of FTM0 channel 5
          * PWM/OCMP. */
         | SIM_SOPT8_FTM0OCH5SRC(SOPT8_FTM0OCH5SRC_NO_MODULATION));
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
