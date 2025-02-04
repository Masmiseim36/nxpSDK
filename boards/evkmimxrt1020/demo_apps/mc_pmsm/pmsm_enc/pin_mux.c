/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v11.0
processor: MIMXRT1021xxxxx
package_id: MIMXRT1021DAG5A
mcu_data: ksdk2_0
processor_version: 11.0.1
board: MIMXRT1020-EVK
pin_labels:
- {pin_num: '81', pin_signal: GPIO_AD_B1_09, label: 'UART_RX/J17[1]', identifier: Trigger}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

#include "fsl_common.h"
#include "fsl_xbara.h"
#include "fsl_iomuxc.h"
#include "pin_mux.h"

/* FUNCTION ************************************************************************************************************
 * 
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 * 
 * END ****************************************************************************************************************/
void BOARD_InitBootPins(void) {
    BOARD_InitADC();
    BOARD_InitLPUART();
    BOARD_InitPWM();
    BOARD_Misc();
    BOARD_InitCMP();
    BOARD_InitENC();
}

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitADC:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '79', peripheral: ADC1, signal: 'IN, 11', pin_signal: GPIO_AD_B1_11, pull_keeper_enable: Disable}
  - {pin_num: '78', peripheral: ADC1, signal: 'IN, 12', pin_signal: GPIO_AD_B1_12, pull_keeper_enable: Disable}
  - {pin_num: '76', peripheral: ADC1, signal: 'IN, 13', pin_signal: GPIO_AD_B1_13, pull_keeper_enable: Disable}
  - {pin_num: '79', peripheral: ADC2, signal: 'IN, 11', pin_signal: GPIO_AD_B1_11}
  - {pin_num: '78', peripheral: ADC2, signal: 'IN, 12', pin_signal: GPIO_AD_B1_12}
  - {pin_num: '76', peripheral: ADC2, signal: 'IN, 13', pin_signal: GPIO_AD_B1_13, identifier: ''}
  - {pin_num: '80', peripheral: ADC1, signal: 'IN, 10', pin_signal: GPIO_AD_B1_10, pull_keeper_enable: Disable}
  - {pin_num: '80', peripheral: ADC2, signal: 'IN, 10', pin_signal: GPIO_AD_B1_10}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitADC
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitADC(void) {
  CLOCK_EnableClock(kCLOCK_Iomuxc);           

  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_10_GPIO1_IO26, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_11_GPIO1_IO27, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_12_GPIO1_IO28, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_13_GPIO1_IO29, 0U); 
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_10_GPIO1_IO26, 0xB0U); 
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_11_GPIO1_IO27, 0xB0U); 
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_12_GPIO1_IO28, 0xB0U); 
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_13_GPIO1_IO29, 0xB0U); 
}


/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitLPUART:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '105', peripheral: LPUART1, signal: TX, pin_signal: GPIO_AD_B0_06, identifier: ''}
  - {pin_num: '101', peripheral: LPUART1, signal: RX, pin_signal: GPIO_AD_B0_07}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitLPUART
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitLPUART(void) {
  CLOCK_EnableClock(kCLOCK_Iomuxc);           

  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_06_LPUART1_TX, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_07_LPUART1_RX, 0U); 
}


/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPWM:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '94', peripheral: PWM2, signal: 'A, 0', pin_signal: GPIO_AD_B0_14}
  - {pin_num: '96', peripheral: PWM2, signal: 'A, 1', pin_signal: GPIO_AD_B0_12}
  - {pin_num: '98', peripheral: PWM2, signal: 'A, 2', pin_signal: GPIO_AD_B0_10}
  - {pin_num: '93', peripheral: PWM2, signal: 'B, 0', pin_signal: GPIO_AD_B0_15}
  - {pin_num: '95', peripheral: PWM2, signal: 'B, 1', pin_signal: GPIO_AD_B0_13}
  - {pin_num: '97', peripheral: PWM2, signal: 'B, 2', pin_signal: GPIO_AD_B0_11}
  - {peripheral: PWM2, signal: 'FAULT, 0', pin_signal: ACMP2_OUT}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPWM
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitPWM(void) {
  CLOCK_EnableClock(kCLOCK_Iomuxc);           
  CLOCK_EnableClock(kCLOCK_Xbar1);            

  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_10_FLEXPWM2_PWMA02, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_11_FLEXPWM2_PWMB02, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_12_FLEXPWM2_PWMA01, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_13_FLEXPWM2_PWMB01, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_14_FLEXPWM2_PWMA00, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_15_FLEXPWM2_PWMB00, 0U); 
  XBARA_SetSignalsConnection(XBARA, kXBARA1_InputAcmp2Out, kXBARA1_OutputFlexpwm2Fault0); 
}


/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_Misc:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '99', peripheral: GPIO1, signal: 'gpio_io, 09', pin_signal: GPIO_AD_B0_09}
  - {pin_num: '52', peripheral: GPIO5, signal: 'gpio_io, 00', pin_signal: WAKEUP}
  - {pin_num: '106', peripheral: GPIO1, signal: 'gpio_io, 05', pin_signal: GPIO_AD_B0_05}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_Misc
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_Misc(void) {
  CLOCK_EnableClock(kCLOCK_Iomuxc);           
  CLOCK_EnableClock(kCLOCK_IomuxcSnvs);       

  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_05_GPIO1_IO05, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_09_GPIO1_IO09, 0U); 
  IOMUXC_SetPinMux(IOMUXC_SNVS_WAKEUP_GPIO5_IO00, 0U); 
}


/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitCMP:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '99', peripheral: CMP2, signal: 'IN, 4', pin_signal: GPIO_AD_B0_09}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitCMP
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitCMP(void) {
  CLOCK_EnableClock(kCLOCK_Iomuxc);           

  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_09_GPIO1_IO09, 0U); 
}


/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitENC:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '82', peripheral: ENC1, signal: 'PHASE, A', pin_signal: GPIO_AD_B1_08}
  - {pin_num: '81', peripheral: ENC1, signal: 'PHASE, B', pin_signal: GPIO_AD_B1_09}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitENC
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitENC(void) {
  CLOCK_EnableClock(kCLOCK_Iomuxc);           
  CLOCK_EnableClock(kCLOCK_Xbar1);            

  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_08_XBAR1_INOUT12, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_09_XBAR1_INOUT13, 0U); 
  IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
    (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_12_MASK | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_13_MASK))) 
      | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_12(0x00U) 
      | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_13(0x00U) 
    );
  XBARA_SetSignalsConnection(XBARA, kXBARA1_InputIomuxXbarInout12, kXBARA1_OutputEnc1PhaseAInput); 
  XBARA_SetSignalsConnection(XBARA, kXBARA1_InputIomuxXbarInout13, kXBARA1_OutputEnc1PhaseBInput); 
}

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
