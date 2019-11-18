/*
 * Copyright 2018 NXP
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v3.0
processor: MIMX8QM5xxxDD
package_id: MIMX8QM5AVUDD
mcu_data: i_mx_1_0
processor_version: 0.0.0
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

#include "pin_mux.h"
#include "fsl_common.h"
#include "main/imx8qm_pads.h"
#include "svc/pad/pad_api.h"

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: cm4_1}
- pin_list:
  - {pin_num: AU45, peripheral: DMA__UART2, signal: uart_rx, pin_signal: UART0_RTS_B, sw_config: sw_config_0}
  - {pin_num: AW49, peripheral: DMA__UART2, signal: uart_tx, pin_signal: UART0_CTS_B, sw_config: sw_config_0}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitPins(void)                                  /*!< Function assigned for the core: Cortex-M4F(core 1)[m4_1] */
{
  sc_ipc_t ipc;
  sc_err_t err = SC_ERR_NONE;
  ipc = SystemGetScfwIpcHandle();

  err = sc_pad_set_all(ipc, SC_P_UART0_CTS_B, 2U, SC_PAD_CONFIG_NORMAL, SC_PAD_ISO_OFF, 0x0 ,SC_PAD_WAKEUP_OFF);/* IOMUXD_REG_UART0_CTS_B register modification value */
  if (SC_ERR_NONE != err)
  {
      assert(false);
  }
  err = sc_pad_set_all(ipc, SC_P_UART0_RTS_B, 2U, SC_PAD_CONFIG_NORMAL, SC_PAD_ISO_OFF, 0x0 ,SC_PAD_WAKEUP_OFF);/* IOMUXD_REG_UART0_RTS_B register modification value */
  if (SC_ERR_NONE != err)
  {
      assert(false);
  }
}


/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
LPI2C2_InitPins:
- options: {coreID: cm4_1}
- pin_list:
  - {pin_num: BN9, peripheral: DMA__I2C0, signal: i2c_scl, pin_signal: HDMI_TX0_TS_SCL, sw_config: sw_config_0}
  - {pin_num: BN7, peripheral: DMA__I2C0, signal: i2c_sda, pin_signal: HDMI_TX0_TS_SDA, sw_config: sw_config_0}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C2_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void LPI2C2_InitPins(void)                                 /*!< Function assigned for the core: Cortex-M4F(core 1)[m4_1] */
{
  sc_ipc_t ipc;
  sc_err_t err = SC_ERR_NONE;
  ipc = SystemGetScfwIpcHandle();

  err = sc_pad_set_all(ipc, SC_P_HDMI_TX0_TS_SCL, 1U, SC_PAD_CONFIG_NORMAL, SC_PAD_ISO_OFF, 0x0 ,SC_PAD_WAKEUP_OFF);/* IOMUXD_REG_HDMI_TX0_TS_SCL register modification value */
  if (SC_ERR_NONE != err)
  {
      assert(false);
  }
  err = sc_pad_set_all(ipc, SC_P_HDMI_TX0_TS_SDA, 1U, SC_PAD_CONFIG_NORMAL, SC_PAD_ISO_OFF, 0x0 ,SC_PAD_WAKEUP_OFF);/* IOMUXD_REG_HDMI_TX0_TS_SDA register modification value */
  if (SC_ERR_NONE != err)
  {
      assert(false);
  }
}


/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
LPI2C2_DeinitPins:
- options: {coreID: cm4_1}
- pin_list:
  - {pin_num: BN9, peripheral: HDMI_TX0__I2C0, signal: i2c_scl, pin_signal: HDMI_TX0_TS_SCL, sw_config: sw_config_0}
  - {pin_num: BN7, peripheral: HDMI_TX0__I2C0, signal: i2c_sda, pin_signal: HDMI_TX0_TS_SDA, sw_config: sw_config_0}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C2_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void LPI2C2_DeinitPins(void)                               /*!< Function assigned for the core: Cortex-M4F(core 1)[m4_1] */
{
  sc_ipc_t ipc;
  sc_err_t err = SC_ERR_NONE;
  ipc = SystemGetScfwIpcHandle();

  err = sc_pad_set_all(ipc, SC_P_HDMI_TX0_TS_SCL, 0U, SC_PAD_CONFIG_NORMAL, SC_PAD_ISO_OFF, 0x0 ,SC_PAD_WAKEUP_OFF);/* IOMUXD_REG_HDMI_TX0_TS_SCL register modification value */
  if (SC_ERR_NONE != err)
  {
      assert(false);
  }
  err = sc_pad_set_all(ipc, SC_P_HDMI_TX0_TS_SDA, 0U, SC_PAD_CONFIG_NORMAL, SC_PAD_ISO_OFF, 0x0 ,SC_PAD_WAKEUP_OFF);/* IOMUXD_REG_HDMI_TX0_TS_SDA register modification value */
  if (SC_ERR_NONE != err)
  {
      assert(false);
  }
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
