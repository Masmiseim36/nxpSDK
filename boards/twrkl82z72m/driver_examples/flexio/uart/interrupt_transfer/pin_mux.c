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
package_id: MKL82Z128VMC7
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
  - {pin_num: D10, peripheral: FLEXIO0, signal: 'D, 4', pin_signal: PTB10/SPI1_PCS0/FXIO0_D4}
  - {pin_num: C10, peripheral: FLEXIO0, signal: 'D, 5', pin_signal: PTB11/SPI1_SCK/FXIO0_D5}
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

    /* PORTB10 (pin D10) is configured as FXIO0_D4 */
    PORT_SetPinMux(PORTB, 10U, kPORT_MuxAlt7);

    /* PORTB11 (pin C10) is configured as FXIO0_D5 */
    PORT_SetPinMux(PORTB, 11U, kPORT_MuxAlt7);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
