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
processor: MKL28Z512xxx7
package_id: MKL28Z512VLL7
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
  - {pin_num: '35', peripheral: LPUART0, signal: RX, pin_signal: TSI0_CH2/PTA1/LPUART0_RX/TPM2_CH0}
  - {pin_num: '36', peripheral: LPUART0, signal: TX, pin_signal: TSI0_CH3/PTA2/LPUART0_TX/TPM2_CH1}
  - {pin_num: '38', peripheral: GPIOA, signal: 'GPIO, 4', pin_signal: TSI0_CH5/PTA4/LLWU_P3/LPI2C1_SDA/TPM0_CH1/NMI0_b, slew_rate: slow, open_drain: disable, pull_select: up,
    pull_enable: enable}
  - {pin_num: '46', peripheral: GPIOA, signal: 'GPIO, 16', pin_signal: PTA16/LPSPI0_SOUT/LPUART0_CTS_b/I2S0_RX_FS/I2S0_RXD0}
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
    /* Clock Gate Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortA);

    /* PORTA1 (pin 35) is configured as LPUART0_RX */
    PORT_SetPinMux(PORTA, 1U, kPORT_MuxAlt2);

    /* PORTA16 (pin 46) is configured as PTA16 */
    PORT_SetPinMux(PORTA, 16U, kPORT_MuxAsGpio);

    /* PORTA2 (pin 36) is configured as LPUART0_TX */
    PORT_SetPinMux(PORTA, 2U, kPORT_MuxAlt2);

    /* PORTA4 (pin 38) is configured as PTA4 */
    PORT_SetPinMux(PORTA, 4U, kPORT_MuxAsGpio);

    PORTA->PCR[4] =
        ((PORTA->PCR[4] &
          /* Mask bits to zero which are setting */
          (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_SRE_MASK | PORT_PCR_ODE_MASK | PORT_PCR_ISF_MASK)))

         /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE
          * field is set. */
         | (uint32_t)(kPORT_PullUp)

         /* Slew Rate Enable: Slow slew rate is configured on the corresponding pin, if the pin is configured as a
          * digital output. */
         | PORT_PCR_SRE(kPORT_SlowSlewRate)

         /* Open Drain Enable: Open drain output is disabled on the corresponding pin. */
         | PORT_PCR_ODE(kPORT_OpenDrainDisable));
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
