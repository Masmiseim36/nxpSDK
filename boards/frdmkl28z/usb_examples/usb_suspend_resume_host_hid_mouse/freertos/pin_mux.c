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
  - {pin_num: '62', peripheral: LPUART0, signal: RX, pin_signal: TSI0_CH9/PTB16/LPSPI1_SOUT/LPUART0_RX/TPM0_CLKIN/LPSPI2_PCS3/FXIO0_D16}
  - {pin_num: '63', peripheral: LPUART0, signal: TX, pin_signal: TSI0_CH10/PTB17/LPSPI1_SIN/LPUART0_TX/TPM1_CLKIN/LPSPI2_PCS2/FXIO0_D17}
  - {pin_num: '5', peripheral: GPIOE, signal: 'GPIO, 4', pin_signal: PTE4/LLWU_P2/LPSPI1_PCS0, direction: INPUT, slew_rate: fast, open_drain: disable, pull_select: up,
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
    /* Clock Gate Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortB);
    /* Clock Gate Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortE);

    /* PORTB16 (pin 62) is configured as LPUART0_RX */
    PORT_SetPinMux(PORTB, 16U, kPORT_MuxAlt3);

    /* PORTB17 (pin 63) is configured as LPUART0_TX */
    PORT_SetPinMux(PORTB, 17U, kPORT_MuxAlt3);

    const port_pin_config_t porte4_pin5_config = {/* Internal pull-up resistor is enabled */
                                                  kPORT_PullUp,
                                                  /* Fast slew rate is configured */
                                                  kPORT_FastSlewRate,
                                                  /* Passive filter is disabled */
                                                  kPORT_PassiveFilterDisable,
                                                  /* Open drain is disabled */
                                                  kPORT_OpenDrainDisable,
                                                  /* Low drive strength is configured */
                                                  kPORT_LowDriveStrength,
                                                  /* Pin is configured as PTE4 */
                                                  kPORT_MuxAsGpio,
                                                  /* Pin Control Register fields [15:0] are not locked */
                                                  kPORT_UnlockRegister};
    /* PORTE4 (pin 5) is configured as PTE4 */
    PORT_SetPinConfig(PORTE, 4U, &porte4_pin5_config);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
