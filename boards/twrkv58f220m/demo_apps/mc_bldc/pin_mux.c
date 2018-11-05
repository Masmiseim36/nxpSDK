/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
 /*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v3.0
processor: MKV58F1M0xxx24
package_id: MKV58F1M0VLQ24
mcu_data: ksdk2_0
processor_version: 2.0.0
board: TWR-KV58F220M
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "fsl_xbara.h"
#include "pin_mux.h"



#define PIN0_IDX                         0u   /*!< Pin number for pin 0 in a port */
#define PIN1_IDX                         1u   /*!< Pin number for pin 1 in a port */
#define PIN4_IDX                         4u   /*!< Pin number for pin 4 in a port */
#define PIN6_IDX                         6u   /*!< Pin number for pin 6 in a port */
#define PIN10_IDX                       10u   /*!< Pin number for pin 10 in a port */
#define PIN11_IDX                       11u   /*!< Pin number for pin 11 in a port */
#define PIN12_IDX                       12u   /*!< Pin number for pin 12 in a port */
#define PIN13_IDX                       13u   /*!< Pin number for pin 13 in a port */
#define PIN14_IDX                       14u   /*!< Pin number for pin 14 in a port */
#define PIN15_IDX                       15u   /*!< Pin number for pin 15 in a port */
#define PIN16_IDX                       16u   /*!< Pin number for pin 16 in a port */
#define PIN17_IDX                       17u   /*!< Pin number for pin 17 in a port */
#define PIN18_IDX                       18u   /*!< Pin number for pin 18 in a port */
#define PIN19_IDX                       19u   /*!< Pin number for pin 19 in a port */
#define SOPT5_UART0RXSRC_UART_RX      0x00u   /*!< UART 0 receive data source select: UART0_RX pin */
#define SOPT5_UART0TXSRC_UART_TX      0x00u   /*!< UART 0 transmit data source select: UART0_TX pin */
#define SOPT7_HSADC0AALTTRGEN_XBAR    0x00u   /*!< HSADC0A alternate trigger enable: XBARA output 12. */
#define SOPT7_HSADC1ATRGSEL_XBAR      0x0Cu   /*!< HSADC1A trigger select: XBARA output 41 */

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '81', peripheral: UART0, signal: RX, pin_signal: HSADC0B_CH2/PTB0/LLWU_P5/I2C0_SCL/FTM1_CH0/FTM1_QD_PHA/UART0_RX/RMII0_MDIO/MII0_MDIO}
  - {pin_num: '82', peripheral: UART0, signal: TX, pin_signal: HSADC0B_CH3/PTB1/I2C0_SDA/FTM1_CH1/FTM0_FLT2/EWM_IN/FTM1_QD_PHB/UART0_TX/RMII0_MDC/MII0_MDC}
  - {pin_num: '54', peripheral: GPIOA, signal: 'GPIO, 4', pin_signal: PTA4/LLWU_P3/FTM0_CH1/XB_IN10/FTM0_FLT3/FLEXPWM0_B0/NMI_b, slew_rate: fast, open_drain: disable,
    pull_select: up, pull_enable: enable, passive_filter: disable}
  - {pin_num: '87', peripheral: GPIOB, signal: 'GPIO, 6', pin_signal: HSADC1A_CH12/PTB6/CAN2_TX/FLEXPWM1_X2/FB_AD23}
  - {pin_num: '139', peripheral: PWM0, signal: 'A, 2', pin_signal: PTD10/UART5_RTS_b/FLEXPWM0_A2/FB_A18}
  - {pin_num: '140', peripheral: PWM0, signal: 'B, 2', pin_signal: PTD11/LLWU_P25/SPI2_PCS0/UART5_CTS_b/FLEXPWM0_B2/FB_A19}
  - {pin_num: '141', peripheral: PWM0, signal: 'A, 1', pin_signal: PTD12/SPI2_SCK/FTM3_FLT0/XB_IN5/XB_OUT5/FLEXPWM0_A1/FB_A20}
  - {pin_num: '142', peripheral: PWM0, signal: 'B, 1', pin_signal: PTD13/SPI2_SOUT/XB_IN7/XB_OUT7/FLEXPWM0_B1/FB_A21}
  - {pin_num: '143', peripheral: PWM0, signal: 'A, 0', pin_signal: PTD14/SPI2_SIN/XB_IN11/XB_OUT11/FLEXPWM0_A0/FB_A22}
  - {pin_num: '144', peripheral: PWM0, signal: 'B, 0', pin_signal: PTD15/SPI2_PCS1/FLEXPWM0_B0/FB_A23}
  - {pin_num: '18', peripheral: SPI0, signal: PCS0_SS, pin_signal: HSADC0A_CH0/ADC0_SE1/ADC0_DP1/PTE16/SPI0_PCS0/UART2_TX/FTM_CLKIN0/FTM0_FLT3}
  - {pin_num: '19', peripheral: SPI0, signal: SCK, pin_signal: HSADC0A_CH1/ADC0_SE9/ADC0_DM1/PTE17/LLWU_P19/SPI0_SCK/UART2_RX/FTM_CLKIN1/LPTMR0_ALT3}
  - {pin_num: '20', peripheral: SPI0, signal: SOUT, pin_signal: HSADC0B_CH0/ADC0_SE5a/PTE18/LLWU_P20/SPI0_SOUT/UART2_CTS_b/I2C0_SDA}
  - {pin_num: '21', peripheral: SPI0, signal: SIN, pin_signal: HSADC0B_CH1/ADC0_SE6a/PTE19/SPI0_SIN/UART2_RTS_b/I2C0_SCL/CMP3_OUT}
  - {peripheral: HSADC0, signal: 'TRG, A', pin_signal: PWM0_CH0_TRG0}
  - {peripheral: HSADC1, signal: 'TRG, A', pin_signal: PWM0_CH0_TRG0}
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
  CLOCK_EnableClock(kCLOCK_PortB);                           /* Port B Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortD);                           /* Port D Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortE);                           /* Port E Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_XbarA);                           /* XBARA Clock Gate Control: Clock enabled */

  const port_pin_config_t porta4_pin54_config = {
    kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
    kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
    kPORT_OpenDrainDisable,                                  /* Open drain is disabled */
    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
    kPORT_MuxAsGpio,                                         /* Pin is configured as PTA4 */
    kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
  };
  PORT_SetPinConfig(PORTA, PIN4_IDX, &porta4_pin54_config);  /* PORTA4 (pin 54) is configured as PTA4 */
  PORT_SetPinMux(PORTB, PIN0_IDX, kPORT_MuxAlt7);            /* PORTB0 (pin 81) is configured as UART0_RX */
  PORT_SetPinMux(PORTB, PIN1_IDX, kPORT_MuxAlt7);            /* PORTB1 (pin 82) is configured as UART0_TX */
  PORT_SetPinMux(PORTB, PIN6_IDX, kPORT_MuxAsGpio);          /* PORTB6 (pin 87) is configured as PTB6 */
  PORT_SetPinMux(PORTD, PIN10_IDX, kPORT_MuxAlt6);           /* PORTD10 (pin 139) is configured as FLEXPWM0_A2 */
  PORT_SetPinMux(PORTD, PIN11_IDX, kPORT_MuxAlt6);           /* PORTD11 (pin 140) is configured as FLEXPWM0_B2 */
  PORT_SetPinMux(PORTD, PIN12_IDX, kPORT_MuxAlt6);           /* PORTD12 (pin 141) is configured as FLEXPWM0_A1 */
  PORT_SetPinMux(PORTD, PIN13_IDX, kPORT_MuxAlt6);           /* PORTD13 (pin 142) is configured as FLEXPWM0_B1 */
  PORT_SetPinMux(PORTD, PIN14_IDX, kPORT_MuxAlt6);           /* PORTD14 (pin 143) is configured as FLEXPWM0_A0 */
  PORT_SetPinMux(PORTD, PIN15_IDX, kPORT_MuxAlt6);           /* PORTD15 (pin 144) is configured as FLEXPWM0_B0 */
  PORT_SetPinMux(PORTE, PIN16_IDX, kPORT_MuxAlt2);           /* PORTE16 (pin 18) is configured as SPI0_PCS0 */
  PORT_SetPinMux(PORTE, PIN17_IDX, kPORT_MuxAlt2);           /* PORTE17 (pin 19) is configured as SPI0_SCK */
  PORT_SetPinMux(PORTE, PIN18_IDX, kPORT_MuxAlt2);           /* PORTE18 (pin 20) is configured as SPI0_SOUT */
  PORT_SetPinMux(PORTE, PIN19_IDX, kPORT_MuxAlt2);           /* PORTE19 (pin 21) is configured as SPI0_SIN */
  SIM->SOPT5 = ((SIM->SOPT5 &
    (~(SIM_SOPT5_UART0TXSRC_MASK | SIM_SOPT5_UART0RXSRC_MASK))) /* Mask bits to zero which are setting */
      | SIM_SOPT5_UART0TXSRC(SOPT5_UART0TXSRC_UART_TX)       /* UART 0 transmit data source select: UART0_TX pin */
      | SIM_SOPT5_UART0RXSRC(SOPT5_UART0RXSRC_UART_RX)       /* UART 0 receive data source select: UART0_RX pin */
    );
  SIM->SOPT7 = ((SIM->SOPT7 &
    (~(SIM_SOPT7_HSADC0AALTTRGEN_MASK | SIM_SOPT7_HSADC1ATRGSEL_MASK | SIM_SOPT7_HSADC1AALTTRGEN_MASK))) /* Mask bits to zero which are setting */
      | SIM_SOPT7_HSADC0AALTTRGEN(SOPT7_HSADC0AALTTRGEN_XBAR) /* HSADC0A alternate trigger enable: XBARA output 12. */
      | SIM_SOPT7_HSADC1ATRGSEL(SOPT7_HSADC1ATRGSEL_XBAR)    /* HSADC1A trigger select: XBARA output 41 */
      | SIM_SOPT7_HSADC1AALTTRGEN(0x02u)                     /* HSADC1A alternate trigger enable: 0x02u */
    );
  XBARA_SetSignalsConnection(XBARA, kXBARA_InputPwm0Ch0Trg0, kXBARA_OutputPdb1InCh12); /* PWMA channel 0 trigger 0 output assigned to XBARA_IN20 input is connected to XBARA_OUT41 output assigned to PDB1 trigger option 12 */
  XBARA_SetSignalsConnection(XBARA, kXBARA_InputPwm0Ch0Trg0, kXBARA_OutputHsadc0ATrig); /* PWMA channel 0 trigger 0 output assigned to XBARA_IN20 input is connected to XBARA_OUT12 output assigned to HSADC0 converter A trigger */
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
