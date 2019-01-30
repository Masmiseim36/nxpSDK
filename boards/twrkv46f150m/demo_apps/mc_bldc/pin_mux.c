/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

 
 /*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v3.0
processor: MKV46F256xxx16
package_id: MKV46F256VLL16
mcu_data: ksdk2_0
processor_version: 2.0.0
board: TWR-KV46F150M
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"



#define PIN0_IDX                         0u   /*!< Pin number for pin 0 in a port */
#define PIN1_IDX                         1u   /*!< Pin number for pin 1 in a port */
#define PIN2_IDX                         2u   /*!< Pin number for pin 2 in a port */
#define PIN3_IDX                         3u   /*!< Pin number for pin 3 in a port */
#define PIN4_IDX                         4u   /*!< Pin number for pin 4 in a port */
#define PIN5_IDX                         5u   /*!< Pin number for pin 5 in a port */
#define PIN6_IDX                         6u   /*!< Pin number for pin 6 in a port */
#define PIN14_IDX                       14u   /*!< Pin number for pin 14 in a port */
#define PIN15_IDX                       15u   /*!< Pin number for pin 15 in a port */
#define PIN16_IDX                       16u   /*!< Pin number for pin 16 in a port */
#define PIN17_IDX                       17u   /*!< Pin number for pin 17 in a port */
#define PIN24_IDX                       24u   /*!< Pin number for pin 24 in a port */
#define SOPT5_UART1RXSRC_UART_RX      0x00u   /*!< UART 1 receive data source select: UART1_RX pin */
#define SOPT5_UART1TXSRC_UART_TX      0x00u   /*!< UART 1 transmit data source select: UART1_TX pin */
#define SOPT8_FTM0OCH0SRC_FTM         0x00u   /*!< FTM0 channel 0 output source: FTM0_CH0 pin is output of FTM0 channel 0 output */
#define SOPT8_FTM0OCH1SRC_FTM         0x00u   /*!< FTM0 channel 1 output source: FTM0_CH1 pin is output of FTM0 channel 1 output */
#define SOPT8_FTM0OCH2SRC_FTM         0x00u   /*!< FTM0 channel 2 output source: FTM0_CH2 pin is output of FTM0 channel 2 output */
#define SOPT8_FTM0OCH3SRC_FTM         0x00u   /*!< FTM0 channel 3 output source: FTM0_CH3 pin is output of FTM0 channel 3 output */
#define SOPT8_FTM0OCH4SRC_FTM         0x00u   /*!< FTM0 channel 4 output source: FTM0_CH4 pin is output of FTM0 channel 4 output */
#define SOPT8_FTM0OCH5SRC_FTM         0x00u   /*!< FTM0 channel 5 output source: FTM0_CH5 pin is output of FTM0 channel 5 output */

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '1', peripheral: UART1, signal: TX, pin_signal: ADCB_CH6f/PTE0/UART1_TX/XBAROUT10/XBARIN11}
  - {pin_num: '2', peripheral: UART1, signal: RX, pin_signal: ADCB_CH7f/PTE1/LLWU_P0/UART1_RX/XBAROUT11/XBARIN7}
  - {pin_num: '7', peripheral: GPIOE, signal: 'GPIO, 6', pin_signal: PTE6/LLWU_P16/FTM3_CH1, slew_rate: fast, open_drain: disable, pull_select: up, pull_enable: enable}
  - {pin_num: '31', peripheral: GPIOE, signal: 'GPIO, 24', pin_signal: ADCB_CH4/PTE24/CAN1_TX/FTM0_CH0/XBARIN2/I2C0_SCL/EWM_OUT_b/XBAROUT4}
  - {pin_num: '93', peripheral: FTM0, signal: 'CH, 0', pin_signal: PTD0/LLWU_P12/SPI0_PCS0/FTM3_CH0/FTM0_CH0/FlexPWM_A0, direction: OUTPUT}
  - {pin_num: '94', peripheral: FTM0, signal: 'CH, 1', pin_signal: ADCA_CH7f/PTD1/SPI0_SCK/FTM3_CH1/FTM0_CH1/FlexPWM_B0, direction: OUTPUT}
  - {pin_num: '95', peripheral: FTM0, signal: 'CH, 2', pin_signal: PTD2/LLWU_P13/SPI0_SOUT/FTM3_CH2/FTM0_CH2/FlexPWM_A1/I2C0_SCL, direction: OUTPUT}
  - {pin_num: '96', peripheral: FTM0, signal: 'CH, 3', pin_signal: PTD3/SPI0_SIN/FTM3_CH3/FTM0_CH3/FlexPWM_B1/I2C0_SDA, direction: OUTPUT}
  - {pin_num: '97', peripheral: FTM0, signal: 'CH, 4', pin_signal: PTD4/LLWU_P14/SPI0_PCS1/UART0_RTS_b/FTM0_CH4/FlexPWM_A2/EWM_IN/SPI0_PCS0, direction: OUTPUT}
  - {pin_num: '98', peripheral: FTM0, signal: 'CH, 5', pin_signal: ADCA_CH6g/PTD5/SPI0_PCS2/UART0_CTS_b/UART0_COL_b/FTM0_CH5/FlexPWM_B2/EWM_OUT_b/SPI0_SCK, direction: OUTPUT}
  - {pin_num: '44', peripheral: SPI0, signal: PCS0_SS, pin_signal: CMP3_IN0/PTA14/SPI0_PCS0/UART0_TX}
  - {pin_num: '45', peripheral: SPI0, signal: SCK, pin_signal: CMP3_IN1/PTA15/SPI0_SCK/UART0_RX}
  - {pin_num: '46', peripheral: SPI0, signal: SOUT, pin_signal: CMP3_IN2/PTA16/SPI0_SOUT/UART0_CTS_b/UART0_COL_b}
  - {pin_num: '47', peripheral: SPI0, signal: SIN, pin_signal: ADCA_CH7e/PTA17/SPI0_SIN/UART0_RTS_b}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortA);                           /* Port A Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortD);                           /* Port D Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortE);                           /* Port E Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTA, PIN14_IDX, kPORT_MuxAlt2);           /* PORTA14 (pin 44) is configured as SPI0_PCS0 */
  PORT_SetPinMux(PORTA, PIN15_IDX, kPORT_MuxAlt2);           /* PORTA15 (pin 45) is configured as SPI0_SCK */
  PORT_SetPinMux(PORTA, PIN16_IDX, kPORT_MuxAlt2);           /* PORTA16 (pin 46) is configured as SPI0_SOUT */
  PORT_SetPinMux(PORTA, PIN17_IDX, kPORT_MuxAlt2);           /* PORTA17 (pin 47) is configured as SPI0_SIN */
  PORT_SetPinMux(PORTD, PIN0_IDX, kPORT_MuxAlt5);            /* PORTD0 (pin 93) is configured as FTM0_CH0 */
  PORT_SetPinMux(PORTD, PIN1_IDX, kPORT_MuxAlt5);            /* PORTD1 (pin 94) is configured as FTM0_CH1 */
  PORT_SetPinMux(PORTD, PIN2_IDX, kPORT_MuxAlt5);            /* PORTD2 (pin 95) is configured as FTM0_CH2 */
  PORT_SetPinMux(PORTD, PIN3_IDX, kPORT_MuxAlt5);            /* PORTD3 (pin 96) is configured as FTM0_CH3 */
  PORT_SetPinMux(PORTD, PIN4_IDX, kPORT_MuxAlt4);            /* PORTD4 (pin 97) is configured as FTM0_CH4 */
  PORT_SetPinMux(PORTD, PIN5_IDX, kPORT_MuxAlt4);            /* PORTD5 (pin 98) is configured as FTM0_CH5 */
  PORT_SetPinMux(PORTE, PIN0_IDX, kPORT_MuxAlt3);            /* PORTE0 (pin 1) is configured as UART1_TX */
  PORT_SetPinMux(PORTE, PIN1_IDX, kPORT_MuxAlt3);            /* PORTE1 (pin 2) is configured as UART1_RX */
  PORT_SetPinMux(PORTE, PIN24_IDX, kPORT_MuxAsGpio);         /* PORTE24 (pin 31) is configured as PTE24 */
  const port_pin_config_t porte6_pin7_config = {
    kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
    kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
    kPORT_OpenDrainDisable,                                  /* Open drain is disabled */
    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
    kPORT_MuxAsGpio,                                         /* Pin is configured as PTE6 */
    kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
  };
  PORT_SetPinConfig(PORTE, PIN6_IDX, &porte6_pin7_config);   /* PORTE6 (pin 7) is configured as PTE6 */
  SIM->SOPT5 = ((SIM->SOPT5 &
    (~(SIM_SOPT5_UART1TXSRC_MASK | SIM_SOPT5_UART1RXSRC_MASK))) /* Mask bits to zero which are setting */
      | SIM_SOPT5_UART1TXSRC(SOPT5_UART1TXSRC_UART_TX)       /* UART 1 transmit data source select: UART1_TX pin */
      | SIM_SOPT5_UART1RXSRC(SOPT5_UART1RXSRC_UART_RX)       /* UART 1 receive data source select: UART1_RX pin */
    );
  SIM->SOPT8 = ((SIM->SOPT8 &
    (~(SIM_SOPT8_FTM0OCH0SRC_MASK | SIM_SOPT8_FTM0OCH1SRC_MASK | SIM_SOPT8_FTM0OCH2SRC_MASK | SIM_SOPT8_FTM0OCH3SRC_MASK | SIM_SOPT8_FTM0OCH4SRC_MASK | SIM_SOPT8_FTM0OCH5SRC_MASK))) /* Mask bits to zero which are setting */
      | SIM_SOPT8_FTM0OCH0SRC(SOPT8_FTM0OCH0SRC_FTM)         /* FTM0 channel 0 output source: FTM0_CH0 pin is output of FTM0 channel 0 output */
      | SIM_SOPT8_FTM0OCH1SRC(SOPT8_FTM0OCH1SRC_FTM)         /* FTM0 channel 1 output source: FTM0_CH1 pin is output of FTM0 channel 1 output */
      | SIM_SOPT8_FTM0OCH2SRC(SOPT8_FTM0OCH2SRC_FTM)         /* FTM0 channel 2 output source: FTM0_CH2 pin is output of FTM0 channel 2 output */
      | SIM_SOPT8_FTM0OCH3SRC(SOPT8_FTM0OCH3SRC_FTM)         /* FTM0 channel 3 output source: FTM0_CH3 pin is output of FTM0 channel 3 output */
      | SIM_SOPT8_FTM0OCH4SRC(SOPT8_FTM0OCH4SRC_FTM)         /* FTM0 channel 4 output source: FTM0_CH4 pin is output of FTM0 channel 4 output */
      | SIM_SOPT8_FTM0OCH5SRC(SOPT8_FTM0OCH5SRC_FTM)         /* FTM0 channel 5 output source: FTM0_CH5 pin is output of FTM0 channel 5 output */
    );
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
