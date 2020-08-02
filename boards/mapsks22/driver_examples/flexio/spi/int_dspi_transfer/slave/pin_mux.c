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
processor: MKS22FN256xxx12
package_id: MKS22FN256VLL12
mcu_data: ksdk2_0
processor_version: 0.0.11
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
  - {pin_num: '1', peripheral: UART1, signal: TX, pin_signal: ADC0_SE4a/PTE0/CLKOUT32K/SPI1_PCS1/UART1_TX/LPI2C1_SDA/RTC_CLKOUT}
  - {pin_num: '2', peripheral: UART1, signal: RX, pin_signal: ADC0_SE5a/PTE1/LLWU_P0/SPI1_SOUT/UART1_RX/LPI2C1_SCL/SPI1_SIN}
  - {pin_num: '84', peripheral: FLEXIO, signal: 'D, 0', pin_signal: PTC12/LPI2C1_SCLS/TPM_CLKIN0/FXIO0_D0}
  - {pin_num: '85', peripheral: FLEXIO, signal: 'D, 1', pin_signal: PTC13/LPI2C1_SDAS/TPM_CLKIN1/FXIO0_D1}
  - {pin_num: '86', peripheral: FLEXIO, signal: 'D, 2', pin_signal: PTC14/LPUART0_RTS_b/FXIO0_D2}
  - {pin_num: '87', peripheral: FLEXIO, signal: 'D, 3', pin_signal: PTC15/LPUART0_CTS_b/FXIO0_D3, slew_rate: fast, open_drain: disable, pull_select: up, pull_enable: enable}
  - {pin_num: '97', peripheral: SPI1, signal: PCS0_SS, pin_signal: PTD4/LLWU_P14/SPI0_PCS1/UART0_RTS_b/TPM0_CH4/EWM_IN/SPI1_PCS0}
  - {pin_num: '98', peripheral: SPI1, signal: SCK, pin_signal: ADC0_SE6b/PTD5/SPI0_PCS2/UART0_CTS_b/TPM0_CH5/EWM_OUT_b/SPI1_SCK}
  - {pin_num: '99', peripheral: SPI1, signal: SOUT, pin_signal: ADC0_SE7b/PTD6/LLWU_P15/SPI0_PCS3/UART0_RX/SPI1_SOUT}
  - {pin_num: '100', peripheral: SPI1, signal: SIN, pin_signal: PTD7/UART0_TX/SPI1_SIN}
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
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);
    /* Port D Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    /* PORTC12 (pin 84) is configured as FXIO0_D0 */
    PORT_SetPinMux(PORTC, 12U, kPORT_MuxAlt7);

    /* PORTC13 (pin 85) is configured as FXIO0_D1 */
    PORT_SetPinMux(PORTC, 13U, kPORT_MuxAlt7);

    /* PORTC14 (pin 86) is configured as FXIO0_D2 */
    PORT_SetPinMux(PORTC, 14U, kPORT_MuxAlt7);

    const port_pin_config_t portc15_pin87_config = {/* Internal pull-up resistor is enabled */
                                                    kPORT_PullUp,
                                                    /* Fast slew rate is configured */
                                                    kPORT_FastSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Open drain is disabled */
                                                    kPORT_OpenDrainDisable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as FXIO0_D3 */
                                                    kPORT_MuxAlt7,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    kPORT_UnlockRegister};
    /* PORTC15 (pin 87) is configured as FXIO0_D3 */
    PORT_SetPinConfig(PORTC, 15U, &portc15_pin87_config);

    /* PORTD4 (pin 97) is configured as SPI1_PCS0 */
    PORT_SetPinMux(PORTD, 4U, kPORT_MuxAlt7);

    /* PORTD5 (pin 98) is configured as SPI1_SCK */
    PORT_SetPinMux(PORTD, 5U, kPORT_MuxAlt7);

    /* PORTD6 (pin 99) is configured as SPI1_SOUT */
    PORT_SetPinMux(PORTD, 6U, kPORT_MuxAlt7);

    /* PORTD7 (pin 100) is configured as SPI1_SIN */
    PORT_SetPinMux(PORTD, 7U, kPORT_MuxAlt7);

    /* PORTE0 (pin 1) is configured as UART1_TX */
    PORT_SetPinMux(PORTE, 0U, kPORT_MuxAlt3);

    /* PORTE1 (pin 2) is configured as UART1_RX */
    PORT_SetPinMux(PORTE, 1U, kPORT_MuxAlt3);

    SIM->SOPT5 = ((SIM->SOPT5 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT5_UART1TXSRC_MASK)))

                  /* UART 1 transmit data source select: UART1_TX pin. */
                  | SIM_SOPT5_UART1TXSRC(SOPT5_UART1TXSRC_UART_TX));
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
