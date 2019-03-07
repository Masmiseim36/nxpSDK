/*
 * Copyright (c) 2015-2016, Freescale Semiconductor, Inc.
 * Copyright 2017 NXP
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
 *
 */

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v3.0
processor: MCIMX6G3xxx05
package_id: MCIMX6G3CVM05
mcu_data: i_mx_1_0
processor_version: 1.0.0
board: MCIMX6UL-EVK-REV-B
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "pin_mux.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: ca7}
- pin_list:
  - {pin_num: K16, peripheral: UART1, signal: uart_rx, pin_signal: UART1_RX_DATA, PUE: KEEP, PKE: ENABLED, DSE: OHM_120}
  - {pin_num: K14, peripheral: UART1, signal: uart_tx, pin_signal: UART1_TX_DATA, PUE: KEEP, PKE: ENABLED, DSE: OHM_120}
  - {pin_num: N17, peripheral: GPIO1, signal: 'gpio_io, 8', pin_signal: GPIO1_IO08, SION: ENABLED}
  - {pin_num: F17, peripheral: I2C2, signal: i2c_scl, pin_signal: UART5_TX_DATA, SION: ENABLED, PUS: OHM_47K_PU, PUE: PULL, PKE: ENABLED, ODE: ENABLED, DSE: OHM_120}
  - {pin_num: G13, peripheral: I2C2, signal: i2c_sda, pin_signal: UART5_RX_DATA, SION: ENABLED, PUS: OHM_47K_PU, PUE: PULL, PKE: ENABLED, ODE: ENABLED, DSE: OHM_120}
  - {pin_num: K13, peripheral: ADC1, signal: adc_in0, pin_signal: GPIO1_IO00, PKE: DISABLED}
  - {pin_num: L15, peripheral: ADC1, signal: adc_in1, pin_signal: GPIO1_IO01, PKE: DISABLED}
  - {pin_num: L14, peripheral: ADC1, signal: adc_in2, pin_signal: GPIO1_IO02, PKE: DISABLED}
  - {pin_num: L17, peripheral: ADC1, signal: adc_in3, pin_signal: GPIO1_IO03, PKE: DISABLED}
  - {pin_num: M16, peripheral: ADC1, signal: adc_in4, pin_signal: GPIO1_IO04, PKE: DISABLED}
  - {pin_num: T10, peripheral: GPIO5, signal: 'gpio_io, 10', pin_signal: BOOT_MODE0, HYS: ENABLED, PUE: PULL, PKE: ENABLED}
  - {pin_num: U10, peripheral: GPIO5, signal: 'gpio_io, 11', pin_signal: BOOT_MODE1, HYS: ENABLED, PUE: PULL, PKE: ENABLED}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitPins(void) {                                /*!< Function assigned for the core: Cortex-A7[ca7] */
    IOMUXC_SetPinMux(IOMUXC_BOOT_MODE0_GPIO5_IO10, 0U);
    IOMUXC_SetPinConfig(IOMUXC_BOOT_MODE0_GPIO5_IO10, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(4U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
    IOMUXC_SetPinMux(IOMUXC_BOOT_MODE1_GPIO5_IO11, 0U);
    IOMUXC_SetPinConfig(IOMUXC_BOOT_MODE1_GPIO5_IO11, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(4U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO00_GPIO1_IO00, 0U);
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO00_GPIO1_IO00, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(6U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U));
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO01_GPIO1_IO01, 0U);
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO01_GPIO1_IO01, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(6U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U));
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO02_GPIO1_IO02, 0U);
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO02_GPIO1_IO02, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(6U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U));
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO03_GPIO1_IO03, 0U);
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO03_GPIO1_IO03, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(6U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U));
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO04_GPIO1_IO04, 0U);
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO04_GPIO1_IO04, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(6U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U));
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO08_GPIO1_IO08, 1U);
    IOMUXC_SetPinMux(IOMUXC_UART1_RX_DATA_UART1_RX, 0U);
    IOMUXC_SetPinConfig(IOMUXC_UART1_RX_DATA_UART1_RX, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK);
    IOMUXC_SetPinMux(IOMUXC_UART1_TX_DATA_UART1_TX, 0U);
    IOMUXC_SetPinConfig(IOMUXC_UART1_TX_DATA_UART1_TX, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK);
    IOMUXC_SetPinMux(IOMUXC_UART5_RX_DATA_I2C2_SDA, 1U);
    IOMUXC_SetPinConfig(IOMUXC_UART5_RX_DATA_I2C2_SDA, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_ODE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUS(1U));
    IOMUXC_SetPinMux(IOMUXC_UART5_TX_DATA_I2C2_SCL, 1U);
    IOMUXC_SetPinConfig(IOMUXC_UART5_TX_DATA_I2C2_SCL, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_ODE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUS(1U));
}


/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_LCDIF_ConfigurePins:
- options: {coreID: ca7}
- pin_list:
  - {pin_num: A8, peripheral: LCDIF, signal: lcdif_clk, pin_signal: LCD_CLK, SION: DISABLED, HYS: DISABLED, PUS: no_init, PUE: KEEP, PKE: DISABLED, DSE: OHM_34, SRE: FAST}
  - {pin_num: B9, peripheral: LCDIF, signal: 'lcdif_data, 0', pin_signal: LCD_DATA00, SION: DISABLED, HYS: DISABLED, PUS: no_init, PUE: KEEP, PKE: DISABLED, DSE: OHM_34,
    SRE: FAST}
  - {pin_num: A9, peripheral: LCDIF, signal: 'lcdif_data, 1', pin_signal: LCD_DATA01, SION: DISABLED, HYS: DISABLED, PUS: no_init, PUE: KEEP, PKE: DISABLED, DSE: OHM_34,
    SRE: FAST}
  - {pin_num: E10, peripheral: LCDIF, signal: 'lcdif_data, 2', pin_signal: LCD_DATA02, SION: DISABLED, HYS: DISABLED, PUS: no_init, PUE: KEEP, PKE: DISABLED, DSE: OHM_34,
    SRE: FAST}
  - {pin_num: D10, peripheral: LCDIF, signal: 'lcdif_data, 3', pin_signal: LCD_DATA03, SION: DISABLED, HYS: DISABLED, PUS: no_init, PUE: KEEP, PKE: DISABLED, DSE: OHM_34,
    SRE: FAST}
  - {pin_num: C10, peripheral: LCDIF, signal: 'lcdif_data, 4', pin_signal: LCD_DATA04, SION: DISABLED, HYS: DISABLED, PUS: no_init, PUE: KEEP, PKE: DISABLED, DSE: OHM_34,
    SRE: FAST}
  - {pin_num: B10, peripheral: LCDIF, signal: 'lcdif_data, 5', pin_signal: LCD_DATA05, SION: DISABLED, HYS: DISABLED, PUS: no_init, PUE: KEEP, PKE: DISABLED, DSE: OHM_34,
    SRE: FAST}
  - {pin_num: A10, peripheral: LCDIF, signal: 'lcdif_data, 6', pin_signal: LCD_DATA06, SION: DISABLED, HYS: DISABLED, PUS: no_init, PUE: KEEP, PKE: DISABLED, DSE: OHM_34,
    SRE: FAST}
  - {pin_num: D11, peripheral: LCDIF, signal: 'lcdif_data, 7', pin_signal: LCD_DATA07, SION: DISABLED, HYS: DISABLED, PUS: no_init, PUE: KEEP, PKE: DISABLED, DSE: OHM_34,
    SRE: FAST}
  - {pin_num: B11, peripheral: LCDIF, signal: 'lcdif_data, 8', pin_signal: LCD_DATA08, SION: DISABLED, HYS: DISABLED, PUS: no_init, PUE: KEEP, PKE: DISABLED, DSE: OHM_34,
    SRE: FAST}
  - {pin_num: A11, peripheral: LCDIF, signal: 'lcdif_data, 9', pin_signal: LCD_DATA09, SION: DISABLED, HYS: DISABLED, PUS: no_init, PUE: KEEP, PKE: DISABLED, DSE: OHM_34,
    SRE: FAST}
  - {pin_num: E12, peripheral: LCDIF, signal: 'lcdif_data, 10', pin_signal: LCD_DATA10, SION: DISABLED, HYS: DISABLED, PUS: no_init, PUE: KEEP, PKE: DISABLED, DSE: OHM_34,
    SRE: FAST}
  - {pin_num: D12, peripheral: LCDIF, signal: 'lcdif_data, 11', pin_signal: LCD_DATA11, SION: DISABLED, HYS: DISABLED, PUS: no_init, PUE: KEEP, PKE: DISABLED, DSE: OHM_34,
    SRE: FAST}
  - {pin_num: C12, peripheral: LCDIF, signal: 'lcdif_data, 12', pin_signal: LCD_DATA12, SION: DISABLED, HYS: DISABLED, PUS: no_init, PUE: KEEP, PKE: DISABLED, DSE: OHM_34,
    SRE: FAST}
  - {pin_num: B12, peripheral: LCDIF, signal: 'lcdif_data, 13', pin_signal: LCD_DATA13, SION: DISABLED, HYS: DISABLED, PUS: no_init, PUE: KEEP, PKE: DISABLED, DSE: OHM_34,
    SRE: FAST}
  - {pin_num: A12, peripheral: LCDIF, signal: 'lcdif_data, 14', pin_signal: LCD_DATA14, SION: DISABLED, HYS: DISABLED, PUS: no_init, PUE: KEEP, PKE: DISABLED, DSE: OHM_34,
    SRE: FAST}
  - {pin_num: D13, peripheral: LCDIF, signal: 'lcdif_data, 15', pin_signal: LCD_DATA15, SION: DISABLED, HYS: DISABLED, PUS: no_init, PUE: KEEP, PKE: DISABLED, DSE: OHM_34,
    SRE: FAST}
  - {pin_num: C13, peripheral: LCDIF, signal: 'lcdif_data, 16', pin_signal: LCD_DATA16, SION: DISABLED, HYS: DISABLED, PUS: no_init, PUE: KEEP, PKE: DISABLED, DSE: OHM_34,
    SRE: FAST}
  - {pin_num: B13, peripheral: LCDIF, signal: 'lcdif_data, 17', pin_signal: LCD_DATA17, SION: DISABLED, HYS: DISABLED, PUS: no_init, PUE: KEEP, PKE: DISABLED, DSE: OHM_34,
    SRE: FAST}
  - {pin_num: A13, peripheral: LCDIF, signal: 'lcdif_data, 18', pin_signal: LCD_DATA18, SION: DISABLED, HYS: DISABLED, PUS: no_init, PUE: KEEP, PKE: DISABLED, DSE: OHM_34,
    SRE: FAST}
  - {pin_num: D14, peripheral: LCDIF, signal: 'lcdif_data, 19', pin_signal: LCD_DATA19, SION: DISABLED, HYS: DISABLED, PUS: no_init, PUE: KEEP, PKE: DISABLED, DSE: OHM_34,
    SRE: FAST}
  - {pin_num: C14, peripheral: LCDIF, signal: 'lcdif_data, 20', pin_signal: LCD_DATA20, SION: DISABLED, HYS: DISABLED, PUS: no_init, PUE: KEEP, PKE: DISABLED, DSE: OHM_34,
    SRE: FAST}
  - {pin_num: B14, peripheral: LCDIF, signal: 'lcdif_data, 21', pin_signal: LCD_DATA21, SION: DISABLED, HYS: DISABLED, PUS: no_init, PUE: KEEP, PKE: DISABLED, DSE: OHM_34,
    SRE: FAST}
  - {pin_num: A14, peripheral: LCDIF, signal: 'lcdif_data, 22', pin_signal: LCD_DATA22, SION: DISABLED, HYS: DISABLED, PUS: no_init, PUE: KEEP, PKE: DISABLED, DSE: OHM_34,
    SRE: FAST}
  - {pin_num: B16, peripheral: LCDIF, signal: 'lcdif_data, 23', pin_signal: LCD_DATA23, SION: DISABLED, HYS: DISABLED, PUS: no_init, PUE: KEEP, PKE: DISABLED, DSE: OHM_34,
    SRE: FAST}
  - {pin_num: B8, peripheral: LCDIF, signal: lcdif_enable, pin_signal: LCD_ENABLE, SION: DISABLED, HYS: DISABLED, PUS: no_init, PUE: KEEP, PKE: DISABLED, DSE: OHM_34,
    SRE: FAST}
  - {pin_num: D9, peripheral: LCDIF, signal: lcdif_hsync, pin_signal: LCD_HSYNC, SION: DISABLED, HYS: DISABLED, PUS: no_init, PUE: KEEP, PKE: DISABLED, DSE: OHM_34,
    SRE: FAST}
  - {pin_num: C9, peripheral: LCDIF, signal: lcdif_vsync, pin_signal: LCD_VSYNC, SION: DISABLED, HYS: DISABLED, PUS: no_init, PUE: KEEP, PKE: DISABLED, DSE: OHM_34,
    SRE: FAST}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_LCDIF_ConfigurePins
 * Description   : Initialize eLCDIF pins.
 *
 *END**************************************************************************/
void BOARD_LCDIF_ConfigurePins(void) {                     /*!< Function assigned for the core: Cortex-A7[ca7] */
    IOMUXC_SetPinMux(IOMUXC_LCD_CLK_LCDIF_CLK, 0U);
    IOMUXC_SetPinConfig(IOMUXC_LCD_CLK_LCDIF_CLK, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(7U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U));
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA00_LCDIF_DATA00, 0U);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA00_LCDIF_DATA00, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(7U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U));
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA01_LCDIF_DATA01, 0U);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA01_LCDIF_DATA01, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(7U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U));
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA02_LCDIF_DATA02, 0U);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA02_LCDIF_DATA02, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(7U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U));
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA03_LCDIF_DATA03, 0U);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA03_LCDIF_DATA03, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(7U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U));
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA04_LCDIF_DATA04, 0U);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA04_LCDIF_DATA04, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(7U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U));
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA05_LCDIF_DATA05, 0U);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA05_LCDIF_DATA05, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(7U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U));
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA06_LCDIF_DATA06, 0U);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA06_LCDIF_DATA06, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(7U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U));
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA07_LCDIF_DATA07, 0U);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA07_LCDIF_DATA07, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(7U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U));
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA08_LCDIF_DATA08, 0U);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA08_LCDIF_DATA08, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(7U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U));
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA09_LCDIF_DATA09, 0U);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA09_LCDIF_DATA09, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(7U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U));
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA10_LCDIF_DATA10, 0U);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA10_LCDIF_DATA10, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(7U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U));
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA11_LCDIF_DATA11, 0U);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA11_LCDIF_DATA11, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(7U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U));
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA12_LCDIF_DATA12, 0U);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA12_LCDIF_DATA12, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(7U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U));
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA13_LCDIF_DATA13, 0U);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA13_LCDIF_DATA13, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(7U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U));
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA14_LCDIF_DATA14, 0U);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA14_LCDIF_DATA14, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(7U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U));
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA15_LCDIF_DATA15, 0U);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA15_LCDIF_DATA15, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(7U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U));
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA16_LCDIF_DATA16, 0U);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA16_LCDIF_DATA16, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(7U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U));
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA17_LCDIF_DATA17, 0U);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA17_LCDIF_DATA17, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(7U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U));
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA18_LCDIF_DATA18, 0U);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA18_LCDIF_DATA18, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(7U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U));
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA19_LCDIF_DATA19, 0U);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA19_LCDIF_DATA19, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(7U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U));
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA20_LCDIF_DATA20, 0U);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA20_LCDIF_DATA20, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(7U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U));
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA21_LCDIF_DATA21, 0U);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA21_LCDIF_DATA21, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(7U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U));
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA22_LCDIF_DATA22, 0U);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA22_LCDIF_DATA22, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(7U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U));
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA23_LCDIF_DATA23, 0U);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA23_LCDIF_DATA23, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(7U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U));
    IOMUXC_SetPinMux(IOMUXC_LCD_ENABLE_LCDIF_ENABLE, 0U);
    IOMUXC_SetPinConfig(IOMUXC_LCD_ENABLE_LCDIF_ENABLE, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(7U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U));
    IOMUXC_SetPinMux(IOMUXC_LCD_HSYNC_LCDIF_HSYNC, 0U);
    IOMUXC_SetPinConfig(IOMUXC_LCD_HSYNC_LCDIF_HSYNC, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(7U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U));
    IOMUXC_SetPinMux(IOMUXC_LCD_VSYNC_LCDIF_VSYNC, 0U);
    IOMUXC_SetPinConfig(IOMUXC_LCD_VSYNC_LCDIF_VSYNC, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(7U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U));
}


/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_CSI_ConfigurePins:
- options: {coreID: ca7}
- pin_list:
  - {pin_num: F3, peripheral: CSI, signal: csi_hsync, pin_signal: CSI_HSYNC, PKE: ENABLED}
  - {pin_num: F5, peripheral: CSI, signal: csi_mclk, pin_signal: CSI_MCLK, PKE: ENABLED}
  - {pin_num: E5, peripheral: CSI, signal: csi_pixclk, pin_signal: CSI_PIXCLK, PKE: ENABLED}
  - {pin_num: F2, peripheral: CSI, signal: csi_vsync, pin_signal: CSI_VSYNC, PKE: ENABLED}
  - {pin_num: E4, peripheral: CSI, signal: 'csi_data, 2', pin_signal: CSI_DATA00, PKE: ENABLED}
  - {pin_num: E3, peripheral: CSI, signal: 'csi_data, 3', pin_signal: CSI_DATA01, PKE: ENABLED}
  - {pin_num: E2, peripheral: CSI, signal: 'csi_data, 4', pin_signal: CSI_DATA02, PKE: ENABLED}
  - {pin_num: E1, peripheral: CSI, signal: 'csi_data, 5', pin_signal: CSI_DATA03, PKE: ENABLED}
  - {pin_num: D4, peripheral: CSI, signal: 'csi_data, 6', pin_signal: CSI_DATA04, PKE: ENABLED}
  - {pin_num: D3, peripheral: CSI, signal: 'csi_data, 7', pin_signal: CSI_DATA05, PKE: ENABLED}
  - {pin_num: D2, peripheral: CSI, signal: 'csi_data, 8', pin_signal: CSI_DATA06, PKE: ENABLED}
  - {pin_num: D1, peripheral: CSI, signal: 'csi_data, 9', pin_signal: CSI_DATA07, PKE: ENABLED}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_CSI_ConfigurePins
 * Description   : Initialize CSI pins.
 *
 *END**************************************************************************/
void BOARD_CSI_ConfigurePins(void) {                       /*!< Function assigned for the core: Cortex-A7[ca7] */
    IOMUXC_SetPinMux(IOMUXC_CSI_DATA00_CSI_DATA02, 0U);
    IOMUXC_SetPinConfig(IOMUXC_CSI_DATA00_CSI_DATA02, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(6U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK);
    IOMUXC_SetPinMux(IOMUXC_CSI_DATA01_CSI_DATA03, 0U);
    IOMUXC_SetPinConfig(IOMUXC_CSI_DATA01_CSI_DATA03, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(6U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK);
    IOMUXC_SetPinMux(IOMUXC_CSI_DATA02_CSI_DATA04, 0U);
    IOMUXC_SetPinConfig(IOMUXC_CSI_DATA02_CSI_DATA04, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(6U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK);
    IOMUXC_SetPinMux(IOMUXC_CSI_DATA03_CSI_DATA05, 0U);
    IOMUXC_SetPinConfig(IOMUXC_CSI_DATA03_CSI_DATA05, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(6U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK);
    IOMUXC_SetPinMux(IOMUXC_CSI_DATA04_CSI_DATA06, 0U);
    IOMUXC_SetPinConfig(IOMUXC_CSI_DATA04_CSI_DATA06, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(6U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK);
    IOMUXC_SetPinMux(IOMUXC_CSI_DATA05_CSI_DATA07, 0U);
    IOMUXC_SetPinConfig(IOMUXC_CSI_DATA05_CSI_DATA07, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(6U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK);
    IOMUXC_SetPinMux(IOMUXC_CSI_DATA06_CSI_DATA08, 0U);
    IOMUXC_SetPinConfig(IOMUXC_CSI_DATA06_CSI_DATA08, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(6U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK);
    IOMUXC_SetPinMux(IOMUXC_CSI_DATA07_CSI_DATA09, 0U);
    IOMUXC_SetPinConfig(IOMUXC_CSI_DATA07_CSI_DATA09, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(6U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK);
    IOMUXC_SetPinMux(IOMUXC_CSI_HSYNC_CSI_HSYNC, 0U);
    IOMUXC_SetPinConfig(IOMUXC_CSI_HSYNC_CSI_HSYNC, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(6U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK);
    IOMUXC_SetPinMux(IOMUXC_CSI_MCLK_CSI_MCLK, 0U);
    IOMUXC_SetPinConfig(IOMUXC_CSI_MCLK_CSI_MCLK, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(6U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK);
    IOMUXC_SetPinMux(IOMUXC_CSI_PIXCLK_CSI_PIXCLK, 0U);
    IOMUXC_SetPinConfig(IOMUXC_CSI_PIXCLK_CSI_PIXCLK, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(6U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK);
    IOMUXC_SetPinMux(IOMUXC_CSI_VSYNC_CSI_VSYNC, 0U);
    IOMUXC_SetPinConfig(IOMUXC_CSI_VSYNC_CSI_VSYNC, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(6U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK);
}


/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_USDHC_ConfigurePins:
- options: {coreID: ca7}
- pin_list:
  - {pin_num: C1, peripheral: uSDHC1, signal: usdhc_clk, pin_signal: SD1_CLK, HYS: ENABLED, PUS: OHM_47K_PU, PUE: PULL, PKE: ENABLED, SPEED: MHZ_50, DSE: OHM_240,
    SRE: FAST}
  - {pin_num: C2, peripheral: uSDHC1, signal: usdhc_cmd, pin_signal: SD1_CMD, HYS: ENABLED, PUS: OHM_47K_PU, PUE: PULL, PKE: ENABLED, DSE: OHM_240, SRE: FAST}
  - {pin_num: B3, peripheral: uSDHC1, signal: 'usdhc_data, 0', pin_signal: SD1_DATA0, HYS: ENABLED, PUS: OHM_47K_PU, PUE: PULL, PKE: ENABLED, DSE: OHM_240, SRE: FAST}
  - {pin_num: B2, peripheral: uSDHC1, signal: 'usdhc_data, 1', pin_signal: SD1_DATA1, HYS: ENABLED, PUS: OHM_47K_PU, PUE: PULL, PKE: ENABLED, DSE: OHM_240, SRE: FAST}
  - {pin_num: B1, peripheral: uSDHC1, signal: 'usdhc_data, 2', pin_signal: SD1_DATA2, HYS: ENABLED, PUS: OHM_47K_PU, PUE: PULL, PKE: ENABLED, DSE: OHM_240, SRE: FAST}
  - {pin_num: A2, peripheral: uSDHC1, signal: 'usdhc_data, 3', pin_signal: SD1_DATA3, HYS: ENABLED, PUS: OHM_47K_PU, PUE: PULL, PKE: ENABLED, DSE: OHM_240, SRE: FAST}
  - {pin_num: J14, peripheral: GPIO1, signal: 'gpio_io, 19', pin_signal: UART1_RTS_B, HYS: ENABLED, PUS: OHM_100K_PU, PUE: KEEP, PKE: DISABLED, SPEED: MHZ_100, DSE: HIZ,
    SRE: SLOW}
  - {pin_num: M15, peripheral: GPIO1, signal: 'gpio_io, 9', pin_signal: GPIO1_IO09, HYS: ENABLED, PUS: OHM_100K_PD, PUE: KEEP, PKE: DISABLED, SPEED: MHZ_50, DSE: OHM_240,
    SRE: FAST}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_USDHC_ConfigurePins
 * Description   : Initialize CSI pins.
 *
 *END**************************************************************************/
void BOARD_USDHC_ConfigurePins(void) {                     /*!< Function assigned for the core: Cortex-A7[ca7] */
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO09_GPIO1_IO09, 0U);
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO09_GPIO1_IO09, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(1U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(1U) |
                        IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
    IOMUXC_SetPinMux(IOMUXC_SD1_CLK_USDHC1_CLK, 0U);
    IOMUXC_SetPinConfig(IOMUXC_SD1_CLK_USDHC1_CLK, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(1U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(1U) |
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUS(1U) |
                        IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
    IOMUXC_SetPinMux(IOMUXC_SD1_CMD_USDHC1_CMD, 0U);
    IOMUXC_SetPinConfig(IOMUXC_SD1_CMD_USDHC1_CMD, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(1U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUS(1U) |
                        IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
    IOMUXC_SetPinMux(IOMUXC_SD1_DATA0_USDHC1_DATA0, 0U);
    IOMUXC_SetPinConfig(IOMUXC_SD1_DATA0_USDHC1_DATA0, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(1U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUS(1U) |
                        IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
    IOMUXC_SetPinMux(IOMUXC_SD1_DATA1_USDHC1_DATA1, 0U);
    IOMUXC_SetPinConfig(IOMUXC_SD1_DATA1_USDHC1_DATA1, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(1U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUS(1U) |
                        IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
    IOMUXC_SetPinMux(IOMUXC_SD1_DATA2_USDHC1_DATA2, 0U);
    IOMUXC_SetPinConfig(IOMUXC_SD1_DATA2_USDHC1_DATA2, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(1U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUS(1U) |
                        IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
    IOMUXC_SetPinMux(IOMUXC_SD1_DATA3_USDHC1_DATA3, 0U);
    IOMUXC_SetPinConfig(IOMUXC_SD1_DATA3_USDHC1_DATA3, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(1U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUS(1U) |
                        IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
    IOMUXC_SetPinMux(IOMUXC_UART1_RTS_B_GPIO1_IO19, 0U);
    IOMUXC_SetPinConfig(IOMUXC_UART1_RTS_B_GPIO1_IO19, 
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_PUS(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
