//------------------------------------------------------------------------------
// Copyright (c) 2017 Qualcomm Technologies, Inc.
// All rights reserved.
// Redistribution and use in source and binary forms, with or without modification, are permitted (subject to the
// limitations in the disclaimer below)
// provided that the following conditions are met:
// Redistributions of source code must retain the above copyright notice, this list of conditions and the following
// disclaimer.
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation and/or other materials provided with the
// distribution.
// Neither the name of Qualcomm Technologies, Inc. nor the names of its contributors may be used to endorse or promote
// products derived
// from this software without specific prior written permission.
// NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
// INCLUDING,
// BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY,
// OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

///////////////////////////////////////////////////////////////////////////////
//  Includes
///////////////////////////////////////////////////////////////////////////////

// SDK Included Files
#include "board.h"
#include "fsl_debug_console.h"
#include "qcom_api.h"

#include "atheros_wifi.h"
#include "atheros_wifi_api.h"
#include "atheros_wifi_internal.h"

#include "hw20_apb_map.h"
#include "hw20_mbox_reg.h"
#include "hw40_rtc_reg.h"
#include "targaddrs.h"
#include "bmi.h"

#include "fw_serial.h"

#if ((WIFISHIELD_IS) != (WIFISHIELD_IS_GT202))
#error "FW-update demo supports only GT202 WiFi shield"
#endif

#include "fsl_device_registers.h"
#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#include "FreeRTOSConfig.h"
/* UART instance and clock */
#define DEMO_UART BOARD_DEBUG_UART_BASEADDR
#define DEMO_UART_BAUDRATE BOARD_DEBUG_UART_BAUDRATE
#define DEMO_UART_CLK_FREQ BOARD_DEBUG_UART_CLK_FREQ
#define DEMO_UART_RX_TX_IRQn BOARD_UART_IRQ
#ifdef configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY
#define DEMO_UART_RX_TX_IRQp ((configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY) + 1)
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

const int TASK_MAIN_PRIO       = configMAX_PRIORITIES - 3;
const int TASK_MAIN_STACK_SIZE = 800;

portSTACK_TYPE *task_main_stack = NULL;
TaskHandle_t task_main_task_handler;

/*!
 * @brief versions struct
 */
typedef PREPACK struct
{
    uint32_t host_ver FIELD_PACKED;
    uint32_t target_ver FIELD_PACKED;
    uint32_t wlan_ver FIELD_PACKED;
    uint32_t abi_ver FIELD_PACKED;
} POSTPACK fw_version_t;

/*!
 * @brief request struct
 */
typedef PREPACK struct
{
    uint16_t cmdId FIELD_PACKED;
    uint8_t buffer[1024] FIELD_PACKED;
    uint32_t addr FIELD_PACKED;
    uint32_t option FIELD_PACKED;
    uint16_t length FIELD_PACKED;
    uint16_t crc FIELD_PACKED;
} POSTPACK fw_request_t;

/*!
 * @brief response struct
 */
typedef PREPACK struct
{
    uint16_t cmdId FIELD_PACKED;
    uint32_t result FIELD_PACKED;
    fw_version_t version FIELD_PACKED;
} POSTPACK fw_response_t;

/*!
 * @brief command IDs
 */
enum cmd_id
{
    FW_CMD_PROGRAM = 0,
    FW_CMD_PROGRAM_RESULT,
    FW_CMD_EXECUTE,
    FW_CMD_EXECUTE_RESULT,
    FW_CMD_CRC_ERROR,
    FW_CMD_VERSION_NUM,
    FW_CMD_FW_VERSION,
    FW_CMD_UNKNOWN_CMD
};

/* declarations to access wifi internal fn and structs */
void reset_atheros_driver_setup_init(void);
uint32_t Custom_Api_Mediactl(QCA_CONTEXT_STRUCT_PTR qca_ptr, uint32_t command_id, void *inout_param);
extern QCA_CONTEXT_STRUCT wifiCtx;
extern ATH_CUSTOM_INIT_T ath_custom_init;
extern uint32_t ar4XXX_boot_param;

/*!
 * @brief Taken from former code, used as a callback to configure BMI
 */
static A_STATUS Driver_BMIConfig(void *pCxt)
{
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    /* The config is contained within the driver itself */
    uint32_t param = 0, options = 0, sleep = 0, address = 0;
    (void)options;
    (void)sleep;

    if (pDCxt->targetVersion != ATH_FIRMWARE_TARGET && pDCxt->targetVersion != TARGET_AR400X_REV2 &&
        pDCxt->targetVersion != TARGET_AR400X_REV3 && (pDCxt->targetVersion != TARGET_AR400X_REV4))
    {
        /* compatible wifi chip version is decided at compile time */
        A_ASSERT(0);
        return A_ERROR;
    }

    /* Temporarily disable system sleep */
    address = MBOX_BASE_ADDRESS + LOCAL_SCRATCH_ADDRESS;
    if (A_OK != BMIReadSOCRegister(pCxt, address, &param))
    {
        return A_ERROR;
    }
    options = A_LE2CPU32(param);
    param |= A_CPU2LE32(AR6K_OPTION_SLEEP_DISABLE);
    if (A_OK != BMIWriteSOCRegister(pCxt, address, param))
    {
        return A_ERROR;
    }
    address = RTC_BASE_ADDRESS + SYSTEM_SLEEP_ADDRESS;
    if (A_OK != BMIReadSOCRegister(pCxt, address, &param))
    {
        return A_ERROR;
    }
    sleep = A_LE2CPU32(param);
    param |= A_CPU2LE32(WLAN_SYSTEM_SLEEP_DISABLE_SET(1));
    if (A_OK != BMIReadSOCRegister(pCxt, address, &param))
    {
        return A_ERROR;
    }

    /* Run at 40/44MHz by default */
    param   = CPU_CLOCK_STANDARD_SET(0);
    address = RTC_BASE_ADDRESS + CPU_CLOCK_ADDRESS;
    if (A_OK != BMIWriteSOCRegister(pCxt, address, A_CPU2LE32(param)))
    {
        return A_ERROR;
    }

    address = RTC_BASE_ADDRESS + LPO_CAL_ADDRESS;
    param   = LPO_CAL_ENABLE_SET(1);
    if (A_OK != BMIWriteSOCRegister(pCxt, address, A_CPU2LE32(param)))
    {
        return A_ERROR;
    }

    return A_OK;
}

/*!
 * @brief Taken from former code, restart driver
 */
int32_t ath_driver_reset(void)
{
    int32_t error;
    ATH_IOCTL_PARAM_STRUCT inout_param;
    uint32_t value;

    reset_atheros_driver_setup_init();

    inout_param.cmd_id = ATH_CHIP_STATE;
    inout_param.length = sizeof(value);

    /*Bring the driver down*/
    value            = 0;
    inout_param.data = &value;
    error            = Custom_Api_Mediactl(&wifiCtx, QCA_MEDIACTL_VENDOR_SPECIFIC, &inout_param);

    if (error != A_OK)
    {
        return A_ERROR;
    }

    /*Bring the driver up*/
    value            = 1;
    inout_param.data = &value;
    error            = Custom_Api_Mediactl(&wifiCtx, QCA_MEDIACTL_VENDOR_SPECIFIC, &inout_param);

    return error == A_OK ? A_OK : A_ERROR;
}

/*!
 * @brief Taken from former code, overwrite internal options to
 * initialize driver in BMI mode
 */
void atheros_driver_setup_init(void)
{
    ath_custom_init.Driver_TargetConfig = NULL;
    ath_custom_init.Driver_BootComm     = NULL;
    ath_custom_init.Driver_BMIConfig    = Driver_BMIConfig;
    ath_custom_init.skipWmi             = true;
    ath_custom_init.exitAtBmi           = true;

    ar4XXX_boot_param = AR4XXX_PARAM_MODE_BMI;
}

/*!
 * @brief Taken from former code, overwrite internal options to
 * restart driver
 */
void reset_atheros_driver_setup_init(void)
{
    ath_custom_init.Driver_TargetConfig = NULL;
    ath_custom_init.Driver_BootComm     = NULL;
    ath_custom_init.Driver_BMIConfig    = NULL;
    ath_custom_init.skipWmi             = false;
    ath_custom_init.exitAtBmi           = false;

    ar4XXX_boot_param = AR4XXX_PARAM_MODE_NORMAL;
}

fw_request_t g_request   = {0};
fw_response_t g_response = {0};

/*!
 * @brief Transfer 1K payload to GT202 module
 */
int32_t fw_cmd_program(fw_request_t *request, fw_response_t *response)
{
    ATH_PROGRAM_FLASH_STRUCT flash_msg = {0};
    ATH_IOCTL_PARAM_STRUCT inout_param = {0};
    int32_t result                     = 0;
    (void)result;

    response->cmdId     = FW_CMD_PROGRAM_RESULT;
    response->cmdId     = A_BE2CPU16(response->cmdId);
    flash_msg.load_addr = A_LE2CPU32(request->addr);
    flash_msg.length    = A_LE2CPU16(request->length);
    flash_msg.result    = 0;
    flash_msg.buffer    = request->buffer;

    inout_param.cmd_id = ATH_PROGRAM_FLASH;
    inout_param.data   = &flash_msg;
    inout_param.length = sizeof(flash_msg);

    result = Custom_Api_Mediactl(&wifiCtx, QCA_MEDIACTL_VENDOR_SPECIFIC, &inout_param);
    assert(result == 0);

    response->result = ((ATH_PROGRAM_FLASH_STRUCT *)inout_param.data)->result;
    response->result = A_BE2CPU32(response->result);
    return 0;
}

/*!
 * @brief Flash data, invoked at the end of the transfer
 */
int32_t fw_cmd_execute(fw_request_t *request, fw_response_t *response)
{
    ATH_PROGRAM_FLASH_STRUCT flash_msg = {0};
    ATH_IOCTL_PARAM_STRUCT inout_param = {0};
    int32_t result                     = 0;
    (void)result;

    response->cmdId     = FW_CMD_EXECUTE_RESULT;
    response->cmdId     = A_BE2CPU16(response->cmdId);
    flash_msg.load_addr = A_LE2CPU32(request->addr);
    flash_msg.length    = A_LE2CPU16(request->length);
    flash_msg.result    = A_LE2CPU32(request->option);
    flash_msg.buffer    = NULL;

    inout_param.cmd_id = ATH_EXECUTE_FLASH;
    inout_param.data   = &flash_msg;
    inout_param.length = sizeof(flash_msg);

    result = Custom_Api_Mediactl(&wifiCtx, QCA_MEDIACTL_VENDOR_SPECIFIC, &inout_param);
    assert(result == 0);

    response->result = ((ATH_PROGRAM_FLASH_STRUCT *)inout_param.data)->result;

    return 0;
}

/*!
 * @brief Get version of this application
 */
int32_t fw_cmd_sw_version(fw_request_t *request, fw_response_t *response)
{
    /* host.exe checks compatibility, to pass the test the return value must be 4 */
    response->cmdId  = FW_CMD_VERSION_NUM;
    response->result = 4;
    response->cmdId  = A_BE2CPU16(response->cmdId);
    response->result = A_BE2CPU32(response->result);
    return 0;
}

/*!
 * @brief Get firmware version
 */
int32_t fw_cmd_fw_version(fw_request_t *request, fw_response_t *response)
{
    uint32_t tmp = 0;

#if 0
    ATH_IOCTL_PARAM_STRUCT inout_param = {0};
    fw_version_t version = {0};
    int32_t result = 0;

    response->cmdId = FW_CMD_FW_VERSION;
    response->cmdId = A_BE2CPU16(response->cmdId);

    ath_driver_reset();
    inout_param.cmd_id = ATH_GET_VERSION;
    inout_param.data = &version;
    inout_param.length = 16;

    result = Custom_Api_Mediactl(&wifiCtx, QCA_MEDIACTL_VENDOR_SPECIFIC, &inout_param);
    assert(result == 0);

    if (A_OK != result)
    {
        response->result = 1;
    }
    else
    {
        response->result = 0;
        response->version.host_ver = A_BE2CPU32(version.host_ver);
        response->version.target_ver = A_BE2CPU32(version.target_ver);
        response->version.wlan_ver = A_BE2CPU32(version.wlan_ver);
        response->version.abi_ver = A_BE2CPU32(version.abi_ver);
    }
#endif

    /* Set fake version numbers because we cannot switch to WMI mode */
    response->cmdId              = A_BE2CPU16(FW_CMD_FW_VERSION);
    response->result             = 0;
    tmp                          = 0x33000000UL; // "3.3.0.0"
    response->version.host_ver   = A_BE2CPU32(tmp);
    tmp                          = 0x31C80997UL; // "0x31c80997"
    response->version.target_ver = A_BE2CPU32(tmp);
    tmp                          = 0x33050074UL; // "3.3.5.116"
    response->version.wlan_ver   = A_BE2CPU32(tmp);
    tmp                          = 0x1UL; // "1"
    response->version.abi_ver    = A_BE2CPU32(tmp);

    return 0;
}

/*!
 * @brief calculate crc using CRC-16/XMODEM mode
 */
uint16_t crc16(uint8_t *ptr, int count)
{
    uint16_t crc, i;

    crc = 0;
    while (--count >= 0)
    {
        crc = crc ^ (int)*ptr++ << 8;
        for (i = 0; i < 8; ++i)
            if (crc & 0x8000)
                crc = crc << 1 ^ 0x1021;
            else
                crc = crc << 1;
    }
    return (crc & 0xFFFF);
}

/*!
 * @brief Receive request packet
 */
int32_t fw_receive_request(fw_request_t *request)
{
    size_t received_n = 0;
    size_t request_n  = sizeof(*request);

    int32_t error = fw_serial_receive((uint8_t *)request, sizeof(*request), &received_n);
    if ((kStatus_Success != error) || (received_n != request_n))
    {
        return -1;
    }

    uint32_t tmp_crc = crc16((uint8_t *)request, sizeof(*request) - sizeof(request->crc));
    if (tmp_crc != request->crc)
    {
        return -1;
    }

    return 0;
}

/*!
 * @brief Send response back to host to acknowledge request
 */
int32_t fw_send_response(fw_response_t *response)
{
    int error = fw_serial_send((uint8_t *)response, sizeof(*response));
    return (error == kStatus_Success) ? 0 : -1;
}

/*!
 * @brief Initialize sequence to start
 */
int32_t fw_init(void)
{
    return fw_serial_init((uint8_t *)DEMO_UART, DEMO_UART_BAUDRATE, DEMO_UART_CLK_FREQ);
}

/*!
 * @brief Main app task
 */
void task_main(void *param)
{
    int32_t result;
    extern int wlan_driver_start(void);

    /* Initialize WIFI shield */
    result = WIFISHIELD_Init();
    assert(A_OK == result);

    /* Override default settings */
    atheros_driver_setup_init();

    /* Initialize the WIFI driver (thus starting the driver task) */
    result = wlan_driver_start();
    assert(A_OK == result);

    fw_init();
    do
    {
        result = fw_receive_request(&g_request);
        if (0 == result)
        {
            switch (g_request.cmdId)
            {
                case FW_CMD_PROGRAM:
                    fw_cmd_program(&g_request, &g_response);
                    break;
                case FW_CMD_EXECUTE:
                    fw_cmd_execute(&g_request, &g_response);
                    break;
                case FW_CMD_VERSION_NUM:
                    fw_cmd_sw_version(&g_request, &g_response);
                    break;
                // Fake one, need to solve driver restart issue first.
                case FW_CMD_FW_VERSION:
                    fw_cmd_fw_version(&g_request, &g_response);
                    break;
            }
        }
        else
        {
            g_response.cmdId = FW_CMD_CRC_ERROR;
        }
        fw_send_response(&g_response);
    } while (1);
}

int main(void)
{
    BaseType_t result = 0;
    (void)result;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
#ifdef DEMO_UART_RX_TX_IRQp
    NVIC_SetPriority(DEMO_UART_RX_TX_IRQn, DEMO_UART_RX_TX_IRQp);
#endif

    result =
        xTaskCreate(task_main, "main", TASK_MAIN_STACK_SIZE, task_main_stack, TASK_MAIN_PRIO, &task_main_task_handler);
    assert(pdPASS == result);

    vTaskStartScheduler();
    for (;;)
        ;
}
