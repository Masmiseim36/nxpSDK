/*
 * Copyright 2023-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#if defined(APP_LOWPOWER_ENABLED) && (APP_LOWPOWER_ENABLED > 0)
/*******************************************************************************
 * Include
 ******************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <porting.h>

#include "board.h"
#include "fsl_adapter_gpio.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"
#include "controller_low_power.h"
#include "controller_low_power_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Controller low power command configuration. */
#if defined(WIFI_IW416_BOARD_MURATA_1XK_M2) || defined(WIFI_IW612_BOARD_MURATA_2EL_M2)
/*
 * Vendor specific HCI command.
 */
#define CONTROLLER_LOW_POWER_CONFIG_CMD_CNT     3U
#define BT_HCI_CMD_SET_BT_SLEEP_MODE            BT_OP(BT_OGF_VS, 0x0023U)
#define BT_HCI_CMD_WAKE_UP_METHOD               BT_OP(BT_OGF_VS, 0x0053U)
#define BT_HCI_CMD_BLE_POWER_SAVE_MODE_CONFIG   BT_OP(BT_OGF_VS, 0x008BU)

/*
 * HCI command configuration.
 */
/* BT_HCI_CMD_SET_BT_SLEEP_MODE */
static uint8_t set_bt_sleep_mode_cmd[] = {0x02U, 0x00U, 0x00U};

/* BT_HCI_CMD_WAKE_UP_METHOD */
static uint8_t wake_up_method_cmd[] = {0x03U, BT_C2H_GPIO,
                                       0x02U, BT_H2C_GPIO,
                                       H2C_WAKEUP_VALUE,
                                       C2H_WAKEUP_VALUE};

/* BT_HCI_CMD_BLE_POWER_SAVE_MODE_CONFIG */
static uint8_t ble_power_save_mode_config_cmd[] = {0x01U};

/*
 * HCI command struct.
 */
static lp_config_cmd_t lp_mode_cfg_cmd[CONTROLLER_LOW_POWER_CONFIG_CMD_CNT] = {
    {
        .opcode  = BT_HCI_CMD_WAKE_UP_METHOD,
        .cmd     = wake_up_method_cmd,
        .cmd_len = sizeof(wake_up_method_cmd),
    },
    {
        .opcode  = BT_HCI_CMD_SET_BT_SLEEP_MODE,
        .cmd     = set_bt_sleep_mode_cmd,
        .cmd_len = sizeof(set_bt_sleep_mode_cmd),
    },
    {
        .opcode  = BT_HCI_CMD_BLE_POWER_SAVE_MODE_CONFIG,
        .cmd     = ble_power_save_mode_config_cmd,
        .cmd_len = sizeof(ble_power_save_mode_config_cmd),
    }
};
#endif /* WIFI_IW416_BOARD_MURATA_1XK_M2 */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void controller_wakeup_pin_init(void);
static uint8_t controller_lowpower_config_cmd(void);
static void controller_c2h_callback(void *param);
static void controller_enable_wakeup_host_irq(void);
static void controller_disable_wakeup_host_irq(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static bool s_LowPowerPinInitialized = false;

/* H2C pin config  */
static GPIO_HANDLE_DEFINE(h2c_gpio_handle);
static hal_gpio_pin_config_t h2c_gpio_config = 
{
    .direction = kHAL_GpioDirectionOut,
    .level     = H2C_WAKEUP_VALUE,
    .port      = BOARD_BT_UART_H2C_PORT,
    .pin       = BOARD_BT_UART_H2C_PIN,
};

/* C2H pin config */
GPIO_HANDLE_DEFINE(c2h_wakeup_handle);
static hal_gpio_pin_config_t c2h_gpio_config = 
{
    .direction = kHAL_GpioDirectionIn,
    .level     = C2H_WAKEUP_VALUE,
    .port      = BOARD_BT_UART_C2H_PORT,
    .pin       = BOARD_BT_UART_C2H_PIN,
};

/*******************************************************************************
 * Public functions
 ******************************************************************************/
/*
 * brief Initialize wake up pin and send vendor specific HCI command to configure controller.
 *
 * return Zero on success or negative error code otherwise.
 */
uint8_t controller_low_power_init(void)
{
    uint8_t err = 0;

    /* Initialize wake up pin. */
    controller_wakeup_pin_init();

    /* Send vendor specific HCI command to configure controller. */
    err = controller_lowpower_config_cmd();

    s_LowPowerPinInitialized = true;

    return err;
}

/*
 * brief Check the power state of controller.
 *
 * return Zero on ready or 1 on not ready.
 */
uint8_t controller_power_state(void)
{
    uint8_t pinState;
    uint8_t ret = CONTROLLER_WAKE;

    if(s_LowPowerPinInitialized)
    {
        HAL_GpioGetInput(c2h_wakeup_handle, &pinState);

        if(pinState != C2H_WAKEUP_VALUE)
        {
            ret = CONTROLLER_SLEEP;
        }
    }

    return ret;
}

/*
 * brief Host allows controller to enter lowpower mode.
 *
 * return Zero on success.
 */
uint8_t controller_enter_low_power(void)
{
    int err = 1;
    
    if(s_LowPowerPinInitialized)
    {
        err = HAL_GpioSetOutput(h2c_gpio_handle, !H2C_WAKEUP_VALUE);

        controller_enable_wakeup_host_irq();
    }

    return err;
}

/*
 * brief Host wake up controller.
 *
 * return Zero on success.
 */
uint8_t controller_exit_low_power(void)
{
    int err = 1;
    
    if(s_LowPowerPinInitialized)
    {
        err = HAL_GpioSetOutput(h2c_gpio_handle, H2C_WAKEUP_VALUE);
        
        controller_disable_wakeup_host_irq();
    }

    return err;
}

/*******************************************************************************
 * Private functions
 ******************************************************************************/
static void controller_c2h_callback(void *param)
{
    controller_disable_wakeup_host_irq();
}

static void controller_wakeup_pin_init(void)
{
    /* Controller wake up pin init. */
    HAL_GpioInit(h2c_gpio_handle, &h2c_gpio_config);
    
    /* Host wake up pin init. */
    HAL_GpioInit(c2h_wakeup_handle, &c2h_gpio_config);

    /* Install C2H IRQ callback. */
    HAL_GpioInstallCallback(c2h_wakeup_handle, controller_c2h_callback, NULL);
}

static uint8_t controller_lowpower_config_cmd()
{
    uint8_t             err;
    struct net_buf *buf;
    /* Send vendor specific HCI command to configure controller low power feature. */
    for(uint8_t idx = 0U; idx < CONTROLLER_LOW_POWER_CONFIG_CMD_CNT; idx++)
    {
        buf = bt_hci_cmd_create(lp_mode_cfg_cmd[idx].opcode, lp_mode_cfg_cmd[idx].cmd_len);
        if (NULL == buf)
        {
            PRINTF("No buffer space available\r\n");
            err = -1;
            break;
        }

        net_buf_add_mem(buf, lp_mode_cfg_cmd[idx].cmd, lp_mode_cfg_cmd[idx].cmd_len);
        err = bt_hci_cmd_send_sync(lp_mode_cfg_cmd[idx].opcode, buf, NULL);
        if (0 != err)
        {
            PRINTF("Send hci command 0x%x failed (err %d)\n", lp_mode_cfg_cmd[idx].opcode, err);
            break;
        }
        else
        {
            OSA_TimeDelay(WAIT_CMD_COMPLETE);
            err = 0;
        }
    }

    PRINTF("Low power configuration successful.\r\n");
    return err;
}

static void controller_enable_wakeup_host_irq()
{
    if(s_LowPowerPinInitialized)
    {
        /* Clean GPIO interrupt flags. */
        GPIO_PinClearInterruptFlag(BOARD_BT_UART_C2H_GPIO, BOARD_BT_UART_C2H_PORT, BOARD_BT_UART_C2H_PIN, 0);
        NVIC_ClearPendingIRQ(BOARD_BT_UART_C2H_IRQ);

        /* Enable interrupt. */
        HAL_GpioSetTriggerMode(c2h_wakeup_handle, HOST_WAKEUP_MODE);
    }
}

static void controller_disable_wakeup_host_irq()
{
    if(s_LowPowerPinInitialized)
    {
        HAL_GpioSetTriggerMode(c2h_wakeup_handle, kHAL_GpioInterruptDisable);
    }
}
#endif /* APP_LOWPOWER_ENABLED */
