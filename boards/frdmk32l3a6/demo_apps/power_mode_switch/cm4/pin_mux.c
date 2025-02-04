/*
 * Copyright 2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v8.0
processor: K32L3A60xxx
package_id: K32L3A60VPJ1A
mcu_data: ksdk2_0
processor_version: 0.8.5
board: FRDM-K32L3A6
pin_labels:
- {pin_num: J1, pin_signal: PTB20/LLWU_P11/LPSPI1_SCK/LPUART2_CTS/FB_CS0_b/TPM1_CH0/FXIO0_D10, label: 'U15[6]/MEMORY/SPI2_SCK', identifier: FLASH_SPI1_SCK}
- {pin_num: L1, pin_signal: PTB22/LLWU_P12/LPSPI1_PCS2/LPUART0_CTS/LPI2C2_SDA/FB_AD3/TPM2_CLKIN/FXIO0_D12, label: 'U15[1]/MEMORY/SPI2_PCS0', identifier: FLASH_SPI1_PCS}
- {pin_num: J2, pin_signal: PTB21/LPSPI1_SOUT/LPUART2_RTS/LPI2C2_HREQ/FB_AD4/TPM1_CH1/FXIO0_D11, label: 'U15[5]/MEMORY/SPI2_SOUT', identifier: FLASH_SPI1_SOUT}
- {pin_num: L2, pin_signal: PTB24/LPSPI1_SIN/LPUART0_RTS/LPI2C2_SCL/FB_AD2/EWM_IN/FXIO0_D13, label: 'U15[2]/MEMORY/SPI2_SIN', identifier: FLASH_SPI1_SIN}
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
    BOARD_InitPins_Core0();
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins_Core0:
- options: {callFromInitBoot: 'true', coreID: cm4, enableClock: 'true'}
- pin_list:
  - {pin_num: N2, peripheral: LPUART0, signal: RX, pin_signal: LPCMP0_IN0/PTC7/LLWU_P15/LPSPI0_PCS3/LPUART0_RX/LPI2C1_HREQ/TPM0_CH0/LPTMR1_ALT1}
  - {pin_num: P3, peripheral: LPUART0, signal: TX, pin_signal: LPCMP0_IN1/PTC8/LPSPI0_SCK/LPUART0_TX/LPI2C0_HREQ/TPM0_CH1}
  - {pin_num: J1, peripheral: LPSPI1, signal: SCK, pin_signal: PTB20/LLWU_P11/LPSPI1_SCK/LPUART2_CTS/FB_CS0_b/TPM1_CH0/FXIO0_D10}
  - {pin_num: L1, peripheral: LPSPI1, signal: PCS2, pin_signal: PTB22/LLWU_P12/LPSPI1_PCS2/LPUART0_CTS/LPI2C2_SDA/FB_AD3/TPM2_CLKIN/FXIO0_D12}
  - {pin_num: J2, peripheral: LPSPI1, signal: SOUT, pin_signal: PTB21/LPSPI1_SOUT/LPUART2_RTS/LPI2C2_HREQ/FB_AD4/TPM1_CH1/FXIO0_D11}
  - {pin_num: L2, peripheral: LPSPI1, signal: SIN, pin_signal: PTB24/LPSPI1_SIN/LPUART0_RTS/LPI2C2_SCL/FB_AD2/EWM_IN/FXIO0_D13}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins_Core0
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
/* Function assigned for the Cortex-M4F */
void BOARD_InitPins_Core0(void)
{
    /* Clock Gate Control: Clock enabled. The current clock selection and divider options are locked. */
    CLOCK_EnableClock(kCLOCK_PortB);
    /* Clock Gate Control: Clock enabled. The current clock selection and divider options are locked. */
    CLOCK_EnableClock(kCLOCK_PortC);

    /* PORTB20 (pin J1) is configured as LPSPI1_SCK */
    PORT_SetPinMux(BOARD_INITPINS_CORE0_FLASH_SPI1_SCK_PORT, BOARD_INITPINS_CORE0_FLASH_SPI1_SCK_PIN, kPORT_MuxAlt2);

    /* PORTB21 (pin J2) is configured as LPSPI1_SOUT */
    PORT_SetPinMux(BOARD_INITPINS_CORE0_FLASH_SPI1_SOUT_PORT, BOARD_INITPINS_CORE0_FLASH_SPI1_SOUT_PIN, kPORT_MuxAlt2);

    /* PORTB22 (pin L1) is configured as LPSPI1_PCS2 */
    PORT_SetPinMux(BOARD_INITPINS_CORE0_FLASH_SPI1_PCS_PORT, BOARD_INITPINS_CORE0_FLASH_SPI1_PCS_PIN, kPORT_MuxAlt2);

    /* PORTB24 (pin L2) is configured as LPSPI1_SIN */
    PORT_SetPinMux(BOARD_INITPINS_CORE0_FLASH_SPI1_SIN_PORT, BOARD_INITPINS_CORE0_FLASH_SPI1_SIN_PIN, kPORT_MuxAlt2);

    /* PORTC7 (pin N2) is configured as LPUART0_RX */
    PORT_SetPinMux(BOARD_INITPINS_CORE0_DEBUG_UART0_RX_PORT, BOARD_INITPINS_CORE0_DEBUG_UART0_RX_PIN, kPORT_MuxAlt3);

    /* PORTC8 (pin P3) is configured as LPUART0_TX */
    PORT_SetPinMux(BOARD_INITPINS_CORE0_DEBUG_UART0_TX_PORT, BOARD_INITPINS_CORE0_DEBUG_UART0_TX_PIN, kPORT_MuxAlt3);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
