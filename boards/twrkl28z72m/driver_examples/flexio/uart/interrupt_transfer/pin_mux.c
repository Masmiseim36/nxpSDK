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
  - {pin_num: '17', peripheral: FLEXIO0, signal: 'D, 3', pin_signal: ADC0_DM2/ADC0_SE6a/PTE19/LPSPI0_SIN/LPUART2_RTS_b/LPI2C0_SCL/FXIO0_D3}
  - {pin_num: '16', peripheral: FLEXIO0, signal: 'D, 2', pin_signal: ADC0_DP2/ADC0_SE2/PTE18/LLWU_P20/LPSPI0_SOUT/LPUART2_CTS_b/LPI2C0_SDA/FXIO0_D2}
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
    CLOCK_EnableClock(kCLOCK_PortE);

    /* PORTE18 (pin 16) is configured as FXIO0_D2 */
    PORT_SetPinMux(PORTE, 18U, kPORT_MuxAlt6);

    /* PORTE19 (pin 17) is configured as FXIO0_D3 */
    PORT_SetPinMux(PORTE, 19U, kPORT_MuxAlt6);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
