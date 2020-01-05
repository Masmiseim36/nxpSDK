/*
 * The Clear BSD License
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v3.0
processor: MKL46Z256xxx4
package_id: MKL46Z256VLL4
mcu_data: ksdk2_0
processor_version: 0.0.9
pin_labels:
- {pin_num: '96', pin_signal: LCD_P43/PTD3/SPI0_MISO/UART2_TX/TPM0_CH3/SPI0_MOSI/LCD_P43_Fault, label: IRQ, identifier: IRQ}
- {pin_num: '38', pin_signal: TSI0_CH5/PTA4/I2C1_SDA/TPM0_CH1/NMI_b, label: PWRON, identifier: PWRON}
- {pin_num: '85', pin_signal: LCD_P29/CMP0_IN3/PTC9/I2C0_SDA/TPM0_CH5/I2S0_RX_BCLK/LCD_P29_Fault, label: IRQ, identifier: IRQ}
- {pin_num: '97', pin_signal: LCD_P44/PTD4/LLWU_P14/SPI1_PCS0/UART2_RX/TPM0_CH4/LCD_P44_Fault, label: CS}
- {pin_num: '99', pin_signal: LCD_P46/ADC0_SE7b/PTD6/LLWU_P15/SPI1_MOSI/UART0_RX/SPI1_MISO/LCD_P46_Fault, label: MOSI}
- {pin_num: '100', pin_signal: LCD_P47/PTD7/SPI1_MISO/UART0_TX/SPI1_MOSI/LCD_P47_Fault, label: MISO}
- {pin_num: '98', pin_signal: LCD_P45/ADC0_SE6b/PTD5/SPI1_SCK/UART2_TX/TPM0_CH5/LCD_P45_Fault, label: SCK, identifier: LED_GREEN}
- {pin_num: '43', pin_signal: PTA13/TPM1_CH1/I2S0_TX_FS, label: PWRON, identifier: PWRON}
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
  - {pin_num: '36', peripheral: UART0, signal: TX, pin_signal: TSI0_CH3/PTA2/UART0_TX/TPM2_CH1}
  - {pin_num: '35', peripheral: UART0, signal: RX, pin_signal: TSI0_CH2/PTA1/UART0_RX/TPM2_CH0, pull_enable: enable}
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

    /* PORTA1 (pin 35) is configured as UART0_RX */
    PORT_SetPinMux(PORTA, 1U, kPORT_MuxAlt2);

    PORTA->PCR[1] = ((PORTA->PCR[1] &
                      /* Mask bits to zero which are setting */
                      (~(PORT_PCR_PE_MASK | PORT_PCR_ISF_MASK)))

                     /* Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin. */
                     | (uint32_t)(PORT_PCR_PE_MASK));

    /* PORTA2 (pin 36) is configured as UART0_TX */
    PORT_SetPinMux(PORTA, 2U, kPORT_MuxAlt2);

    SIM->SOPT5 = ((SIM->SOPT5 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT5_UART0TXSRC_MASK | SIM_SOPT5_UART0RXSRC_MASK)))

                  /* UART0 Transmit Data Source Select: UART0_TX pin. */
                  | SIM_SOPT5_UART0TXSRC(SOPT5_UART0TXSRC_UART_TX)

                  /* UART0 Receive Data Source Select: UART_RX pin. */
                  | SIM_SOPT5_UART0RXSRC(SOPT5_UART0RXSRC_UART_RX));
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitGT202Shield:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '97', peripheral: SPI1, signal: PCS0, pin_signal: LCD_P44/PTD4/LLWU_P14/SPI1_PCS0/UART2_RX/TPM0_CH4/LCD_P44_Fault}
  - {pin_num: '98', peripheral: SPI1, signal: SCK, pin_signal: LCD_P45/ADC0_SE6b/PTD5/SPI1_SCK/UART2_TX/TPM0_CH5/LCD_P45_Fault, identifier: ''}
  - {pin_num: '99', peripheral: SPI1, signal: MOSI, pin_signal: LCD_P46/ADC0_SE7b/PTD6/LLWU_P15/SPI1_MOSI/UART0_RX/SPI1_MISO/LCD_P46_Fault}
  - {pin_num: '100', peripheral: SPI1, signal: MISO, pin_signal: LCD_P47/PTD7/SPI1_MISO/UART0_TX/SPI1_MOSI/LCD_P47_Fault}
  - {pin_num: '85', peripheral: GPIOC, signal: 'GPIO, 9', pin_signal: LCD_P29/CMP0_IN3/PTC9/I2C0_SDA/TPM0_CH5/I2S0_RX_BCLK/LCD_P29_Fault, direction: INPUT, pull_enable: enable}
  - {pin_num: '43', peripheral: GPIOA, signal: 'GPIO, 13', pin_signal: PTA13/TPM1_CH1/I2S0_TX_FS, direction: OUTPUT, pull_select: down, pull_enable: enable}
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
    /* Port A Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);
    /* Port D Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);

    /* PORTA13 (pin 43) is configured as PTA13 */
    PORT_SetPinMux(BOARD_INITGT202SHIELD_PWRON_PORT, BOARD_INITGT202SHIELD_PWRON_PIN, kPORT_MuxAsGpio);

    PORTA->PCR[13] = ((PORTA->PCR[13] &
                       /* Mask bits to zero which are setting */
                       (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ISF_MASK)))

                      /* Pull Select: Internal pulldown resistor is enabled on the corresponding pin, if the
                       * corresponding PE field is set. */
                      | (uint32_t)(kPORT_PullDown));

    /* PORTC9 (pin 85) is configured as PTC9 */
    PORT_SetPinMux(BOARD_INITGT202SHIELD_IRQ_PORT, BOARD_INITGT202SHIELD_IRQ_PIN, kPORT_MuxAsGpio);

    PORTC->PCR[9] = ((PORTC->PCR[9] &
                      /* Mask bits to zero which are setting */
                      (~(PORT_PCR_PE_MASK | PORT_PCR_ISF_MASK)))

                     /* Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin. */
                     | (uint32_t)(PORT_PCR_PE_MASK));

    /* PORTD4 (pin 97) is configured as SPI1_PCS0 */
    PORT_SetPinMux(PORTD, 4U, kPORT_MuxAlt2);

    /* PORTD5 (pin 98) is configured as SPI1_SCK */
    PORT_SetPinMux(PORTD, 5U, kPORT_MuxAlt2);

    /* PORTD6 (pin 99) is configured as SPI1_MOSI */
    PORT_SetPinMux(PORTD, 6U, kPORT_MuxAlt2);

    /* PORTD7 (pin 100) is configured as SPI1_MISO */
    PORT_SetPinMux(PORTD, 7U, kPORT_MuxAlt2);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitSilex2401Shield:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '100', peripheral: SPI1, signal: MISO, pin_signal: LCD_P47/PTD7/SPI1_MISO/UART0_TX/SPI1_MOSI/LCD_P47_Fault}
  - {pin_num: '99', peripheral: SPI1, signal: MOSI, pin_signal: LCD_P46/ADC0_SE7b/PTD6/LLWU_P15/SPI1_MOSI/UART0_RX/SPI1_MISO/LCD_P46_Fault}
  - {pin_num: '98', peripheral: SPI1, signal: SCK, pin_signal: LCD_P45/ADC0_SE6b/PTD5/SPI1_SCK/UART2_TX/TPM0_CH5/LCD_P45_Fault, identifier: ''}
  - {pin_num: '97', peripheral: SPI1, signal: PCS0, pin_signal: LCD_P44/PTD4/LLWU_P14/SPI1_PCS0/UART2_RX/TPM0_CH4/LCD_P44_Fault}
  - {pin_num: '96', peripheral: GPIOD, signal: 'GPIO, 3', pin_signal: LCD_P43/PTD3/SPI0_MISO/UART2_TX/TPM0_CH3/SPI0_MOSI/LCD_P43_Fault, direction: INPUT, pull_enable: enable}
  - {pin_num: '38', peripheral: GPIOA, signal: 'GPIO, 4', pin_signal: TSI0_CH5/PTA4/I2C1_SDA/TPM0_CH1/NMI_b, direction: OUTPUT, pull_select: down}
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
    /* Port A Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);
    /* Port D Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);

    /* PORTA4 (pin 38) is configured as PTA4 */
    PORT_SetPinMux(BOARD_INITSILEX2401SHIELD_PWRON_PORT, BOARD_INITSILEX2401SHIELD_PWRON_PIN, kPORT_MuxAsGpio);

    PORTA->PCR[4] = ((PORTA->PCR[4] &
                      /* Mask bits to zero which are setting */
                      (~(PORT_PCR_PS_MASK | PORT_PCR_ISF_MASK)))

                     /* Pull Select: Internal pulldown resistor is enabled on the corresponding pin, if the
                      * corresponding PE field is set. */
                     | PORT_PCR_PS(kPORT_PullDown));

    /* PORTD3 (pin 96) is configured as PTD3 */
    PORT_SetPinMux(BOARD_INITSILEX2401SHIELD_IRQ_PORT, BOARD_INITSILEX2401SHIELD_IRQ_PIN, kPORT_MuxAsGpio);

    PORTD->PCR[3] = ((PORTD->PCR[3] &
                      /* Mask bits to zero which are setting */
                      (~(PORT_PCR_PE_MASK | PORT_PCR_ISF_MASK)))

                     /* Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin. */
                     | (uint32_t)(PORT_PCR_PE_MASK));

    /* PORTD4 (pin 97) is configured as SPI1_PCS0 */
    PORT_SetPinMux(PORTD, 4U, kPORT_MuxAlt2);

    /* PORTD5 (pin 98) is configured as SPI1_SCK */
    PORT_SetPinMux(PORTD, 5U, kPORT_MuxAlt2);

    /* PORTD6 (pin 99) is configured as SPI1_MOSI */
    PORT_SetPinMux(PORTD, 6U, kPORT_MuxAlt2);

    /* PORTD7 (pin 100) is configured as SPI1_MISO */
    PORT_SetPinMux(PORTD, 7U, kPORT_MuxAlt2);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
