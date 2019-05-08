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
  - {pin_num: '43', peripheral: FLEXIO0, signal: 'D, 0', pin_signal: ADC0_SE8/TSI0_CH0/PTB0/LLWU_P5/I2C0_SCL/TPM1_CH0/FXIO0_D0}
  - {pin_num: '44', peripheral: FLEXIO0, signal: 'D, 1', pin_signal: ADC0_SE9/TSI0_CH6/PTB1/I2C0_SDA/TPM1_CH1/FXIO0_D1}
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

    /* PORTB0 (pin 43) is configured as FXIO0_D0 */
    PORT_SetPinMux(PORTB, 0U, kPORT_MuxAlt7);

    /* PORTB1 (pin 44) is configured as FXIO0_D1 */
    PORT_SetPinMux(PORTB, 1U, kPORT_MuxAlt7);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
