/*
Amazon FreeRTOS
Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
Copyright 2016-2017 NXP

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 http://aws.amazon.com/freertos
 http://www.FreeRTOS.org
*/

///////////////////////////////////////////////////////////////////////////////
//  Includes
///////////////////////////////////////////////////////////////////////////////

/* SDK Included Files */
#include "board.h"
#include "fsl_debug_console.h"

#include "pin_mux.h"
#include <stdbool.h>

/* Amazon FreeRTOS Includes */
#include "FreeRTOS.h"
#include "task.h"
#include "aws_clientcredential.h"
#include "iot_logging_task.h"
#include "iot_wifi.h"
#include "iot_system_init.h"
#include "aws_test_runner.h"
#include "aws_dev_mode_key_provisioning.h"

#include "fsl_device_registers.h"
#include "clock_config.h"
#include "ksdk_mbedtls.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/* The lpc54018 usb logging driver calls a blocking write function. Since this
 * task is the lowest priority, all of the test task priorities must be higher than
 * this to run. */
#define mainLOGGING_TASK_PRIORITY (tskIDLE_PRIORITY)
#define mainLOGGING_TASK_STACK_SIZE (configMINIMAL_STACK_SIZE * 4)
#define mainLOGGING_QUEUE_LENGTH (16)

/* Test runner task defines. */
#define mainTEST_RUNNER_TASK_STACK_SIZE (configMINIMAL_STACK_SIZE * 16)

/* The task delay for allowing the lower priority logging task to print out Wi-Fi
 * failure status before blocking indefinitely. */
#define mainLOGGING_WIFI_STATUS_DELAY pdMS_TO_TICKS(1000)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static void prvWifiConnect(void);

/*******************************************************************************
 * Code
 ******************************************************************************/

int main(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CRYPTO_InitHardware();

    xLoggingTaskInitialize(mainLOGGING_TASK_STACK_SIZE, mainLOGGING_TASK_PRIORITY, mainLOGGING_QUEUE_LENGTH);

    vTaskStartScheduler();
    for (;;)
        ;
}

void vApplicationDaemonTaskStartupHook(void)
{
    if (SYSTEM_Init() == pdPASS)
    {
        prvWifiConnect();

        /* A simple example to demonstrate key and certificate provisioning in
         * microcontroller flash using PKCS#11 interface. This should be replaced
         * by production ready key provisioning mechanism. */
        vDevModeKeyProvisioning();

        /* Create the task to run tests. */
        if (xTaskCreate(TEST_RUNNER_RunTests_task, "TestRunner", mainTEST_RUNNER_TASK_STACK_SIZE, NULL,
                        tskIDLE_PRIORITY + 1, NULL) != pdPASS)
        {
            PRINTF("Failed to create TestRunner task\n");
            while (1)
            {
            }
        }
    }
}

/*-----------------------------------------------------------*/

static void prvWifiConnect(void)
{
    WIFINetworkParams_t xNetworkParams;
    WIFIReturnCode_t xWifiStatus;
    uint8_t ucTempIp[4];

    /* Initialize Network params. */
    xNetworkParams.pcSSID           = clientcredentialWIFI_SSID;
    xNetworkParams.ucSSIDLength     = sizeof(clientcredentialWIFI_SSID);
    xNetworkParams.pcPassword       = clientcredentialWIFI_PASSWORD;
    xNetworkParams.ucPasswordLength = sizeof(clientcredentialWIFI_PASSWORD);
    xNetworkParams.xSecurity        = clientcredentialWIFI_SECURITY;
    xNetworkParams.cChannel         = 0;

    configPRINTF(("Starting Wi-Fi...\r\n"));

    xWifiStatus = WIFI_On();
    if (xWifiStatus == eWiFiSuccess)
    {
        configPRINTF(("Wi-Fi module initialized. Connecting to AP %s...\r\n", xNetworkParams.pcSSID));
    }
    else
    {
        configPRINTF(("Wi-Fi module failed to initialize.\r\n"));

        /* Delay to allow the lower priority logging task to print the above status. */
        vTaskDelay(pdMS_TO_TICKS(mainLOGGING_WIFI_STATUS_DELAY));

        while (1)
        {
        }
    }

    xWifiStatus = WIFI_ConnectAP(&xNetworkParams);
    if (xWifiStatus == eWiFiSuccess)
    {
        configPRINTF(("Wi-Fi connected to AP %s.\r\n", xNetworkParams.pcSSID));

        xWifiStatus = WIFI_GetIP(ucTempIp);
        if (eWiFiSuccess == xWifiStatus)
        {
            configPRINTF(("IP Address acquired %d.%d.%d.%d\r\n", ucTempIp[0], ucTempIp[1], ucTempIp[2], ucTempIp[3]));
        }
    }
    else
    {
        configPRINTF(("Wi-Fi failed to connect to AP %s.\r\n", xNetworkParams.pcSSID));

        /* Delay to allow the lower priority logging task to print the above status. */
        vTaskDelay(pdMS_TO_TICKS(mainLOGGING_WIFI_STATUS_DELAY));

        while (1)
        {
        }
    }
}

/*-----------------------------------------------------------*/

void *pvPortCalloc(size_t xNum, size_t xSize)
{
    void *pvReturn;

    pvReturn = pvPortMalloc(xNum * xSize);
    if (pvReturn != NULL)
    {
        memset(pvReturn, 0x00, xNum * xSize);
    }

    return pvReturn;
}
