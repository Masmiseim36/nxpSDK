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
processor: MKE18F512xxx16
package_id: MKE18F512VLL16
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
  - {pin_num: '20', peripheral: LPUART2, signal: RX, pin_signal: PTD17/FTM0_FLT2/LPUART2_RX}
  - {pin_num: '19', peripheral: LPUART2, signal: TX, pin_signal: PTE12/FTM0_FLT3/LPUART2_TX}
  - {pin_num: '23', peripheral: GPIOE, signal: 'GPIO, 9', pin_signal: ACMP2_IN2/DAC0_OUT/PTE9/FTM0_CH7/LPUART2_CTS, direction: OUTPUT}
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
    /* Clock Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortD);
    /* Clock Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortE);

    /* PORTD17 (pin 20) is configured as LPUART2_RX */
    PORT_SetPinMux(PORTD, 17U, kPORT_MuxAlt3);

    /* PORTE12 (pin 19) is configured as LPUART2_TX */
    PORT_SetPinMux(PORTE, 12U, kPORT_MuxAlt3);

    /* PORTE9 (pin 23) is configured as PTE9 */
    PORT_SetPinMux(PORTE, 9U, kPORT_MuxAsGpio);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
