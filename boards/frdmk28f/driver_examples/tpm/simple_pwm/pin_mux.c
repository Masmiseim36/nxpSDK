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
  - {pin_num: A7, peripheral: LPUART0, signal: RX, pin_signal: PTC25/LPUART0_RX/FB_A4/SDRAM_D4/QSPI0A_SCLK}
  - {pin_num: B7, peripheral: LPUART0, signal: TX, pin_signal: PTC24/LPUART0_TX/FB_A5/SDRAM_D5/QSPI0A_DATA3}
  - {pin_num: M11, peripheral: TPM2, signal: 'CH, 1', pin_signal: PTA11/LLWU_P23/I2C2_SCL/FTM2_CH1/FXIO0_D17/FTM2_QD_PHB/TPM2_CH1/USB1_ID}
  - {pin_num: N11, peripheral: TPM2, signal: 'CH, 0', pin_signal: PTA10/LLWU_P22/I2C2_SDA/FTM2_CH0/FXIO0_D16/FTM2_QD_PHA/TPM2_CH0/TRACE_D0}
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

    /* PORTA10 (pin N11) is configured as TPM2_CH0 */
    PORT_SetPinMux(PORTA, 10U, kPORT_MuxAlt6);

    /* PORTA11 (pin M11) is configured as TPM2_CH1 */
    PORT_SetPinMux(PORTA, 11U, kPORT_MuxAlt6);

    /* PORTC24 (pin B7) is configured as LPUART0_TX */
    PORT_SetPinMux(PORTC, 24U, kPORT_MuxAlt3);

    /* PORTC25 (pin A7) is configured as LPUART0_RX */
    PORT_SetPinMux(PORTC, 25U, kPORT_MuxAlt3);

    SIM->SOPT5 = ((SIM->SOPT5 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT5_LPUART0TXSRC_MASK | SIM_SOPT5_LPUART0RXSRC_MASK)))

                  /* LPUART0 transmit data source select: LPUART0_TX pin. */
                  | SIM_SOPT5_LPUART0TXSRC(SOPT5_LPUART0TXSRC_LPUART_TX)

                  /* LPUART0 receive data source select: LPUART0_RX pin. */
                  | SIM_SOPT5_LPUART0RXSRC(SOPT5_LPUART0RXSRC_LPUART_RX));

    SIM->SOPT9 = ((SIM->SOPT9 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT9_TPM2CH0SRC_MASK)))

                  /* TPM2 channel 0 input capture source select: TPM2_CH0 signal. */
                  | SIM_SOPT9_TPM2CH0SRC(SOPT9_TPM2CH0SRC_TPM));
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
