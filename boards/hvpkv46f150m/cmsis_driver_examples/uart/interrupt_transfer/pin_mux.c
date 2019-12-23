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
processor: MKV46F256xxx16
package_id: MKV46F256VLL16
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
UART0_InitPins:
- options: {callFromInitBoot: 'false', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '17', peripheral: UART0, signal: RX, pin_signal: ADCA_CH7b/PTE21/FTM1_CH1/UART0_RX}
  - {pin_num: '16', peripheral: UART0, signal: TX, pin_signal: ADCA_CH6b/PTE20/FTM1_CH0/UART0_TX}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : UART0_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void UART0_InitPins(void)
{
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    /* PORTE20 (pin 16) is configured as UART0_TX */
    PORT_SetPinMux(PORTE, 20U, kPORT_MuxAlt4);

    /* PORTE21 (pin 17) is configured as UART0_RX */
    PORT_SetPinMux(PORTE, 21U, kPORT_MuxAlt4);

    SIM->SOPT5 = ((SIM->SOPT5 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT5_UART0TXSRC_MASK | SIM_SOPT5_UART0RXSRC_MASK)))

                  /* UART 0 transmit data source select: UART0_TX pin. */
                  | SIM_SOPT5_UART0TXSRC(SOPT5_UART0TXSRC_UART_TX)

                  /* UART 0 receive data source select: UART0_RX pin. */
                  | SIM_SOPT5_UART0RXSRC(SOPT5_UART0RXSRC_UART_RX));
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
UART0_DeinitPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '16', peripheral: ADC, signal: 'ADCA, 6', pin_signal: ADCA_CH6b/PTE20/FTM1_CH0/UART0_TX}
  - {pin_num: '17', peripheral: ADC, signal: 'ADCA, 7', pin_signal: ADCA_CH7b/PTE21/FTM1_CH1/UART0_RX}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : UART0_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void UART0_DeinitPins(void)
{
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    /* PORTE20 (pin 16) is configured as ADCA_CH6b */
    PORT_SetPinMux(PORTE, 20U, kPORT_PinDisabledOrAnalog);

    /* PORTE21 (pin 17) is configured as ADCA_CH7b */
    PORT_SetPinMux(PORTE, 21U, kPORT_PinDisabledOrAnalog);

    SIM->ADCOPT = ((SIM->ADCOPT &
                    /* Mask bits to zero which are setting */
                    (~(SIM_ADCOPT_ADCACH6SEL_MASK | SIM_ADCOPT_ADCACH7SEL_MASK)))

                   /* ADCA MUX0 selection for ADCA channel 6: ADCA MUX0's channel b. */
                   | SIM_ADCOPT_ADCACH6SEL(ADCOPT_ADCACH6SEL_CH_B)

                   /* ADCA MUX1 selection for ADCA channel 7: ADCA MUX1's channel b. */
                   | SIM_ADCOPT_ADCACH7SEL(ADCOPT_ADCACH7SEL_CH_B));
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
UART1_InitPins:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '73', peripheral: UART1, signal: RX, pin_signal: CMP1_IN1/PTC3/LLWU_P7/SPI0_PCS1/UART1_RX/FTM0_CH2/CLKOUT/FTM3_FLT0}
  - {pin_num: '76', peripheral: UART1, signal: TX, pin_signal: PTC4/LLWU_P8/SPI0_PCS0/UART1_TX/FTM0_CH3/CMP1_OUT}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : UART1_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void UART1_InitPins(void)
{
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    /* PORTC3 (pin 73) is configured as UART1_RX */
    PORT_SetPinMux(PORTC, 3U, kPORT_MuxAlt3);

    /* PORTC4 (pin 76) is configured as UART1_TX */
    PORT_SetPinMux(PORTC, 4U, kPORT_MuxAlt3);

    SIM->SOPT5 = ((SIM->SOPT5 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT5_UART1TXSRC_MASK | SIM_SOPT5_UART1RXSRC_MASK)))

                  /* UART 1 transmit data source select: UART1_TX pin. */
                  | SIM_SOPT5_UART1TXSRC(SOPT5_UART1TXSRC_UART_TX)

                  /* UART 1 receive data source select: UART1_RX pin. */
                  | SIM_SOPT5_UART1RXSRC(SOPT5_UART1RXSRC_UART_RX));
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
UART1_DeinitPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '73', peripheral: CMP1, signal: 'IN, 1', pin_signal: CMP1_IN1/PTC3/LLWU_P7/SPI0_PCS1/UART1_RX/FTM0_CH2/CLKOUT/FTM3_FLT0}
  - {pin_num: '76', peripheral: CMP1, signal: OUT, pin_signal: PTC4/LLWU_P8/SPI0_PCS0/UART1_TX/FTM0_CH3/CMP1_OUT}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : UART1_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void UART1_DeinitPins(void)
{
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    /* PORTC3 (pin 73) is configured as CMP1_IN1 */
    PORT_SetPinMux(PORTC, 3U, kPORT_PinDisabledOrAnalog);

    /* PORTC4 (pin 76) is configured as CMP1_OUT */
    PORT_SetPinMux(PORTC, 4U, kPORT_MuxAlt6);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
