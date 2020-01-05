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
processor: MK22FN512xxx12
package_id: MK22FN512VDC12
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
  - {pin_num: E4, peripheral: UART1, signal: TX, pin_signal: ADC1_SE4a/PTE0/CLKOUT32K/SPI1_PCS1/UART1_TX/I2C1_SDA/RTC_CLKOUT}
  - {pin_num: E3, peripheral: UART1, signal: RX, pin_signal: ADC1_SE5a/PTE1/LLWU_P0/SPI1_SOUT/UART1_RX/I2C1_SCL/SPI1_SIN}
  - {pin_num: D10, peripheral: SPI1, signal: PCS0_SS, pin_signal: ADC1_SE14/PTB10/SPI1_PCS0/LPUART0_RX/FB_AD19/FTM0_FLT1}
  - {pin_num: C10, peripheral: SPI1, signal: SCK, pin_signal: ADC1_SE15/PTB11/SPI1_SCK/LPUART0_TX/FB_AD18/FTM0_FLT2}
  - {pin_num: B10, peripheral: SPI1, signal: SOUT, pin_signal: PTB16/SPI1_SOUT/UART0_RX/FTM_CLKIN0/FB_AD17/EWM_IN}
  - {pin_num: E9, peripheral: SPI1, signal: SIN, pin_signal: PTB17/SPI1_SIN/UART0_TX/FTM_CLKIN1/FB_AD16/EWM_OUT_b}
  - {pin_num: F3, peripheral: GPIOE, signal: 'GPIO, 6', pin_signal: PTE6/SPI1_PCS3/LPUART0_CTS_b/I2S0_MCLK/FTM3_CH1/USB_SOF_OUT, slew_rate: fast, open_drain: disable,
    pull_select: up, pull_enable: enable}
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
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    /* PORTB10 (pin D10) is configured as SPI1_PCS0 */
    PORT_SetPinMux(PORTB, 10U, kPORT_MuxAlt2);

    /* PORTB11 (pin C10) is configured as SPI1_SCK */
    PORT_SetPinMux(PORTB, 11U, kPORT_MuxAlt2);

    /* PORTB16 (pin B10) is configured as SPI1_SOUT */
    PORT_SetPinMux(PORTB, 16U, kPORT_MuxAlt2);

    /* PORTB17 (pin E9) is configured as SPI1_SIN */
    PORT_SetPinMux(PORTB, 17U, kPORT_MuxAlt2);

    /* PORTE0 (pin E4) is configured as UART1_TX */
    PORT_SetPinMux(PORTE, 0U, kPORT_MuxAlt3);

    /* PORTE1 (pin E3) is configured as UART1_RX */
    PORT_SetPinMux(PORTE, 1U, kPORT_MuxAlt3);

    const port_pin_config_t porte6_pinF3_config = {/* Internal pull-up resistor is enabled */
                                                   kPORT_PullUp,
                                                   /* Fast slew rate is configured */
                                                   kPORT_FastSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Open drain is disabled */
                                                   kPORT_OpenDrainDisable,
                                                   /* Low drive strength is configured */
                                                   kPORT_LowDriveStrength,
                                                   /* Pin is configured as PTE6 */
                                                   kPORT_MuxAsGpio,
                                                   /* Pin Control Register fields [15:0] are not locked */
                                                   kPORT_UnlockRegister};
    /* PORTE6 (pin F3) is configured as PTE6 */
    PORT_SetPinConfig(PORTE, 6U, &porte6_pinF3_config);

    SIM->SOPT5 = ((SIM->SOPT5 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT5_UART1TXSRC_MASK)))

                  /* UART 1 transmit data source select: UART1_TX pin. */
                  | SIM_SOPT5_UART1TXSRC(SOPT5_UART1TXSRC_UART_TX));
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
