/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"

#if (defined(WIFI_IW416_BOARD_AW_AM457_USD) || defined(WIFI_IW61x_BOARD_RD_USD) || \
     defined(WIFI_IW416_BOARD_AW_AM510_USD) || defined(WIFI_IW416_BOARD_AW_AM510MA) || \
     defined(WIFI_88W8987_BOARD_AW_CM358_USD) || defined(WIFI_88W8987_BOARD_AW_CM358MA) || \
     defined(WIFI_IW416_BOARD_MURATA_1XK_USD) || defined(WIFI_IW416_BOARD_MURATA_1XK_M2) || \
     defined(WIFI_88W8987_BOARD_MURATA_1ZM_USD) || defined (WIFI_88W8987_BOARD_MURATA_1ZM_M2))

#ifndef CONTROLLER_INIT_ESCAPE
#if defined(SD8978)
#include "sduartIW416_wlan_bt.h"
#elif defined(SD8987)
#include "sduart8987_wlan_bt.h"
#elif defined(IW61x)
#ifndef BT_THIRDPARTY_SUPPORT
#include "sduart_nw61x.h"
#endif
#else
#error The Wi-Fi module is unsupported
#endif
#endif /* CONTROLLER_INIT_ESCAPE */
#include "sdio.h"
#include "firmware_dnld.h"

#include "fsl_adapter_uart.h"
#include "fsl_os_abstraction.h"

#include "controller.h"
#include "controller_hci_uart.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BT_OP(ogf, ocf)                         ((ocf) | ((ogf) << 10))
#define BT_OGF_VS                               0x3f

/* Weak function. */
#if defined(__GNUC__)
#define __WEAK_FUNC __attribute__((weak))
#elif defined(__ICCARM__)
#define __WEAK_FUNC __weak
#elif defined(__CC_ARM) || defined(__ARMCC_VERSION)
#define __WEAK_FUNC __attribute__((weak))
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void controller_hci_uart_init(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static UART_HANDLE_DEFINE(s_controllerHciUartHandle);

/*******************************************************************************
 * Code
 ******************************************************************************/

__WEAK_FUNC int controller_hci_uart_get_configuration(controller_hci_uart_config_t *config);

#ifndef BT_THIRDPARTY_SUPPORT
/* Initialize the platform */
void controller_init(void)
{
#ifndef CONTROLLER_INIT_ESCAPE
    int result;
    /* Download firmware */
    result = sdio_init();
    assert(WM_SUCCESS == result);
    result = sdio_ioport_init();
    assert(WM_SUCCESS == result);
    result = firmware_download(wlan_fw_bin, wlan_fw_bin_len);
    assert(WM_SUCCESS == result);
    (void)result;
#endif
    controller_hci_uart_init();
}
#endif

__WEAK_FUNC int controller_hci_uart_get_configuration(controller_hci_uart_config_t *config)
{
    return -1;
}

static void controller_hci_uart_init(void)
{
    uint16_t *opcode;
    uint8_t  *param_len;
    hal_uart_config_t config;
    controller_hci_uart_config_t hciUartConfig;
    uint8_t sendingBuffer[8];
    uint8_t recvBuffer[8];
    hal_uart_status_t error;

    memset(sendingBuffer, 0, sizeof(sendingBuffer));
    memset(recvBuffer, 0, sizeof(recvBuffer));
    memset(&hciUartConfig, 0, sizeof(hciUartConfig));
    memset(&config, 0, sizeof(config));
#if (defined(WIFI_IW416_BOARD_AW_AM457_USD) || defined(WIFI_IW416_BOARD_AW_AM510_USD) || defined(WIFI_IW416_BOARD_AW_AM510MA) || \
     defined(WIFI_IW416_BOARD_MURATA_1XK_USD) || defined(WIFI_IW416_BOARD_MURATA_1XK_M2) || \
     defined(WIFI_88W8987_BOARD_MURATA_1ZM_USD) || defined (WIFI_88W8987_BOARD_MURATA_1ZM_M2))
    /*delay to make sure controller is ready to receive command*/
    OSA_TimeDelay(100);
#endif
    if (0 != controller_hci_uart_get_configuration(&hciUartConfig))
    {
        return;
    }
    if (hciUartConfig.runningBaudrate == hciUartConfig.defaultBaudrate)
    {
        return;
    }
    /* Set the HCI-UART Configuration parameters */
    config.srcClock_Hz  = hciUartConfig.clockSrc;
    config.baudRate_Bps = hciUartConfig.defaultBaudrate;
    config.parityMode   = kHAL_UartParityDisabled;
    config.stopBitCount = kHAL_UartOneStopBit;
    config.enableRx     = 1;
    config.enableTx     = 1;
    config.instance     = hciUartConfig.instance;
    config.enableRxRTS  = hciUartConfig.enableRxRTS;
    config.enableTxCTS  = hciUartConfig.enableTxCTS;
#if (defined(HAL_UART_ADAPTER_FIFO) && (HAL_UART_ADAPTER_FIFO > 0u))
    config.txFifoWatermark = 0U;
    config.rxFifoWatermark = 0U;
#endif

    /* Initialize UART with Adapter */
    error = HAL_UartInit
          (
              (hal_uart_handle_t)s_controllerHciUartHandle,
              &config
          );
    /* Check if Assert or Log and return? */
    assert(kStatus_HAL_UartSuccess == error);
    sendingBuffer[0] = 0x01;
    opcode = (uint16_t *)&sendingBuffer[1];
    param_len = &sendingBuffer[3];
    *opcode                                   = (uint16_t)BT_OP(BT_OGF_VS, 0x09);
    *param_len                                = sizeof(hciUartConfig.runningBaudrate);
    *((uint32_t *)(&sendingBuffer[4])) = hciUartConfig.runningBaudrate;
    /*delay to make sure controller is ready to receive command*/
    OSA_TimeDelay(60);
    error = HAL_UartSendBlocking((hal_uart_handle_t)s_controllerHciUartHandle, &sendingBuffer[0], 8);
    assert(kStatus_HAL_UartSuccess == error);
    error = HAL_UartReceiveBlocking((hal_uart_handle_t)s_controllerHciUartHandle, &recvBuffer[0], 7);
    assert(kStatus_HAL_UartSuccess == error);

    assert(0 == recvBuffer[7]);

    OSA_TimeDelay(500);
    error = HAL_UartDeinit((hal_uart_handle_t)s_controllerHciUartHandle);

    assert(kStatus_HAL_UartSuccess == error);

    (void)error;
}

#endif
