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
package_id: MKL82Z128VLK7
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
  - {pin_num: '51', peripheral: LPUART0, signal: RX, pin_signal: TSI0_CH9/PTB16/SPI1_SOUT/LPUART0_RX/TPM_CLKIN0/EWM_IN}
  - {pin_num: '52', peripheral: LPUART0, signal: TX, pin_signal: TSI0_CH10/PTB17/SPI1_SIN/LPUART0_TX/TPM_CLKIN1/EWM_OUT_b}
  - {pin_num: '73', peripheral: GPIOD, signal: 'GPIO, 0', pin_signal: PTD0/LLWU_P12/SPI0_PCS0/LPUART2_RTS_b/FXIO0_D22, slew_rate: fast, open_drain: disable, pull_select: up,
    pull_enable: enable}
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
    /* PTD Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);

    /* PORTB16 (pin 51) is configured as LPUART0_RX */
    PORT_SetPinMux(PORTB, 16U, kPORT_MuxAlt3);

    /* PORTB17 (pin 52) is configured as LPUART0_TX */
    PORT_SetPinMux(PORTB, 17U, kPORT_MuxAlt3);

    const port_pin_config_t portd0_pin73_config = {/* Internal pull-up resistor is enabled */
                                                   kPORT_PullUp,
                                                   /* Fast slew rate is configured */
                                                   kPORT_FastSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Open drain is disabled */
                                                   kPORT_OpenDrainDisable,
                                                   /* Pin is configured as PTD0 */
                                                   kPORT_MuxAsGpio,
                                                   /* Pin Control Register fields [15:0] are not locked */
                                                   kPORT_UnlockRegister};
    /* PORTD0 (pin 73) is configured as PTD0 */
    PORT_SetPinConfig(PORTD, 0U, &portd0_pin73_config);

    SIM->SOPT5 = ((SIM->SOPT5 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT5_LPUART0TXSRC_MASK | SIM_SOPT5_LPUART0RXSRC_MASK)))

                  /* LPUART0 transmit data source select: LPUART0_TX pin. */
                  | SIM_SOPT5_LPUART0TXSRC(SOPT5_LPUART0TXSRC_LPUART_TX)

                  /* LPUART 0 receive data source select: LPUART0_RX pin. */
                  | SIM_SOPT5_LPUART0RXSRC(SOPT5_LPUART0RXSRC_LPUART_RX));
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
