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
processor: MK28FN2M0xxx15
package_id: MK28FN2M0VMI15
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
  - {pin_num: L9, peripheral: GPIOA, signal: 'GPIO, 4', pin_signal: TSI0_CH5/PTA4/LLWU_P3/FTM0_CH1/FXIO0_D14/NMI_b, direction: INPUT, slew_rate: fast, open_drain: disable,
    pull_select: up, pull_enable: enable, passive_filter: disable}
  - {pin_num: E2, peripheral: GPIOE, signal: 'GPIO, 6', pin_signal: PTE6/LLWU_P16/FXIO0_D12/LPUART3_CTS_b/I2S0_MCLK/QSPI0B_DATA3/FTM3_CH1/SDHC0_D4, direction: OUTPUT}
  - {pin_num: E3, peripheral: GPIOE, signal: 'GPIO, 7', pin_signal: PTE7/FXIO0_D13/LPUART3_RTS_b/I2S0_RXD0/QSPI0B_SCLK/FTM3_CH2/QSPI0A_SS1_B, direction: OUTPUT}
  - {pin_num: E4, peripheral: GPIOE, signal: 'GPIO, 8', pin_signal: PTE8/I2S0_RXD1/FXIO0_D14/I2S0_RX_FS/QSPI0B_DATA0/FTM3_CH3/SDHC0_D5, direction: OUTPUT}
  - {pin_num: A6, peripheral: GPIOD, signal: 'GPIO, 0', pin_signal: PTD0/LLWU_P12/SPI0_PCS0/LPUART2_RTS_b/FTM3_CH0/FB_ALE/FB_CS1_b/FB_TS_b/FXIO0_D22, direction: INPUT,
    slew_rate: fast, open_drain: disable, pull_select: up, pull_enable: enable}
  - {pin_num: A7, peripheral: LPUART0, signal: RX, pin_signal: PTC25/LPUART0_RX/FB_A4/SDRAM_D4/QSPI0A_SCLK}
  - {pin_num: B7, peripheral: LPUART0, signal: TX, pin_signal: PTC24/LPUART0_TX/FB_A5/SDRAM_D5/QSPI0A_DATA3}
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
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    const port_pin_config_t porta4_pinL9_config = {/* Internal pull-up resistor is enabled */
                                                   kPORT_PullUp,
                                                   /* Fast slew rate is configured */
                                                   kPORT_FastSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Open drain is disabled */
                                                   kPORT_OpenDrainDisable,
                                                   /* Low drive strength is configured */
                                                   kPORT_LowDriveStrength,
                                                   /* Pin is configured as PTA4 */
                                                   kPORT_MuxAsGpio,
                                                   /* Pin Control Register fields [15:0] are not locked */
                                                   kPORT_UnlockRegister};
    /* PORTA4 (pin L9) is configured as PTA4 */
    PORT_SetPinConfig(PORTA, 4U, &porta4_pinL9_config);

    /* PORTC24 (pin B7) is configured as LPUART0_TX */
    PORT_SetPinMux(PORTC, 24U, kPORT_MuxAlt3);

    /* PORTC25 (pin A7) is configured as LPUART0_RX */
    PORT_SetPinMux(PORTC, 25U, kPORT_MuxAlt3);

    const port_pin_config_t portd0_pinA6_config = {/* Internal pull-up resistor is enabled */
                                                   kPORT_PullUp,
                                                   /* Fast slew rate is configured */
                                                   kPORT_FastSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Open drain is disabled */
                                                   kPORT_OpenDrainDisable,
                                                   /* Low drive strength is configured */
                                                   kPORT_LowDriveStrength,
                                                   /* Pin is configured as PTD0 */
                                                   kPORT_MuxAsGpio,
                                                   /* Pin Control Register fields [15:0] are not locked */
                                                   kPORT_UnlockRegister};
    /* PORTD0 (pin A6) is configured as PTD0 */
    PORT_SetPinConfig(PORTD, 0U, &portd0_pinA6_config);

    /* PORTE6 (pin E2) is configured as PTE6 */
    PORT_SetPinMux(PORTE, 6U, kPORT_MuxAsGpio);

    /* PORTE7 (pin E3) is configured as PTE7 */
    PORT_SetPinMux(PORTE, 7U, kPORT_MuxAsGpio);

    /* PORTE8 (pin E4) is configured as PTE8 */
    PORT_SetPinMux(PORTE, 8U, kPORT_MuxAsGpio);

    SIM->SOPT5 = ((SIM->SOPT5 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT5_LPUART0TXSRC_MASK | SIM_SOPT5_LPUART0RXSRC_MASK)))

                  /* LPUART0 transmit data source select: LPUART0_TX pin. */
                  | SIM_SOPT5_LPUART0TXSRC(SOPT5_LPUART0TXSRC_LPUART_TX)

                  /* LPUART0 receive data source select: LPUART0_RX pin. */
                  | SIM_SOPT5_LPUART0RXSRC(SOPT5_LPUART0RXSRC_LPUART_RX));
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
