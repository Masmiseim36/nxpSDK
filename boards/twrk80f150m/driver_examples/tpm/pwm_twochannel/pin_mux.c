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
processor: MK80FN256xxx15
package_id: MK80FN256VDC15
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
  - {pin_num: B8, peripheral: LPUART1, signal: RX, pin_signal: CMP1_IN1/PTC3/LLWU_P7/SPI0_PCS1/LPUART1_RX/FTM0_CH2/CLKOUT/I2S0_TX_BCLK}
  - {pin_num: A8, peripheral: LPUART1, signal: TX, pin_signal: PTC4/LLWU_P8/SPI0_PCS0/LPUART1_TX/FTM0_CH3/FB_AD11/SDRAM_A19/CMP1_OUT}
  - {pin_num: D9, peripheral: TPM2, signal: 'CH, 0', pin_signal: TSI0_CH11/PTB18/FTM2_CH0/I2S0_TX_BCLK/FB_AD15/SDRAM_A23/FTM2_QD_PHA/TPM2_CH0/FXIO0_D6}
  - {pin_num: C9, peripheral: TPM2, signal: 'CH, 1', pin_signal: TSI0_CH12/PTB19/FTM2_CH1/I2S0_TX_FS/FB_OE_b/FTM2_QD_PHB/TPM2_CH1/FXIO0_D7}
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
    /* Port B Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    /* PORTB18 (pin D9) is configured as TPM2_CH0 */
    PORT_SetPinMux(PORTB, 18U, kPORT_MuxAlt6);

    /* PORTB19 (pin C9) is configured as TPM2_CH1 */
    PORT_SetPinMux(PORTB, 19U, kPORT_MuxAlt6);

    /* PORTC3 (pin B8) is configured as LPUART1_RX */
    PORT_SetPinMux(PORTC, 3U, kPORT_MuxAlt3);

    /* PORTC4 (pin A8) is configured as LPUART1_TX */
    PORT_SetPinMux(PORTC, 4U, kPORT_MuxAlt3);

    SIM->SOPT5 = ((SIM->SOPT5 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT5_LPUART1TXSRC_MASK | SIM_SOPT5_LPUART1RXSRC_MASK)))

                  /* LPUART1 transmit data source select: LPUART1_TX pin. */
                  | SIM_SOPT5_LPUART1TXSRC(SOPT5_LPUART1TXSRC_LPUART_TX)

                  /* LPUART1 receive data source select: LPUART1_RX pin. */
                  | SIM_SOPT5_LPUART1RXSRC(SOPT5_LPUART1RXSRC_LPUART_RX));

    SIM->SOPT9 = ((SIM->SOPT9 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT9_TPM2CH0SRC_MASK)))

                  /* TPM2 channel 0 input capture source select: TPM2_CH0 signal. */
                  | SIM_SOPT9_TPM2CH0SRC(SOPT9_TPM2CH0SRC_TPM));
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
