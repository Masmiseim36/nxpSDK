/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
 
 /*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v3.0
processor: MKL28Z512xxx7
package_id: MKL28Z512VLL7
mcu_data: ksdk2_0
processor_version: 2.0.0
* BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"



#define PCR_ODE_DISABLED              0x00u   /*!< Open Drain Enable: Open drain output is disabled on the corresponding pin. */
#define PCR_PE_ENABLED                0x01u   /*!< Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
#define PCR_PS_UP                     0x01u   /*!< Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */
#define PCR_SRE_SLOW                  0x01u   /*!< Slew Rate Enable: Slow slew rate is configured on the corresponding pin, if the pin is configured as a digital output. */
#define PIN4_IDX                         4u   /*!< Pin number for pin 4 in a port */
#define PIN16_IDX                       16u   /*!< Pin number for pin 16 in a port */
#define PIN17_IDX                       17u   /*!< Pin number for pin 17 in a port */
#define PIN29_IDX                       29u   /*!< Pin number for pin 29 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '62', peripheral: LPUART0, signal: RX, pin_signal: TSI0_CH9/PTB16/LPSPI1_SOUT/LPUART0_RX/TPM0_CLKIN/LPSPI2_PCS3/FXIO0_D16}
  - {pin_num: '63', peripheral: LPUART0, signal: TX, pin_signal: TSI0_CH10/PTB17/LPSPI1_SIN/LPUART0_TX/TPM1_CLKIN/LPSPI2_PCS2/FXIO0_D17}
  - {pin_num: '38', peripheral: GPIOA, signal: 'GPIO, 4', pin_signal: TSI0_CH5/PTA4/LLWU_P3/LPI2C1_SDA/TPM0_CH1/NMI0_b, slew_rate: slow, open_drain: disable, pull_select: up,
    pull_enable: enable}
  - {pin_num: '26', peripheral: GPIOE, signal: 'GPIO, 29', pin_signal: CMP1_IN5/CMP0_IN5/ADC0_SE4b/PTE29/EMVSIM0_CLK/TPM0_CH2/TPM0_CLKIN}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortA);                           /* Clock Gate Control: 0x01u */
  CLOCK_EnableClock(kCLOCK_PortB);                           /* Clock Gate Control: 0x01u */
  CLOCK_EnableClock(kCLOCK_PortE);                           /* Clock Gate Control: 0x01u */

  PORT_SetPinMux(PORTA, PIN4_IDX, kPORT_MuxAsGpio);          /* PORTA4 (pin 38) is configured as PTA4 */
  PORTA->PCR[4] = ((PORTA->PCR[4] &
    (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_SRE_MASK | PORT_PCR_ODE_MASK | PORT_PCR_ISF_MASK))) /* Mask bits to zero which are setting */
      | PORT_PCR_PS(PCR_PS_UP)                               /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */
      | PORT_PCR_PE(PCR_PE_ENABLED)                          /* Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
      | PORT_PCR_SRE(PCR_SRE_SLOW)                           /* Slew Rate Enable: Slow slew rate is configured on the corresponding pin, if the pin is configured as a digital output. */
      | PORT_PCR_ODE(PCR_ODE_DISABLED)                       /* Open Drain Enable: Open drain output is disabled on the corresponding pin. */
    );
  PORT_SetPinMux(PORTB, PIN16_IDX, kPORT_MuxAlt3);           /* PORTB16 (pin 62) is configured as LPUART0_RX */
  PORT_SetPinMux(PORTB, PIN17_IDX, kPORT_MuxAlt3);           /* PORTB17 (pin 63) is configured as LPUART0_TX */
  PORT_SetPinMux(PORTE, PIN29_IDX, kPORT_MuxAsGpio);         /* PORTE29 (pin 26) is configured as PTE29 */
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
