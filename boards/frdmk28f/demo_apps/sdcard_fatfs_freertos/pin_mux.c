/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v5.0
processor: MK28FN2M0xxx15
package_id: MK28FN2M0VMI15
mcu_data: ksdk2_0
processor_version: 0.0.20
board: FRDM-K28F
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 *
 * END ****************************************************************************************************************/
void BOARD_InitBootPins(void)
{
    BOARD_InitPins();
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: H12, peripheral: SDHC, signal: 'D, 3', pin_signal: PTA28/LPUART3_TX/SDHC0_D3/FB_A25/I2S1_RXD1, pull_select: up, pull_enable: enable}
  - {pin_num: H11, peripheral: SDHC, signal: 'D, 2', pin_signal: PTA29/LPUART3_RX/SDHC0_D2/FB_A24/I2S1_RXD0, pull_select: up, pull_enable: enable}
  - {pin_num: K11, peripheral: SDHC, signal: 'D, 1', pin_signal: PTA24/LPUART2_TX/SDHC0_D1/FB_A15/SDRAM_D15/FB_A29/I2S1_TX_BCLK, pull_select: up, pull_enable: enable}
  - {pin_num: J11, peripheral: SDHC, signal: 'D, 0', pin_signal: PTA25/LPUART2_RX/SDHC0_D0/FB_A14/SDRAM_D14/FB_A28/I2S1_TX_FS, pull_select: up, pull_enable: enable}
  - {pin_num: H13, peripheral: SDHC, signal: CMD, pin_signal: PTA27/LPUART2_RTS_b/SDHC0_CMD/FB_A12/SDRAM_D12/FB_A26/I2S1_TXD1, pull_select: up, pull_enable: enable}
  - {pin_num: J10, peripheral: SDHC, signal: DCLK, pin_signal: PTA26/LPUART2_CTS_b/SDHC0_DCLK/FB_A13/SDRAM_D13/FB_A27/I2S1_TXD0, pull_select: up, pull_enable: enable}
  - {pin_num: A7, peripheral: LPUART0, signal: RX, pin_signal: PTC25/LPUART0_RX/FB_A4/SDRAM_D4/QSPI0A_SCLK}
  - {pin_num: B7, peripheral: LPUART0, signal: TX, pin_signal: PTC24/LPUART0_TX/FB_A5/SDRAM_D5/QSPI0A_DATA3}
  - {pin_num: F13, peripheral: GPIOB, signal: 'GPIO, 5', pin_signal: PTB5/FTM2_FLT0, identifier: '', pull_select: down, pull_enable: disable}
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
    /* Port B Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    /* PORTA24 (pin K11) is configured as SDHC0_D1 */
    PORT_SetPinMux(BOARD_SDHC0_D1_PORT, BOARD_SDHC0_D1_PIN, kPORT_MuxAlt4);

    PORTA->PCR[24] = ((PORTA->PCR[24] &
                       /* Mask bits to zero which are setting */
                       (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ISF_MASK)))

                      /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the
                       * corresponding PE field is set. */
                      | (uint32_t)(kPORT_PullUp));

    /* PORTA25 (pin J11) is configured as SDHC0_D0 */
    PORT_SetPinMux(BOARD_SDHC0_D0_PORT, BOARD_SDHC0_D0_PIN, kPORT_MuxAlt4);

    PORTA->PCR[25] = ((PORTA->PCR[25] &
                       /* Mask bits to zero which are setting */
                       (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ISF_MASK)))

                      /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the
                       * corresponding PE field is set. */
                      | (uint32_t)(kPORT_PullUp));

    /* PORTA26 (pin J10) is configured as SDHC0_DCLK */
    PORT_SetPinMux(BOARD_SDHC0_DCLK_PORT, BOARD_SDHC0_DCLK_PIN, kPORT_MuxAlt4);

    PORTA->PCR[26] = ((PORTA->PCR[26] &
                       /* Mask bits to zero which are setting */
                       (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ISF_MASK)))

                      /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the
                       * corresponding PE field is set. */
                      | (uint32_t)(kPORT_PullUp));

    /* PORTA27 (pin H13) is configured as SDHC0_CMD */
    PORT_SetPinMux(BOARD_SDHC0_CMD_PORT, BOARD_SDHC0_CMD_PIN, kPORT_MuxAlt4);

    PORTA->PCR[27] = ((PORTA->PCR[27] &
                       /* Mask bits to zero which are setting */
                       (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ISF_MASK)))

                      /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the
                       * corresponding PE field is set. */
                      | (uint32_t)(kPORT_PullUp));

    /* PORTA28 (pin H12) is configured as SDHC0_D3 */
    PORT_SetPinMux(BOARD_SDHC0_D3_PORT, BOARD_SDHC0_D3_PIN, kPORT_MuxAlt4);

    PORTA->PCR[28] = ((PORTA->PCR[28] &
                       /* Mask bits to zero which are setting */
                       (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ISF_MASK)))

                      /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the
                       * corresponding PE field is set. */
                      | (uint32_t)(kPORT_PullUp));

    /* PORTA29 (pin H11) is configured as SDHC0_D2 */
    PORT_SetPinMux(BOARD_SDHC0_D2_PORT, BOARD_SDHC0_D2_PIN, kPORT_MuxAlt4);

    PORTA->PCR[29] = ((PORTA->PCR[29] &
                       /* Mask bits to zero which are setting */
                       (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ISF_MASK)))

                      /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the
                       * corresponding PE field is set. */
                      | (uint32_t)(kPORT_PullUp));

    /* PORTB5 (pin F13) is configured as PTB5 */
    PORT_SetPinMux(PORTB, 5U, kPORT_MuxAsGpio);

    PORTB->PCR[5] = ((PORTB->PCR[5] &
                      /* Mask bits to zero which are setting */
                      (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ISF_MASK)))

                     /* Pull Select: Internal pulldown resistor is enabled on the corresponding pin, if the
                      * corresponding PE field is set. */
                     | PORT_PCR_PS(kPORT_PullDown)

                     /* Pull Enable: Internal pullup or pulldown resistor is not enabled on the corresponding pin. */
                     | PORT_PCR_PE(kPORT_PullDisable));

    /* PORTC24 (pin B7) is configured as LPUART0_TX */
    PORT_SetPinMux(BOARD_DEBUG_UART_TX_PORT, BOARD_DEBUG_UART_TX_PIN, kPORT_MuxAlt3);

    /* PORTC25 (pin A7) is configured as LPUART0_RX */
    PORT_SetPinMux(BOARD_DEBUG_UART_RX_PORT, BOARD_DEBUG_UART_RX_PIN, kPORT_MuxAlt3);

    SIM->SOPT5 = ((SIM->SOPT5 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT5_LPUART0TXSRC_MASK | SIM_SOPT5_LPUART0RXSRC_MASK)))

                  /* LPUART0 transmit data source select: LPUART0_TX pin. */
                  | SIM_SOPT5_LPUART0TXSRC(SOPT5_LPUART0TXSRC_LPUART_TX)

                  /* LPUART0 receive data source select: LPUART0_RX pin. */
                  | SIM_SOPT5_LPUART0RXSRC(SOPT5_LPUART0RXSRC_LPUART_RX));
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
