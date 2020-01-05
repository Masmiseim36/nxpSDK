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
processor: MK82FN256xxx15
package_id: MK82FN256VLL15
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
  - {pin_num: '82', peripheral: GPIOC, signal: 'GPIO, 10', pin_signal: PTC10/I2C1_SCL/FTM3_CH6/I2S0_RX_FS/FB_AD5/SDRAM_A13/FXIO0_D18, direction: INPUT, slew_rate: fast,
    open_drain: disable, pull_select: up, pull_enable: enable}
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
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    const port_pin_config_t portc10_pin82_config = {/* Internal pull-up resistor is enabled */
                                                    kPORT_PullUp,
                                                    /* Fast slew rate is configured */
                                                    kPORT_FastSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Open drain is disabled */
                                                    kPORT_OpenDrainDisable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as PTC10 */
                                                    kPORT_MuxAsGpio,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    kPORT_UnlockRegister};
    /* PORTC10 (pin 82) is configured as PTC10 */
    PORT_SetPinConfig(PORTC, 10U, &portc10_pin82_config);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitButtons:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '40', peripheral: GPIOA, signal: 'GPIO, 4', pin_signal: TSI0_CH5/PTA4/LLWU_P3/FTM0_CH1/FXIO0_D14/EMVSIM0_VCCEN/NMI_b, direction: INPUT, slew_rate: slow,
    open_drain: disable, pull_select: down, pull_enable: disable, passive_filter: disable}
  - {pin_num: '78', peripheral: GPIOC, signal: 'GPIO, 6', pin_signal: CMP0_IN0/PTC6/LLWU_P10/SPI0_SOUT/PDB0_EXTRG/I2S0_RX_BCLK/FB_AD9/SDRAM_A17/I2S0_MCLK/FXIO0_D14,
    direction: INPUT, slew_rate: slow, open_drain: disable, pull_select: down, pull_enable: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitButtons
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitButtons(void)
{
    /* Port A Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    const port_pin_config_t porta4_pin40_config = {/* Internal pull-up/down resistor is disabled */
                                                   kPORT_PullDisable,
                                                   /* Slow slew rate is configured */
                                                   kPORT_SlowSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Open drain is disabled */
                                                   kPORT_OpenDrainDisable,
                                                   /* Low drive strength is configured */
                                                   kPORT_LowDriveStrength,
                                                   /* Pin is configured as PTA4 */
                                                   kPORT_MuxAsGpio,
                                                   /* Pin Control Register fields [15:0] are not locked */
                                                   kPORT_UnlockRegister};
    /* PORTA4 (pin 40) is configured as PTA4 */
    PORT_SetPinConfig(PORTA, 4U, &porta4_pin40_config);

    const port_pin_config_t portc6_pin78_config = {/* Internal pull-up/down resistor is disabled */
                                                   kPORT_PullDisable,
                                                   /* Slow slew rate is configured */
                                                   kPORT_SlowSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Open drain is disabled */
                                                   kPORT_OpenDrainDisable,
                                                   /* Low drive strength is configured */
                                                   kPORT_LowDriveStrength,
                                                   /* Pin is configured as PTC6 */
                                                   kPORT_MuxAsGpio,
                                                   /* Pin Control Register fields [15:0] are not locked */
                                                   kPORT_UnlockRegister};
    /* PORTC6 (pin 78) is configured as PTC6 */
    PORT_SetPinConfig(PORTC, 6U, &portc6_pin78_config);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitOSC:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '50', peripheral: OSC, signal: EXTAL0, pin_signal: EXTAL0/PTA18/FTM0_FLT2/FTM_CLKIN0/TPM_CLKIN0, slew_rate: no_init, open_drain: no_init, pull_select: no_init,
    pull_enable: no_init}
  - {pin_num: '51', peripheral: OSC, signal: XTAL0, pin_signal: XTAL0/PTA19/FTM1_FLT0/FTM_CLKIN1/LPTMR0_ALT1/LPTMR1_ALT1/TPM_CLKIN1, slew_rate: no_init, open_drain: no_init,
    pull_select: no_init, pull_enable: no_init}
  - {pin_num: '31', peripheral: RTC, signal: XTAL32, pin_signal: XTAL32}
  - {pin_num: '32', peripheral: RTC, signal: EXTAL32, pin_signal: EXTAL32}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitOSC
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitOSC(void)
{
    /* Port A Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);

    /* PORTA18 (pin 50) is configured as EXTAL0 */
    PORT_SetPinMux(PORTA, 18U, kPORT_PinDisabledOrAnalog);

    /* PORTA19 (pin 51) is configured as XTAL0 */
    PORT_SetPinMux(PORTA, 19U, kPORT_PinDisabledOrAnalog);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitLEDs:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '80', peripheral: GPIOC, signal: 'GPIO, 8', pin_signal: CMP0_IN2/PTC8/FTM3_CH4/I2S0_MCLK/FB_AD7/SDRAM_A15/FXIO0_D16, direction: OUTPUT, slew_rate: slow,
    open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: '81', peripheral: GPIOC, signal: 'GPIO, 9', pin_signal: CMP0_IN3/PTC9/FTM3_CH5/I2S0_RX_BCLK/FB_AD6/SDRAM_A14/FTM2_FLT0/FXIO0_D17, direction: OUTPUT,
    slew_rate: slow, open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: '82', peripheral: GPIOC, signal: 'GPIO, 10', pin_signal: PTC10/I2C1_SCL/FTM3_CH6/I2S0_RX_FS/FB_AD5/SDRAM_A13/FXIO0_D18, direction: OUTPUT, slew_rate: slow,
    open_drain: disable, pull_select: down, pull_enable: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitLEDs
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitLEDs(void)
{
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    const port_pin_config_t portc10_pin82_config = {/* Internal pull-up/down resistor is disabled */
                                                    kPORT_PullDisable,
                                                    /* Slow slew rate is configured */
                                                    kPORT_SlowSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Open drain is disabled */
                                                    kPORT_OpenDrainDisable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as PTC10 */
                                                    kPORT_MuxAsGpio,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    kPORT_UnlockRegister};
    /* PORTC10 (pin 82) is configured as PTC10 */
    PORT_SetPinConfig(PORTC, 10U, &portc10_pin82_config);

    const port_pin_config_t portc8_pin80_config = {/* Internal pull-up/down resistor is disabled */
                                                   kPORT_PullDisable,
                                                   /* Slow slew rate is configured */
                                                   kPORT_SlowSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Open drain is disabled */
                                                   kPORT_OpenDrainDisable,
                                                   /* Low drive strength is configured */
                                                   kPORT_LowDriveStrength,
                                                   /* Pin is configured as PTC8 */
                                                   kPORT_MuxAsGpio,
                                                   /* Pin Control Register fields [15:0] are not locked */
                                                   kPORT_UnlockRegister};
    /* PORTC8 (pin 80) is configured as PTC8 */
    PORT_SetPinConfig(PORTC, 8U, &portc8_pin80_config);

    const port_pin_config_t portc9_pin81_config = {/* Internal pull-up/down resistor is disabled */
                                                   kPORT_PullDisable,
                                                   /* Slow slew rate is configured */
                                                   kPORT_SlowSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Open drain is disabled */
                                                   kPORT_OpenDrainDisable,
                                                   /* Low drive strength is configured */
                                                   kPORT_LowDriveStrength,
                                                   /* Pin is configured as PTC9 */
                                                   kPORT_MuxAsGpio,
                                                   /* Pin Control Register fields [15:0] are not locked */
                                                   kPORT_UnlockRegister};
    /* PORTC9 (pin 81) is configured as PTC9 */
    PORT_SetPinConfig(PORTC, 9U, &portc9_pin81_config);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitDEBUG_UART:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '86', peripheral: LPUART4, signal: RX, pin_signal: PTC14/LPUART4_RX/FB_AD25/SDRAM_D25/FXIO0_D20, slew_rate: fast, open_drain: disable, pull_select: down,
    pull_enable: disable}
  - {pin_num: '87', peripheral: LPUART4, signal: TX, pin_signal: PTC15/LPUART4_TX/FB_AD24/SDRAM_D24/FXIO0_D21, direction: OUTPUT, slew_rate: fast, open_drain: disable,
    pull_select: down, pull_enable: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitDEBUG_UART
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitDEBUG_UART(void)
{
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    const port_pin_config_t portc14_pin86_config = {/* Internal pull-up/down resistor is disabled */
                                                    kPORT_PullDisable,
                                                    /* Fast slew rate is configured */
                                                    kPORT_FastSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Open drain is disabled */
                                                    kPORT_OpenDrainDisable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as LPUART4_RX */
                                                    kPORT_MuxAlt3,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    kPORT_UnlockRegister};
    /* PORTC14 (pin 86) is configured as LPUART4_RX */
    PORT_SetPinConfig(PORTC, 14U, &portc14_pin86_config);

    const port_pin_config_t portc15_pin87_config = {/* Internal pull-up/down resistor is disabled */
                                                    kPORT_PullDisable,
                                                    /* Fast slew rate is configured */
                                                    kPORT_FastSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Open drain is disabled */
                                                    kPORT_OpenDrainDisable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as LPUART4_TX */
                                                    kPORT_MuxAlt3,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    kPORT_UnlockRegister};
    /* PORTC15 (pin 87) is configured as LPUART4_TX */
    PORT_SetPinConfig(PORTC, 15U, &portc15_pin87_config);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitACCEL:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '37', peripheral: I2C3, signal: SDA, pin_signal: TSI0_CH2/PTA1/LPUART0_RX/FTM0_CH6/I2C3_SDA/FXIO0_D11/EMVSIM0_IO/JTAG_TDI, slew_rate: fast, open_drain: disable,
    pull_select: up, pull_enable: enable}
  - {pin_num: '38', peripheral: I2C3, signal: SCL, pin_signal: TSI0_CH3/PTA2/LPUART0_TX/FTM0_CH7/I2C3_SCL/FXIO0_D12/EMVSIM0_PD/JTAG_TDO/TRACE_SWO, slew_rate: fast,
    open_drain: disable, pull_select: up, pull_enable: enable}
  - {pin_num: '85', peripheral: GPIOC, signal: 'GPIO, 13', pin_signal: PTC13/LPUART4_CTS_b/FTM_CLKIN1/FB_AD26/SDRAM_D26/TPM_CLKIN1, direction: INPUT, slew_rate: slow,
    open_drain: enable, pull_select: down, pull_enable: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitACCEL
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitACCEL(void)
{
    /* Port A Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    const port_pin_config_t porta1_pin37_config = {/* Internal pull-up resistor is enabled */
                                                   kPORT_PullUp,
                                                   /* Fast slew rate is configured */
                                                   kPORT_FastSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Open drain is disabled */
                                                   kPORT_OpenDrainDisable,
                                                   /* Low drive strength is configured */
                                                   kPORT_LowDriveStrength,
                                                   /* Pin is configured as I2C3_SDA */
                                                   kPORT_MuxAlt4,
                                                   /* Pin Control Register fields [15:0] are not locked */
                                                   kPORT_UnlockRegister};
    /* PORTA1 (pin 37) is configured as I2C3_SDA */
    PORT_SetPinConfig(PORTA, 1U, &porta1_pin37_config);

    const port_pin_config_t porta2_pin38_config = {/* Internal pull-up resistor is enabled */
                                                   kPORT_PullUp,
                                                   /* Fast slew rate is configured */
                                                   kPORT_FastSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Open drain is disabled */
                                                   kPORT_OpenDrainDisable,
                                                   /* Low drive strength is configured */
                                                   kPORT_LowDriveStrength,
                                                   /* Pin is configured as I2C3_SCL */
                                                   kPORT_MuxAlt4,
                                                   /* Pin Control Register fields [15:0] are not locked */
                                                   kPORT_UnlockRegister};
    /* PORTA2 (pin 38) is configured as I2C3_SCL */
    PORT_SetPinConfig(PORTA, 2U, &porta2_pin38_config);

    const port_pin_config_t portc13_pin85_config = {/* Internal pull-up/down resistor is disabled */
                                                    kPORT_PullDisable,
                                                    /* Slow slew rate is configured */
                                                    kPORT_SlowSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Open drain is enabled */
                                                    kPORT_OpenDrainEnable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as PTC13 */
                                                    kPORT_MuxAsGpio,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    kPORT_UnlockRegister};
    /* PORTC13 (pin 85) is configured as PTC13 */
    PORT_SetPinConfig(PORTC, 13U, &portc13_pin85_config);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitQSPI_FLASH:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '1', peripheral: QuadSPI0, signal: 'DATA3, A', pin_signal: PTE0/SPI1_PCS1/LPUART1_TX/SDHC0_D1/QSPI0A_DATA3/I2C1_SDA/RTC_CLKOUT, slew_rate: fast, open_drain: disable,
    drive_strength: low, pull_select: down, pull_enable: disable}
  - {pin_num: '2', peripheral: QuadSPI0, signal: 'SCLK, A', pin_signal: PTE1/LLWU_P0/SPI1_SCK/LPUART1_RX/SDHC0_D0/QSPI0A_SCLK/I2C1_SCL/SPI1_SIN, slew_rate: fast,
    open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable}
  - {pin_num: '3', peripheral: QuadSPI0, signal: 'DATA0, A', pin_signal: PTE2/LLWU_P1/SPI1_SOUT/LPUART1_CTS_b/SDHC0_DCLK/QSPI0A_DATA0/SPI1_SCK, slew_rate: fast, open_drain: disable,
    drive_strength: low, pull_select: down, pull_enable: disable}
  - {pin_num: '4', peripheral: QuadSPI0, signal: 'DATA2, A', pin_signal: PTE3/SPI1_PCS2/LPUART1_RTS_b/SDHC0_CMD/QSPI0A_DATA2/SPI1_SOUT, slew_rate: fast, open_drain: disable,
    drive_strength: low, pull_select: down, pull_enable: disable}
  - {pin_num: '7', peripheral: QuadSPI0, signal: 'DATA1, A', pin_signal: PTE4/LLWU_P2/SPI1_SIN/LPUART3_TX/SDHC0_D3/QSPI0A_DATA1, slew_rate: fast, open_drain: disable,
    drive_strength: low, pull_select: down, pull_enable: disable}
  - {pin_num: '8', peripheral: QuadSPI0, signal: 'SS0, A', pin_signal: PTE5/SPI1_PCS0/LPUART3_RX/SDHC0_D2/QSPI0A_SS0_B/FTM3_CH0/USB0_SOF_OUT, slew_rate: fast, open_drain: disable,
    drive_strength: low, pull_select: down, pull_enable: disable}
  - {pin_num: '9', peripheral: QuadSPI0, signal: 'DATA3, B', pin_signal: PTE6/LLWU_P16/SPI1_PCS3/LPUART3_CTS_b/I2S0_MCLK/QSPI0B_DATA3/FTM3_CH1/SDHC0_D4, slew_rate: fast,
    open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable}
  - {pin_num: '10', peripheral: QuadSPI0, signal: 'SCLK, B', pin_signal: PTE7/SPI2_SCK/LPUART3_RTS_b/I2S0_RXD0/QSPI0B_SCLK/FTM3_CH2/QSPI0A_SS1_B, slew_rate: fast,
    open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable}
  - {pin_num: '11', peripheral: QuadSPI0, signal: 'DATA0, B', pin_signal: PTE8/I2S0_RXD1/SPI2_SOUT/I2S0_RX_FS/QSPI0B_DATA0/FTM3_CH3/SDHC0_D5, slew_rate: fast, open_drain: disable,
    drive_strength: low, pull_select: down, pull_enable: disable}
  - {pin_num: '12', peripheral: QuadSPI0, signal: 'DATA2, B', pin_signal: PTE9/LLWU_P17/I2S0_TXD1/SPI2_PCS1/I2S0_RX_BCLK/QSPI0B_DATA2/FTM3_CH4/SDHC0_D6, slew_rate: fast,
    open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable}
  - {pin_num: '13', peripheral: QuadSPI0, signal: 'DATA1, B', pin_signal: PTE10/LLWU_P18/I2C3_SDA/SPI2_SIN/I2S0_TXD0/QSPI0B_DATA1/FTM3_CH5/SDHC0_D7, slew_rate: fast,
    open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable}
  - {pin_num: '14', peripheral: QuadSPI0, signal: 'SS0, B', pin_signal: PTE11/I2C3_SCL/SPI2_PCS0/I2S0_TX_FS/QSPI0B_SS0_B/FTM3_CH6/QSPI0A_DQS, slew_rate: fast, open_drain: disable,
    drive_strength: low, pull_select: down, pull_enable: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitQSPI_FLASH
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitQSPI_FLASH(void)
{
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    const port_pin_config_t porte0_pin1_config = {/* Internal pull-up/down resistor is disabled */
                                                  kPORT_PullDisable,
                                                  /* Fast slew rate is configured */
                                                  kPORT_FastSlewRate,
                                                  /* Passive filter is disabled */
                                                  kPORT_PassiveFilterDisable,
                                                  /* Open drain is disabled */
                                                  kPORT_OpenDrainDisable,
                                                  /* Low drive strength is configured */
                                                  kPORT_LowDriveStrength,
                                                  /* Pin is configured as QSPI0A_DATA3 */
                                                  kPORT_MuxAlt5,
                                                  /* Pin Control Register fields [15:0] are not locked */
                                                  kPORT_UnlockRegister};
    /* PORTE0 (pin 1) is configured as QSPI0A_DATA3 */
    PORT_SetPinConfig(PORTE, 0U, &porte0_pin1_config);

    const port_pin_config_t porte1_pin2_config = {/* Internal pull-up/down resistor is disabled */
                                                  kPORT_PullDisable,
                                                  /* Fast slew rate is configured */
                                                  kPORT_FastSlewRate,
                                                  /* Passive filter is disabled */
                                                  kPORT_PassiveFilterDisable,
                                                  /* Open drain is disabled */
                                                  kPORT_OpenDrainDisable,
                                                  /* Low drive strength is configured */
                                                  kPORT_LowDriveStrength,
                                                  /* Pin is configured as QSPI0A_SCLK */
                                                  kPORT_MuxAlt5,
                                                  /* Pin Control Register fields [15:0] are not locked */
                                                  kPORT_UnlockRegister};
    /* PORTE1 (pin 2) is configured as QSPI0A_SCLK */
    PORT_SetPinConfig(PORTE, 1U, &porte1_pin2_config);

    const port_pin_config_t porte10_pin13_config = {/* Internal pull-up/down resistor is disabled */
                                                    kPORT_PullDisable,
                                                    /* Fast slew rate is configured */
                                                    kPORT_FastSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Open drain is disabled */
                                                    kPORT_OpenDrainDisable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as QSPI0B_DATA1 */
                                                    kPORT_MuxAlt5,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    kPORT_UnlockRegister};
    /* PORTE10 (pin 13) is configured as QSPI0B_DATA1 */
    PORT_SetPinConfig(PORTE, 10U, &porte10_pin13_config);

    const port_pin_config_t porte11_pin14_config = {/* Internal pull-up/down resistor is disabled */
                                                    kPORT_PullDisable,
                                                    /* Fast slew rate is configured */
                                                    kPORT_FastSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Open drain is disabled */
                                                    kPORT_OpenDrainDisable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as QSPI0B_SS0_B */
                                                    kPORT_MuxAlt5,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    kPORT_UnlockRegister};
    /* PORTE11 (pin 14) is configured as QSPI0B_SS0_B */
    PORT_SetPinConfig(PORTE, 11U, &porte11_pin14_config);

    const port_pin_config_t porte2_pin3_config = {/* Internal pull-up/down resistor is disabled */
                                                  kPORT_PullDisable,
                                                  /* Fast slew rate is configured */
                                                  kPORT_FastSlewRate,
                                                  /* Passive filter is disabled */
                                                  kPORT_PassiveFilterDisable,
                                                  /* Open drain is disabled */
                                                  kPORT_OpenDrainDisable,
                                                  /* Low drive strength is configured */
                                                  kPORT_LowDriveStrength,
                                                  /* Pin is configured as QSPI0A_DATA0 */
                                                  kPORT_MuxAlt5,
                                                  /* Pin Control Register fields [15:0] are not locked */
                                                  kPORT_UnlockRegister};
    /* PORTE2 (pin 3) is configured as QSPI0A_DATA0 */
    PORT_SetPinConfig(PORTE, 2U, &porte2_pin3_config);

    const port_pin_config_t porte3_pin4_config = {/* Internal pull-up/down resistor is disabled */
                                                  kPORT_PullDisable,
                                                  /* Fast slew rate is configured */
                                                  kPORT_FastSlewRate,
                                                  /* Passive filter is disabled */
                                                  kPORT_PassiveFilterDisable,
                                                  /* Open drain is disabled */
                                                  kPORT_OpenDrainDisable,
                                                  /* Low drive strength is configured */
                                                  kPORT_LowDriveStrength,
                                                  /* Pin is configured as QSPI0A_DATA2 */
                                                  kPORT_MuxAlt5,
                                                  /* Pin Control Register fields [15:0] are not locked */
                                                  kPORT_UnlockRegister};
    /* PORTE3 (pin 4) is configured as QSPI0A_DATA2 */
    PORT_SetPinConfig(PORTE, 3U, &porte3_pin4_config);

    const port_pin_config_t porte4_pin7_config = {/* Internal pull-up/down resistor is disabled */
                                                  kPORT_PullDisable,
                                                  /* Fast slew rate is configured */
                                                  kPORT_FastSlewRate,
                                                  /* Passive filter is disabled */
                                                  kPORT_PassiveFilterDisable,
                                                  /* Open drain is disabled */
                                                  kPORT_OpenDrainDisable,
                                                  /* Low drive strength is configured */
                                                  kPORT_LowDriveStrength,
                                                  /* Pin is configured as QSPI0A_DATA1 */
                                                  kPORT_MuxAlt5,
                                                  /* Pin Control Register fields [15:0] are not locked */
                                                  kPORT_UnlockRegister};
    /* PORTE4 (pin 7) is configured as QSPI0A_DATA1 */
    PORT_SetPinConfig(PORTE, 4U, &porte4_pin7_config);

    const port_pin_config_t porte5_pin8_config = {/* Internal pull-up/down resistor is disabled */
                                                  kPORT_PullDisable,
                                                  /* Fast slew rate is configured */
                                                  kPORT_FastSlewRate,
                                                  /* Passive filter is disabled */
                                                  kPORT_PassiveFilterDisable,
                                                  /* Open drain is disabled */
                                                  kPORT_OpenDrainDisable,
                                                  /* Low drive strength is configured */
                                                  kPORT_LowDriveStrength,
                                                  /* Pin is configured as QSPI0A_SS0_B */
                                                  kPORT_MuxAlt5,
                                                  /* Pin Control Register fields [15:0] are not locked */
                                                  kPORT_UnlockRegister};
    /* PORTE5 (pin 8) is configured as QSPI0A_SS0_B */
    PORT_SetPinConfig(PORTE, 5U, &porte5_pin8_config);

    const port_pin_config_t porte6_pin9_config = {/* Internal pull-up/down resistor is disabled */
                                                  kPORT_PullDisable,
                                                  /* Fast slew rate is configured */
                                                  kPORT_FastSlewRate,
                                                  /* Passive filter is disabled */
                                                  kPORT_PassiveFilterDisable,
                                                  /* Open drain is disabled */
                                                  kPORT_OpenDrainDisable,
                                                  /* Low drive strength is configured */
                                                  kPORT_LowDriveStrength,
                                                  /* Pin is configured as QSPI0B_DATA3 */
                                                  kPORT_MuxAlt5,
                                                  /* Pin Control Register fields [15:0] are not locked */
                                                  kPORT_UnlockRegister};
    /* PORTE6 (pin 9) is configured as QSPI0B_DATA3 */
    PORT_SetPinConfig(PORTE, 6U, &porte6_pin9_config);

    const port_pin_config_t porte7_pin10_config = {/* Internal pull-up/down resistor is disabled */
                                                   kPORT_PullDisable,
                                                   /* Fast slew rate is configured */
                                                   kPORT_FastSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Open drain is disabled */
                                                   kPORT_OpenDrainDisable,
                                                   /* Low drive strength is configured */
                                                   kPORT_LowDriveStrength,
                                                   /* Pin is configured as QSPI0B_SCLK */
                                                   kPORT_MuxAlt5,
                                                   /* Pin Control Register fields [15:0] are not locked */
                                                   kPORT_UnlockRegister};
    /* PORTE7 (pin 10) is configured as QSPI0B_SCLK */
    PORT_SetPinConfig(PORTE, 7U, &porte7_pin10_config);

    const port_pin_config_t porte8_pin11_config = {/* Internal pull-up/down resistor is disabled */
                                                   kPORT_PullDisable,
                                                   /* Fast slew rate is configured */
                                                   kPORT_FastSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Open drain is disabled */
                                                   kPORT_OpenDrainDisable,
                                                   /* Low drive strength is configured */
                                                   kPORT_LowDriveStrength,
                                                   /* Pin is configured as QSPI0B_DATA0 */
                                                   kPORT_MuxAlt5,
                                                   /* Pin Control Register fields [15:0] are not locked */
                                                   kPORT_UnlockRegister};
    /* PORTE8 (pin 11) is configured as QSPI0B_DATA0 */
    PORT_SetPinConfig(PORTE, 8U, &porte8_pin11_config);

    const port_pin_config_t porte9_pin12_config = {/* Internal pull-up/down resistor is disabled */
                                                   kPORT_PullDisable,
                                                   /* Fast slew rate is configured */
                                                   kPORT_FastSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Open drain is disabled */
                                                   kPORT_OpenDrainDisable,
                                                   /* Low drive strength is configured */
                                                   kPORT_LowDriveStrength,
                                                   /* Pin is configured as QSPI0B_DATA2 */
                                                   kPORT_MuxAlt5,
                                                   /* Pin Control Register fields [15:0] are not locked */
                                                   kPORT_UnlockRegister};
    /* PORTE9 (pin 12) is configured as QSPI0B_DATA2 */
    PORT_SetPinConfig(PORTE, 9U, &porte9_pin12_config);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitTSI_TOUCH_PAD:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '64', peripheral: TSI0, signal: 'CH, 11', pin_signal: TSI0_CH11/PTB18/FTM2_CH0/I2S0_TX_BCLK/FB_AD15/SDRAM_A23/FTM2_QD_PHA/TPM2_CH0/FXIO0_D6, slew_rate: fast,
    open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: '65', peripheral: TSI0, signal: 'CH, 12', pin_signal: TSI0_CH12/PTB19/FTM2_CH1/I2S0_TX_FS/FB_OE_b/FTM2_QD_PHB/TPM2_CH1/FXIO0_D7, slew_rate: fast, open_drain: disable,
    pull_select: down, pull_enable: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitTSI_TOUCH_PAD
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitTSI_TOUCH_PAD(void)
{
    /* Port B Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);

    const port_pin_config_t portb18_pin64_config = {/* Internal pull-up/down resistor is disabled */
                                                    kPORT_PullDisable,
                                                    /* Fast slew rate is configured */
                                                    kPORT_FastSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Open drain is disabled */
                                                    kPORT_OpenDrainDisable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as TSI0_CH11 */
                                                    kPORT_PinDisabledOrAnalog,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    kPORT_UnlockRegister};
    /* PORTB18 (pin 64) is configured as TSI0_CH11 */
    PORT_SetPinConfig(PORTB, 18U, &portb18_pin64_config);

    const port_pin_config_t portb19_pin65_config = {/* Internal pull-up/down resistor is disabled */
                                                    kPORT_PullDisable,
                                                    /* Fast slew rate is configured */
                                                    kPORT_FastSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Open drain is disabled */
                                                    kPORT_OpenDrainDisable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as TSI0_CH12 */
                                                    kPORT_PinDisabledOrAnalog,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    kPORT_UnlockRegister};
    /* PORTB19 (pin 65) is configured as TSI0_CH12 */
    PORT_SetPinConfig(PORTB, 19U, &portb19_pin65_config);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
I2C0_InitPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '53', peripheral: I2C0, signal: SCL, pin_signal: ADC0_SE8/TSI0_CH0/PTB0/LLWU_P5/I2C0_SCL/FTM1_CH0/SDRAM_CAS_b/FTM1_QD_PHA/TPM1_CH0/FXIO0_D0, slew_rate: fast,
    open_drain: enable, drive_strength: high, pull_select: up, pull_enable: enable}
  - {pin_num: '54', peripheral: I2C0, signal: SDA, pin_signal: ADC0_SE9/TSI0_CH6/PTB1/I2C0_SDA/FTM1_CH1/SDRAM_RAS_b/FTM1_QD_PHB/TPM1_CH1/FXIO0_D1, slew_rate: fast,
    open_drain: enable, drive_strength: high, pull_select: up, pull_enable: enable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : I2C0_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void I2C0_InitPins(void)
{
    /* Port B Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);

    const port_pin_config_t portb0_pin53_config = {/* Internal pull-up resistor is enabled */
                                                   kPORT_PullUp,
                                                   /* Fast slew rate is configured */
                                                   kPORT_FastSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Open drain is enabled */
                                                   kPORT_OpenDrainEnable,
                                                   /* High drive strength is configured */
                                                   kPORT_HighDriveStrength,
                                                   /* Pin is configured as I2C0_SCL */
                                                   kPORT_MuxAlt2,
                                                   /* Pin Control Register fields [15:0] are not locked */
                                                   kPORT_UnlockRegister};
    /* PORTB0 (pin 53) is configured as I2C0_SCL */
    PORT_SetPinConfig(PORTB, 0U, &portb0_pin53_config);

    const port_pin_config_t portb1_pin54_config = {/* Internal pull-up resistor is enabled */
                                                   kPORT_PullUp,
                                                   /* Fast slew rate is configured */
                                                   kPORT_FastSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Open drain is enabled */
                                                   kPORT_OpenDrainEnable,
                                                   /* High drive strength is configured */
                                                   kPORT_HighDriveStrength,
                                                   /* Pin is configured as I2C0_SDA */
                                                   kPORT_MuxAlt2,
                                                   /* Pin Control Register fields [15:0] are not locked */
                                                   kPORT_UnlockRegister};
    /* PORTB1 (pin 54) is configured as I2C0_SDA */
    PORT_SetPinConfig(PORTB, 1U, &portb1_pin54_config);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
I2C0_DeinitPins:
- options: {coreID: core0, enableClock: 'false'}
- pin_list:
  - {pin_num: '53', peripheral: ADC0, signal: 'SE, 8', pin_signal: ADC0_SE8/TSI0_CH0/PTB0/LLWU_P5/I2C0_SCL/FTM1_CH0/SDRAM_CAS_b/FTM1_QD_PHA/TPM1_CH0/FXIO0_D0}
  - {pin_num: '54', peripheral: ADC0, signal: 'SE, 9', pin_signal: ADC0_SE9/TSI0_CH6/PTB1/I2C0_SDA/FTM1_CH1/SDRAM_RAS_b/FTM1_QD_PHB/TPM1_CH1/FXIO0_D1}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : I2C0_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void I2C0_DeinitPins(void)
{

    /* PORTB0 (pin 53) is configured as ADC0_SE8 */
    PORT_SetPinMux(PORTB, 0U, kPORT_PinDisabledOrAnalog);

    /* PORTB1 (pin 54) is configured as ADC0_SE9 */
    PORT_SetPinMux(PORTB, 1U, kPORT_PinDisabledOrAnalog);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
I2C1_InitPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '83', peripheral: I2C1, signal: SDA, pin_signal: PTC11/LLWU_P11/I2C1_SDA/FTM3_CH7/I2S0_RXD1/FB_RW_b/FXIO0_D19, slew_rate: fast, open_drain: enable,
    pull_select: up, pull_enable: enable}
  - {pin_num: '82', peripheral: I2C1, signal: SCL, pin_signal: PTC10/I2C1_SCL/FTM3_CH6/I2S0_RX_FS/FB_AD5/SDRAM_A13/FXIO0_D18, slew_rate: fast, open_drain: enable,
    pull_select: up, pull_enable: enable}
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
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    const port_pin_config_t portc10_pin82_config = {/* Internal pull-up resistor is enabled */
                                                    kPORT_PullUp,
                                                    /* Fast slew rate is configured */
                                                    kPORT_FastSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Open drain is enabled */
                                                    kPORT_OpenDrainEnable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as I2C1_SCL */
                                                    kPORT_MuxAlt2,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    kPORT_UnlockRegister};
    /* PORTC10 (pin 82) is configured as I2C1_SCL */
    PORT_SetPinConfig(PORTC, 10U, &portc10_pin82_config);

    const port_pin_config_t portc11_pin83_config = {/* Internal pull-up resistor is enabled */
                                                    kPORT_PullUp,
                                                    /* Fast slew rate is configured */
                                                    kPORT_FastSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Open drain is enabled */
                                                    kPORT_OpenDrainEnable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as I2C1_SDA */
                                                    kPORT_MuxAlt2,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    kPORT_UnlockRegister};
    /* PORTC11 (pin 83) is configured as I2C1_SDA */
    PORT_SetPinConfig(PORTC, 11U, &portc11_pin83_config);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
I2C1_DeinitPins:
- options: {coreID: core0, enableClock: 'false'}
- pin_list:
  - {pin_num: '82', peripheral: n/a, signal: disabled, pin_signal: PTC10/I2C1_SCL/FTM3_CH6/I2S0_RX_FS/FB_AD5/SDRAM_A13/FXIO0_D18}
  - {pin_num: '83', peripheral: n/a, signal: disabled, pin_signal: PTC11/LLWU_P11/I2C1_SDA/FTM3_CH7/I2S0_RXD1/FB_RW_b/FXIO0_D19}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : I2C1_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void I2C1_DeinitPins(void)
{

    /* PORTC10 (pin 82) is disabled */
    PORT_SetPinMux(PORTC, 10U, kPORT_PinDisabledOrAnalog);

    /* PORTC11 (pin 83) is disabled */
    PORT_SetPinMux(PORTC, 11U, kPORT_PinDisabledOrAnalog);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
I2C2_InitPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '58', peripheral: I2C2, signal: SCL, pin_signal: PTB10/SPI1_PCS0/LPUART3_RX/I2C2_SCL/FB_AD19/SDRAM_D19/FTM0_FLT1/FXIO0_D4, slew_rate: fast, open_drain: enable,
    pull_select: up, pull_enable: enable}
  - {pin_num: '59', peripheral: I2C2, signal: SDA, pin_signal: PTB11/SPI1_SCK/LPUART3_TX/I2C2_SDA/FB_AD18/SDRAM_D18/FTM0_FLT2/FXIO0_D5, slew_rate: fast, open_drain: enable,
    pull_select: up, pull_enable: enable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : I2C2_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void I2C2_InitPins(void)
{
    /* Port B Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);

    const port_pin_config_t portb10_pin58_config = {/* Internal pull-up resistor is enabled */
                                                    kPORT_PullUp,
                                                    /* Fast slew rate is configured */
                                                    kPORT_FastSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Open drain is enabled */
                                                    kPORT_OpenDrainEnable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as I2C2_SCL */
                                                    kPORT_MuxAlt4,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    kPORT_UnlockRegister};
    /* PORTB10 (pin 58) is configured as I2C2_SCL */
    PORT_SetPinConfig(PORTB, 10U, &portb10_pin58_config);

    const port_pin_config_t portb11_pin59_config = {/* Internal pull-up resistor is enabled */
                                                    kPORT_PullUp,
                                                    /* Fast slew rate is configured */
                                                    kPORT_FastSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Open drain is enabled */
                                                    kPORT_OpenDrainEnable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as I2C2_SDA */
                                                    kPORT_MuxAlt4,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    kPORT_UnlockRegister};
    /* PORTB11 (pin 59) is configured as I2C2_SDA */
    PORT_SetPinConfig(PORTB, 11U, &portb11_pin59_config);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
I2C2_DeinitPins:
- options: {coreID: core0, enableClock: 'false'}
- pin_list:
  - {pin_num: '58', peripheral: n/a, signal: disabled, pin_signal: PTB10/SPI1_PCS0/LPUART3_RX/I2C2_SCL/FB_AD19/SDRAM_D19/FTM0_FLT1/FXIO0_D4}
  - {pin_num: '59', peripheral: n/a, signal: disabled, pin_signal: PTB11/SPI1_SCK/LPUART3_TX/I2C2_SDA/FB_AD18/SDRAM_D18/FTM0_FLT2/FXIO0_D5}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : I2C2_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void I2C2_DeinitPins(void)
{

    /* PORTB10 (pin 58) is disabled */
    PORT_SetPinMux(PORTB, 10U, kPORT_PinDisabledOrAnalog);

    /* PORTB11 (pin 59) is disabled */
    PORT_SetPinMux(PORTB, 11U, kPORT_PinDisabledOrAnalog);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
I2C3_InitPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '38', peripheral: I2C3, signal: SCL, pin_signal: TSI0_CH3/PTA2/LPUART0_TX/FTM0_CH7/I2C3_SCL/FXIO0_D12/EMVSIM0_PD/JTAG_TDO/TRACE_SWO, slew_rate: fast,
    open_drain: enable, pull_select: up, pull_enable: enable}
  - {pin_num: '37', peripheral: I2C3, signal: SDA, pin_signal: TSI0_CH2/PTA1/LPUART0_RX/FTM0_CH6/I2C3_SDA/FXIO0_D11/EMVSIM0_IO/JTAG_TDI, slew_rate: fast, open_drain: enable,
    pull_select: up, pull_enable: enable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : I2C3_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void I2C3_InitPins(void)
{
    /* Port A Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);

    const port_pin_config_t porta1_pin37_config = {/* Internal pull-up resistor is enabled */
                                                   kPORT_PullUp,
                                                   /* Fast slew rate is configured */
                                                   kPORT_FastSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Open drain is enabled */
                                                   kPORT_OpenDrainEnable,
                                                   /* Low drive strength is configured */
                                                   kPORT_LowDriveStrength,
                                                   /* Pin is configured as I2C3_SDA */
                                                   kPORT_MuxAlt4,
                                                   /* Pin Control Register fields [15:0] are not locked */
                                                   kPORT_UnlockRegister};
    /* PORTA1 (pin 37) is configured as I2C3_SDA */
    PORT_SetPinConfig(PORTA, 1U, &porta1_pin37_config);

    const port_pin_config_t porta2_pin38_config = {/* Internal pull-up resistor is enabled */
                                                   kPORT_PullUp,
                                                   /* Fast slew rate is configured */
                                                   kPORT_FastSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Open drain is enabled */
                                                   kPORT_OpenDrainEnable,
                                                   /* Low drive strength is configured */
                                                   kPORT_LowDriveStrength,
                                                   /* Pin is configured as I2C3_SCL */
                                                   kPORT_MuxAlt4,
                                                   /* Pin Control Register fields [15:0] are not locked */
                                                   kPORT_UnlockRegister};
    /* PORTA2 (pin 38) is configured as I2C3_SCL */
    PORT_SetPinConfig(PORTA, 2U, &porta2_pin38_config);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
I2C3_DeinitPins:
- options: {coreID: core0, enableClock: 'false'}
- pin_list:
  - {pin_num: '13', peripheral: n/a, signal: disabled, pin_signal: PTE10/LLWU_P18/I2C3_SDA/SPI2_SIN/I2S0_TXD0/QSPI0B_DATA1/FTM3_CH5/SDHC0_D7}
  - {pin_num: '14', peripheral: n/a, signal: disabled, pin_signal: PTE11/I2C3_SCL/SPI2_PCS0/I2S0_TX_FS/QSPI0B_SS0_B/FTM3_CH6/QSPI0A_DQS}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : I2C3_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void I2C3_DeinitPins(void)
{

    /* PORTE10 (pin 13) is disabled */
    PORT_SetPinMux(PORTE, 10U, kPORT_PinDisabledOrAnalog);

    /* PORTE11 (pin 14) is disabled */
    PORT_SetPinMux(PORTE, 11U, kPORT_PinDisabledOrAnalog);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
