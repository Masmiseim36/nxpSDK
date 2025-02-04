/*
 * Copyright 2020 NXP
 * All rights reserved.
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
product: Pins v7.0
processor: K32L2A41xxxxA
package_id: K32L2A41VLL1A
mcu_data: ksdk2_0
processor_version: 7.0.0
board: FRDM-K32L2A4S
pin_labels:
- {pin_num: '62', pin_signal: TSI0_CH9/PTB16/LPSPI1_SOUT/LPUART0_RX/TPM0_CLKIN/LPSPI2_PCS3/FXIO0_D16, label: 'J1[2]/U7[25]/D0/UART0_RX/FXIO_D16/UART1_RX_TGTMCU',
  identifier: DEBUG_UART_RX;RX_GPIO;RX}
- {pin_num: '63', pin_signal: TSI0_CH10/PTB17/LPSPI1_SIN/LPUART0_TX/TPM1_CLKIN/LPSPI2_PCS2/FXIO0_D17, label: 'J1[4]/U7[24]/D1/UART0_TX/FXIO_D17/UART1_TX_TGTMCU',
  identifier: DEBUG_UART_TX;TX}
- {pin_num: '66', pin_signal: PTB20/LPSPI2_PCS0/CMP0_OUT, label: 'J20[4]/SPI2_PCS0', identifier: PCS0}
- {pin_num: '67', pin_signal: PTB21/LPSPI2_SCK/CMP1_OUT, label: 'J20[5]/SPI2_SCK', identifier: SCK}
- {pin_num: '68', pin_signal: PTB22/LPSPI2_SOUT, label: 'J20[6]/SPI2_SOUT', identifier: SOUT}
- {pin_num: '69', pin_signal: PTB23/LPSPI2_SIN, label: 'J20[7]/SPI2_SIN', identifier: SIN}
- {pin_num: '71', pin_signal: ADC0_SE15/TSI0_CH14/PTC1/LLWU_P6/LPI2C1_SCL/LPUART1_RTS_b/TPM0_CH0, label: 'J4[12]/A5/ADC0_SE15/I2C1_SCL', identifier: SCL}
- {pin_num: '72', pin_signal: ADC0_SE11/CMP1_IN0/TSI0_CH15/PTC2/LPI2C1_SDA/LPUART1_CTS_b/TPM0_CH1, label: 'J4[10]/A4/ADC0_SE11/I2C1_SDA', identifier: SDA}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

#include "fsl_common.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "pin_mux.h"

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 *
 * END ****************************************************************************************************************/
void BOARD_InitBootPins(void)
{
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
UART0_PollForActivity:
- options: {callFromInitBoot: 'false', coreID: core0, enableClock: 'false'}
- pin_list:
  - {pin_num: '62', peripheral: GPIOB, signal: 'GPIO, 16', pin_signal: TSI0_CH9/PTB16/LPSPI1_SOUT/LPUART0_RX/TPM0_CLKIN/LPSPI2_PCS3/FXIO0_D16, identifier: RX_GPIO,
    direction: INPUT}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : UART0_PollForActivity
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void UART0_PollForActivity(void)
{

    gpio_pin_config_t RX_GPIO_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTB16 (pin 62)  */
    GPIO_PinInit(UART0_POLLFORACTIVITY_RX_GPIO_GPIO, UART0_POLLFORACTIVITY_RX_GPIO_PIN, &RX_GPIO_config);

    /* PORTB16 (pin 62) is configured as PTB16 */
    PORT_SetPinMux(UART0_POLLFORACTIVITY_RX_GPIO_PORT, UART0_POLLFORACTIVITY_RX_GPIO_PIN, kPORT_MuxAsGpio);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
UART0_InitPins:
- options: {callFromInitBoot: 'false', prefix: UART0_, coreID: core0, enableClock: 'false'}
- pin_list:
  - {pin_num: '62', peripheral: LPUART0, signal: RX, pin_signal: TSI0_CH9/PTB16/LPSPI1_SOUT/LPUART0_RX/TPM0_CLKIN/LPSPI2_PCS3/FXIO0_D16, identifier: RX}
  - {pin_num: '63', peripheral: LPUART0, signal: TX, pin_signal: TSI0_CH10/PTB17/LPSPI1_SIN/LPUART0_TX/TPM1_CLKIN/LPSPI2_PCS2/FXIO0_D17, identifier: TX}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : UART0_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void UART0_InitPins(void)
{

    /* PORTB16 (pin 62) is configured as LPUART0_RX */
    PORT_SetPinMux(UART0_RX_PORT, UART0_RX_PIN, kPORT_MuxAlt3);

    /* PORTB17 (pin 63) is configured as LPUART0_TX */
    PORT_SetPinMux(UART0_TX_PORT, UART0_TX_PIN, kPORT_MuxAlt3);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
UART0_RestoreDefault:
- options: {callFromInitBoot: 'false', coreID: core0, enableClock: 'false'}
- pin_list:
  - {pin_num: '62', peripheral: TSI0, signal: 'CH, 9', pin_signal: TSI0_CH9/PTB16/LPSPI1_SOUT/LPUART0_RX/TPM0_CLKIN/LPSPI2_PCS3/FXIO0_D16}
  - {pin_num: '63', peripheral: TSI0, signal: 'CH, 10', pin_signal: TSI0_CH10/PTB17/LPSPI1_SIN/LPUART0_TX/TPM1_CLKIN/LPSPI2_PCS2/FXIO0_D17}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : UART0_RestoreDefault
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void UART0_RestoreDefault(void)
{

    /* PORTB16 (pin 62) is configured as TSI0_CH9 */
    PORT_SetPinMux(PORTB, 16U, kPORT_PinDisabledOrAnalog);

    /* PORTB17 (pin 63) is configured as TSI0_CH10 */
    PORT_SetPinMux(PORTB, 17U, kPORT_PinDisabledOrAnalog);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
I2C1_InitPins:
- options: {callFromInitBoot: 'false', prefix: I2C1_, coreID: core0, enableClock: 'false'}
- pin_list:
  - {pin_num: '71', peripheral: LPI2C1, signal: SCL, pin_signal: ADC0_SE15/TSI0_CH14/PTC1/LLWU_P6/LPI2C1_SCL/LPUART1_RTS_b/TPM0_CH0}
  - {pin_num: '72', peripheral: LPI2C1, signal: SDA, pin_signal: ADC0_SE11/CMP1_IN0/TSI0_CH15/PTC2/LPI2C1_SDA/LPUART1_CTS_b/TPM0_CH1}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : I2C1_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void I2C1_InitPins(void)
{

    /* PORTC1 (pin 71) is configured as LPI2C1_SCL */
    PORT_SetPinMux(I2C1_SCL_PORT, I2C1_SCL_PIN, kPORT_MuxAlt2);

    /* PORTC2 (pin 72) is configured as LPI2C1_SDA */
    PORT_SetPinMux(I2C1_SDA_PORT, I2C1_SDA_PIN, kPORT_MuxAlt2);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
I2C1_RestoreDefault:
- options: {callFromInitBoot: 'false', coreID: core0, enableClock: 'false'}
- pin_list:
  - {pin_num: '71', peripheral: ADC0, signal: 'SE, 15', pin_signal: ADC0_SE15/TSI0_CH14/PTC1/LLWU_P6/LPI2C1_SCL/LPUART1_RTS_b/TPM0_CH0}
  - {pin_num: '72', peripheral: ADC0, signal: 'SE, 11', pin_signal: ADC0_SE11/CMP1_IN0/TSI0_CH15/PTC2/LPI2C1_SDA/LPUART1_CTS_b/TPM0_CH1}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : I2C1_RestoreDefault
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void I2C1_RestoreDefault(void)
{

    /* PORTC1 (pin 71) is configured as ADC0_SE15 */
    PORT_SetPinMux(I2C1_RESTOREDEFAULT_SCL_PORT, I2C1_RESTOREDEFAULT_SCL_PIN, kPORT_PinDisabledOrAnalog);

    /* PORTC2 (pin 72) is configured as ADC0_SE11 */
    PORT_SetPinMux(I2C1_RESTOREDEFAULT_SDA_PORT, I2C1_RESTOREDEFAULT_SDA_PIN, kPORT_PinDisabledOrAnalog);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
SPI2_InitPins:
- options: {callFromInitBoot: 'false', prefix: SPI2_, coreID: core0, enableClock: 'false'}
- pin_list:
  - {pin_num: '66', peripheral: LPSPI2, signal: PCS0, pin_signal: PTB20/LPSPI2_PCS0/CMP0_OUT}
  - {pin_num: '67', peripheral: LPSPI2, signal: SCK, pin_signal: PTB21/LPSPI2_SCK/CMP1_OUT}
  - {pin_num: '68', peripheral: LPSPI2, signal: SOUT, pin_signal: PTB22/LPSPI2_SOUT}
  - {pin_num: '69', peripheral: LPSPI2, signal: SIN, pin_signal: PTB23/LPSPI2_SIN}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : SPI2_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void SPI2_InitPins(void)
{

    /* PORTB20 (pin 66) is configured as LPSPI2_PCS0 */
    PORT_SetPinMux(SPI2_PCS0_PORT, SPI2_PCS0_PIN, kPORT_MuxAlt2);

    /* PORTB21 (pin 67) is configured as LPSPI2_SCK */
    PORT_SetPinMux(SPI2_SCK_PORT, SPI2_SCK_PIN, kPORT_MuxAlt2);

    /* PORTB22 (pin 68) is configured as LPSPI2_SOUT */
    PORT_SetPinMux(SPI2_SOUT_PORT, SPI2_SOUT_PIN, kPORT_MuxAlt2);

    /* PORTB23 (pin 69) is configured as LPSPI2_SIN */
    PORT_SetPinMux(SPI2_SIN_PORT, SPI2_SIN_PIN, kPORT_MuxAlt2);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
SPI2_RestoreDefault:
- options: {callFromInitBoot: 'false', coreID: core0, enableClock: 'false'}
- pin_list:
  - {pin_num: '66', peripheral: n/a, signal: disabled, pin_signal: PTB20/LPSPI2_PCS0/CMP0_OUT}
  - {pin_num: '67', peripheral: n/a, signal: disabled, pin_signal: PTB21/LPSPI2_SCK/CMP1_OUT}
  - {pin_num: '68', peripheral: n/a, signal: disabled, pin_signal: PTB22/LPSPI2_SOUT}
  - {pin_num: '69', peripheral: n/a, signal: disabled, pin_signal: PTB23/LPSPI2_SIN}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : SPI2_RestoreDefault
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void SPI2_RestoreDefault(void)
{

    /* PORTB20 (pin 66) is disabled */
    PORT_SetPinMux(SPI2_RESTOREDEFAULT_PCS0_PORT, SPI2_RESTOREDEFAULT_PCS0_PIN, kPORT_PinDisabledOrAnalog);

    /* PORTB21 (pin 67) is disabled */
    PORT_SetPinMux(SPI2_RESTOREDEFAULT_SCK_PORT, SPI2_RESTOREDEFAULT_SCK_PIN, kPORT_PinDisabledOrAnalog);

    /* PORTB22 (pin 68) is disabled */
    PORT_SetPinMux(SPI2_RESTOREDEFAULT_SOUT_PORT, SPI2_RESTOREDEFAULT_SOUT_PIN, kPORT_PinDisabledOrAnalog);

    /* PORTB23 (pin 69) is disabled */
    PORT_SetPinMux(SPI2_RESTOREDEFAULT_SIN_PORT, SPI2_RESTOREDEFAULT_SIN_PIN, kPORT_PinDisabledOrAnalog);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
