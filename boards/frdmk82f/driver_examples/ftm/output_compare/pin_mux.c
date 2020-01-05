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
processor: MK82FN256xxx15
package_id: MK82FN256VLL15
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
  - {pin_num: '86', peripheral: LPUART4, signal: RX, pin_signal: PTC14/LPUART4_RX/FB_AD25/SDRAM_D25/FXIO0_D20}
  - {pin_num: '87', peripheral: LPUART4, signal: TX, pin_signal: PTC15/LPUART4_TX/FB_AD24/SDRAM_D24/FXIO0_D21}
  - {pin_num: '80', peripheral: FTM3, signal: 'CH, 4', pin_signal: CMP0_IN2/PTC8/FTM3_CH4/I2S0_MCLK/FB_AD7/SDRAM_A15/FXIO0_D16}
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

    /* PORTC14 (pin 86) is configured as LPUART4_RX */
    PORT_SetPinMux(PORTC, 14U, kPORT_MuxAlt3);

    /* PORTC15 (pin 87) is configured as LPUART4_TX */
    PORT_SetPinMux(PORTC, 15U, kPORT_MuxAlt3);

    /* PORTC8 (pin 80) is configured as FTM3_CH4 */
    PORT_SetPinMux(PORTC, 8U, kPORT_MuxAlt3);

    SIM->SOPT8 = ((SIM->SOPT8 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT8_FTM3OCH4SRC_MASK)))

                  /* FTM3 channel 4 output source: FTM3_CH4 pin is output of FTM3 channel 4 output. */
                  | SIM_SOPT8_FTM3OCH4SRC(SOPT8_FTM3OCH4SRC_FTM));
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
