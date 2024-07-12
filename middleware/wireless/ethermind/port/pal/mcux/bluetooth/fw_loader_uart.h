/** @file fw_loader_uart.h
 *
 *  @brief  This file provides firmware download related API for BT only fw download
 *
 *  Copyright 2021-2023 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */
/*===================== Include Files ============================================*/
#ifndef FW_LOADER_UART_H
#define FW_LOADER_UART_H

#include <stdbool.h>
#include <stdint.h>
/*===================== Macros ===================================================*/

/*==================== Typedefs =================================================*/
typedef enum
{
    FW_DOWNLOAD_UART_BL_VER_NOT_SUPP     = -8,
    FW_DOWNLOAD_UART_CHANGE_BR_FAIL      = -7,
    FW_DOWNLOAD_UART_HDR_SIG_TIMEOUT     = -6,
    FW_DOWNLOAD_UART_SECOND_BR_NOT_SUPP  = -5,
    FW_DOWNLOAD_UART_CHANGE_TIMEOUT_FAIL = -4,
    FW_DOWNLOAD_UART_ZERO_FILE_SIZE      = -3,
    FW_DOWNLOAD_UART_INVALID_OBJ         = -2,
    FW_DOWNLOAD_UART_FAILURE             = -1,
    FW_DOWNLOAD_UART_SUCCESS,
    FW_DOWNLOAD_UART_INPROGRESS,
    FW_DOWNLOAD_STATUS_INVALID
} fw_download_uart_status_e;

typedef struct uart_config
{
    void *lpUart;
    uint32_t uartClkFreq;
    uint32_t primaryBaudRate;
    uint32_t secondaryBaudRate;
    uint8_t parity;
    uint8_t uartInstance;
    uint8_t stopBits;
    bool enableTx;
    bool enableRx;
    bool enableRTS;
    bool enableCTS;
    bool isSecondaryBaudRateReq;
} fw_download_uart_config_t;

typedef struct uart_fw_download_settings
{
    fw_download_uart_config_t uartConfig;
    bool wait4HdrSig;
    bool isFwDownloadRetry;
} fw_download_setting_t;
/*===================== Global Vars ==============================================*/

/*==================== Function Prototypes ======================================*/
/******************************************************************************
 *
 * Name: uart_init_interface
 *
 * Description:
 *   This function initialize UART specific instance to download firmware over it
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   void* settings : pointer to interface structure passed by an application
 *
 * Return Value:
 *   void * : pointer to interface structure given back to application
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
void *uart_init_interface(void);

#endif // FW_LOADER_UART_H
