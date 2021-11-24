/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief  shell over UART HAL driver implementation.
 */

#include "board_define.h"
#include <stdarg.h>
#include "fsl_component_serial_manager.h"
#include "fsl_debug_console.h"
#include "fsl_shell.h"

#include "fwk_input_manager.h"
#include "fwk_common.h"
#include "fwk_log.h"
#include "hal_input_dev.h"
#include "hal_lpm_dev.h"

/*******************************************************************************
 * Defines
 ******************************************************************************/
#define SHELL_PROMPT "SHELL>> "
#define SHELL_NAME   "shell_uart"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static hal_input_status_t HAL_InputDev_ShellUart_Init(input_dev_t *dev, input_dev_callback_t callback);
static hal_input_status_t HAL_InputDev_ShellUart_Deinit(const input_dev_t *dev);
static hal_input_status_t HAL_InputDev_ShellUart_Start(const input_dev_t *dev);
static hal_input_status_t HAL_InputDev_ShellUart_Stop(const input_dev_t *dev);
static hal_input_status_t HAL_InputDev_ShellUart_InputNotify(const input_dev_t *dev, void *param);

/*******************************************************************************
 * Global Variables
 ******************************************************************************/
SDK_ALIGN(static uint8_t uart_shellHandleBuffer[SHELL_HANDLE_SIZE], 4);
static shell_handle_t uart_shellHandle;

const static input_dev_operator_t s_InputDev_ShellUartOps = {
    .init        = HAL_InputDev_ShellUart_Init,
    .deinit      = HAL_InputDev_ShellUart_Deinit,
    .start       = HAL_InputDev_ShellUart_Start,
    .stop        = HAL_InputDev_ShellUart_Stop,
    .inputNotify = HAL_InputDev_ShellUart_InputNotify,
};

static input_dev_t s_InputDev_ShellUart = {
    .id = 1, .name = SHELL_NAME, .ops = &s_InputDev_ShellUartOps, .cap = {.callback = NULL}};

__attribute__((weak)) void APP_InputDev_Shell_RegisterShellCommands(shell_handle_t shellContextHandle,
                                                                    input_dev_t *shellDev,
                                                                    input_dev_callback_t callback)
{
}

static hal_input_status_t HAL_InputDev_ShellUart_Init(input_dev_t *dev, input_dev_callback_t callback)
{
    hal_input_status_t error          = kStatus_HAL_InputSuccess;
    s_InputDev_ShellUart.cap.callback = callback;
    uart_shellHandle                  = &uart_shellHandleBuffer[0];
    DbgConsole_Flush();
    SHELL_Init(uart_shellHandle, g_serialHandle, (char *)SHELL_PROMPT);
    /* Register externally-defined commands w/ the shell */
    APP_InputDev_Shell_RegisterShellCommands(uart_shellHandle, dev, callback);

    return error;
}

static hal_input_status_t HAL_InputDev_ShellUart_Deinit(const input_dev_t *dev)
{
    hal_input_status_t error = kStatus_HAL_InputSuccess;
    return error;
}

static hal_input_status_t HAL_InputDev_ShellUart_Start(const input_dev_t *dev)
{
    hal_input_status_t error = kStatus_HAL_InputSuccess;
    LOGD("++input_dev_shell_uart_start");

    LOGD("--input_dev_shell_uart_start");
    return error;
}

static hal_input_status_t HAL_InputDev_ShellUart_Stop(const input_dev_t *dev)
{
    hal_input_status_t error = 0;
    LOGD("++input_dev_shell_uart_stop");

    LOGD("--input_dev_shell_uart_stop");
    return error;
}

static hal_input_status_t HAL_InputDev_ShellUart_InputNotify(const input_dev_t *dev, void *param)
{
    hal_input_status_t error = kStatus_HAL_InputSuccess;
    return error;
}

int HAL_InputDev_ShellUart_Register()
{
    int error = 0;
    LOGD("input_dev_shell_uart_register");
    error = FWK_InputManager_DeviceRegister(&s_InputDev_ShellUart);
    return error;
}
