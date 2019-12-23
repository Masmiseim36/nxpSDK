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
processor: MKV31F512xxx12
package_id: MKV31F512VLL12
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
LPUART0_InitPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '5', peripheral: LPUART0, signal: TX, pin_signal: PTE4/LLWU_P2/SPI1_PCS0/LPUART0_TX}
  - {pin_num: '6', peripheral: LPUART0, signal: RX, pin_signal: PTE5/SPI1_PCS2/LPUART0_RX/FTM3_CH0}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : LPUART0_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void LPUART0_InitPins(void)
{
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    /* PORTE4 (pin 5) is configured as LPUART0_TX */
    PORT_SetPinMux(PORTE, 4U, kPORT_MuxAlt3);

    /* PORTE5 (pin 6) is configured as LPUART0_RX */
    PORT_SetPinMux(PORTE, 5U, kPORT_MuxAlt3);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
LPUART0_DeinitPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '5', peripheral: n/a, signal: disabled, pin_signal: PTE4/LLWU_P2/SPI1_PCS0/LPUART0_TX}
  - {pin_num: '6', peripheral: n/a, signal: disabled, pin_signal: PTE5/SPI1_PCS2/LPUART0_RX/FTM3_CH0}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : LPUART0_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void LPUART0_DeinitPins(void)
{
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    /* PORTE4 (pin 5) is disabled */
    PORT_SetPinMux(PORTE, 4U, kPORT_PinDisabledOrAnalog);

    /* PORTE5 (pin 6) is disabled */
    PORT_SetPinMux(PORTE, 5U, kPORT_PinDisabledOrAnalog);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
