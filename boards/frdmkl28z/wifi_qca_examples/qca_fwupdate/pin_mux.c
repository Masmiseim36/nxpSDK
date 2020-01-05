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
pin_labels:
- {pin_num: '55', pin_signal: ADC0_SE12/TSI0_CH7/PTB2/LPI2C0_SCL/TPM2_CH0/LPUART0_RTS_b/FXIO0_D10, label: IRQ, identifier: IRQ}
- {pin_num: '60', pin_signal: PTB10/LPSPI1_PCS0/FXIO0_D14, label: PWRON, identifier: PWRON}
- {pin_num: '85', pin_signal: PTC13/LPI2C1_SDAS/TPM1_CLKIN, label: IRQ, identifier: IRQ}
- {pin_num: '93', pin_signal: PTD0/LLWU_P12/LPSPI0_PCS0/LPUART2_RTS_b/TPM0_CH0/FXIO0_D0, label: CS}
- {pin_num: '94', pin_signal: ADC0_SE5b/PTD1/LPSPI0_SCK/LPUART2_CTS_b/TPM0_CH1/FXIO0_D1, label: SCK}
- {pin_num: '95', pin_signal: PTD2/LLWU_P13/LPSPI0_SOUT/LPUART2_RX/TPM0_CH2/FXIO0_D2, label: MOSI}
- {pin_num: '96', pin_signal: PTD3/LPSPI0_SIN/LPUART2_TX/TPM0_CH3/FXIO0_D3, label: MISO}
- {pin_num: '99', pin_signal: ADC0_SE7b/PTD6/LLWU_P15/LPSPI1_SOUT/LPUART0_RX/FXIO0_D6, label: PWRON, identifier: PWRON}
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
  - {pin_num: '63', peripheral: LPUART0, signal: TX, pin_signal: TSI0_CH10/PTB17/LPSPI1_SIN/LPUART0_TX/TPM1_CLKIN/LPSPI2_PCS2/FXIO0_D17}
  - {pin_num: '62', peripheral: LPUART0, signal: RX, pin_signal: TSI0_CH9/PTB16/LPSPI1_SOUT/LPUART0_RX/TPM0_CLKIN/LPSPI2_PCS3/FXIO0_D16}
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
    CLOCK_EnableClock(kCLOCK_PortB);

    /* PORTB16 (pin 62) is configured as LPUART0_RX */
    PORT_SetPinMux(PORTB, 16U, kPORT_MuxAlt3);

    /* PORTB17 (pin 63) is configured as LPUART0_TX */
    PORT_SetPinMux(PORTB, 17U, kPORT_MuxAlt3);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitGT202Shield:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '93', peripheral: LPSPI0, signal: PCS0, pin_signal: PTD0/LLWU_P12/LPSPI0_PCS0/LPUART2_RTS_b/TPM0_CH0/FXIO0_D0}
  - {pin_num: '94', peripheral: LPSPI0, signal: SCK, pin_signal: ADC0_SE5b/PTD1/LPSPI0_SCK/LPUART2_CTS_b/TPM0_CH1/FXIO0_D1}
  - {pin_num: '95', peripheral: LPSPI0, signal: SOUT, pin_signal: PTD2/LLWU_P13/LPSPI0_SOUT/LPUART2_RX/TPM0_CH2/FXIO0_D2}
  - {pin_num: '96', peripheral: LPSPI0, signal: SIN, pin_signal: PTD3/LPSPI0_SIN/LPUART2_TX/TPM0_CH3/FXIO0_D3}
  - {pin_num: '85', peripheral: GPIOC, signal: 'GPIO, 13', pin_signal: PTC13/LPI2C1_SDAS/TPM1_CLKIN, direction: INPUT, pull_enable: enable}
  - {pin_num: '99', peripheral: GPIOD, signal: 'GPIO, 6', pin_signal: ADC0_SE7b/PTD6/LLWU_P15/LPSPI1_SOUT/LPUART0_RX/FXIO0_D6, direction: OUTPUT, pull_select: down,
    pull_enable: enable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitGT202Shield
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitGT202Shield(void)
{
    /* Clock Gate Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortC);
    /* Clock Gate Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortD);

    /* PORTC13 (pin 85) is configured as PTC13 */
    PORT_SetPinMux(BOARD_INITGT202SHIELD_IRQ_PORT, BOARD_INITGT202SHIELD_IRQ_PIN, kPORT_MuxAsGpio);

    PORTC->PCR[13] = ((PORTC->PCR[13] &
                       /* Mask bits to zero which are setting */
                       (~(PORT_PCR_PE_MASK | PORT_PCR_ISF_MASK)))

                      /* Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin. */
                      | (uint32_t)(PORT_PCR_PE_MASK));

    /* PORTD0 (pin 93) is configured as LPSPI0_PCS0 */
    PORT_SetPinMux(PORTD, 0U, kPORT_MuxAlt2);

    /* PORTD1 (pin 94) is configured as LPSPI0_SCK */
    PORT_SetPinMux(PORTD, 1U, kPORT_MuxAlt2);

    /* PORTD2 (pin 95) is configured as LPSPI0_SOUT */
    PORT_SetPinMux(PORTD, 2U, kPORT_MuxAlt2);

    /* PORTD3 (pin 96) is configured as LPSPI0_SIN */
    PORT_SetPinMux(PORTD, 3U, kPORT_MuxAlt2);

    /* PORTD6 (pin 99) is configured as PTD6 */
    PORT_SetPinMux(BOARD_INITGT202SHIELD_PWRON_PORT, BOARD_INITGT202SHIELD_PWRON_PIN, kPORT_MuxAsGpio);

    PORTD->PCR[6] = ((PORTD->PCR[6] &
                      /* Mask bits to zero which are setting */
                      (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ISF_MASK)))

                     /* Pull Select: Internal pulldown resistor is enabled on the corresponding pin, if the
                      * corresponding PE field is set. */
                     | (uint32_t)(kPORT_PullDown));
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitSilex2401Shield:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '96', peripheral: LPSPI0, signal: SIN, pin_signal: PTD3/LPSPI0_SIN/LPUART2_TX/TPM0_CH3/FXIO0_D3}
  - {pin_num: '95', peripheral: LPSPI0, signal: SOUT, pin_signal: PTD2/LLWU_P13/LPSPI0_SOUT/LPUART2_RX/TPM0_CH2/FXIO0_D2}
  - {pin_num: '94', peripheral: LPSPI0, signal: SCK, pin_signal: ADC0_SE5b/PTD1/LPSPI0_SCK/LPUART2_CTS_b/TPM0_CH1/FXIO0_D1}
  - {pin_num: '93', peripheral: LPSPI0, signal: PCS0, pin_signal: PTD0/LLWU_P12/LPSPI0_PCS0/LPUART2_RTS_b/TPM0_CH0/FXIO0_D0}
  - {pin_num: '55', peripheral: GPIOB, signal: 'GPIO, 2', pin_signal: ADC0_SE12/TSI0_CH7/PTB2/LPI2C0_SCL/TPM2_CH0/LPUART0_RTS_b/FXIO0_D10, direction: INPUT, pull_enable: enable}
  - {pin_num: '60', peripheral: GPIOB, signal: 'GPIO, 10', pin_signal: PTB10/LPSPI1_PCS0/FXIO0_D14, direction: OUTPUT, pull_select: down, pull_enable: enable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitSilex2401Shield
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitSilex2401Shield(void)
{
    /* Clock Gate Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortB);
    /* Clock Gate Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortD);

    /* PORTB10 (pin 60) is configured as PTB10 */
    PORT_SetPinMux(BOARD_INITSILEX2401SHIELD_PWRON_PORT, BOARD_INITSILEX2401SHIELD_PWRON_PIN, kPORT_MuxAsGpio);

    PORTB->PCR[10] = ((PORTB->PCR[10] &
                       /* Mask bits to zero which are setting */
                       (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ISF_MASK)))

                      /* Pull Select: Internal pulldown resistor is enabled on the corresponding pin, if the
                       * corresponding PE field is set. */
                      | (uint32_t)(kPORT_PullDown));

    /* PORTB2 (pin 55) is configured as PTB2 */
    PORT_SetPinMux(BOARD_INITSILEX2401SHIELD_IRQ_PORT, BOARD_INITSILEX2401SHIELD_IRQ_PIN, kPORT_MuxAsGpio);

    PORTB->PCR[2] = ((PORTB->PCR[2] &
                      /* Mask bits to zero which are setting */
                      (~(PORT_PCR_PE_MASK | PORT_PCR_ISF_MASK)))

                     /* Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin. */
                     | (uint32_t)(PORT_PCR_PE_MASK));

    /* PORTD0 (pin 93) is configured as LPSPI0_PCS0 */
    PORT_SetPinMux(PORTD, 0U, kPORT_MuxAlt2);

    /* PORTD1 (pin 94) is configured as LPSPI0_SCK */
    PORT_SetPinMux(PORTD, 1U, kPORT_MuxAlt2);

    /* PORTD2 (pin 95) is configured as LPSPI0_SOUT */
    PORT_SetPinMux(PORTD, 2U, kPORT_MuxAlt2);

    /* PORTD3 (pin 96) is configured as LPSPI0_SIN */
    PORT_SetPinMux(PORTD, 3U, kPORT_MuxAlt2);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
