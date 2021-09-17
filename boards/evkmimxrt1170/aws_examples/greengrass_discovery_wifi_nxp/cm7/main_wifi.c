/*
 * FreeRTOS V1.0.0
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software. If you wish to use our Amazon
 * FreeRTOS name, please do so in a fair use way that does not cause confusion.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://aws.amazon.com/freertos
 * http://www.FreeRTOS.org
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
/* SDK Included Files */
#include "fsl_debug_console.h"
#include "ksdk_mbedtls.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
/* FreeRTOS Demo Includes */
#include "FreeRTOS.h"
#include "task.h"
#include "iot_logging_task.h"
#include "iot_system_init.h"
#include "aws_dev_mode_key_provisioning.h"
#include "platform/iot_threads.h"
#include "types/iot_network_types.h"
#include "aws_demo.h"

#include "fsl_common.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define LOGGING_TASK_PRIORITY   (tskIDLE_PRIORITY + 1)
#define LOGGING_TASK_STACK_SIZE (240)
#define LOGGING_QUEUE_LENGTH    (16)

#define DEMO_TASK_STACK_SIZE (configMINIMAL_STACK_SIZE * 22)
#define DEMO_TASK_PRIORITY   (tskIDLE_PRIORITY + 1)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* Declaration of demo function. */
extern int vStartGreenGrassDiscoveryTask(bool awsIotMqttMode,
                                         const char *pIdentifier,
                                         void *pNetworkServerInfo,
                                         void *pNetworkCredentialInfo,
                                         const IotNetworkInterface_t *pNetworkInterface);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
void print_string(const char *string)
{
    PRINTF(string);
}

void vApplicationDaemonTaskStartupHook(void)
{
    /* A simple example to demonstrate key and certificate provisioning in
     * microcontroller flash using PKCS#11 interface. This should be replaced
     * by production ready key provisioning mechanism. */
    vDevModeKeyProvisioning();

    if (SYSTEM_Init() == pdPASS)
    {
        static demoContext_t mqttDemoContext = {.networkTypes                = AWSIOT_NETWORK_TYPE_WIFI,
                                                .demoFunction                = vStartGreenGrassDiscoveryTask,
                                                .networkConnectedCallback    = NULL,
                                                .networkDisconnectedCallback = NULL};

        Iot_CreateDetachedThread(runDemoTask, &mqttDemoContext, DEMO_TASK_PRIORITY, DEMO_TASK_STACK_SIZE);
    }
}

int main(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
#if defined(WIFI_88W8987_BOARD_AW_CM358MA)
    /* Init SDIO_RST */
    BOARD_InitM2WifiResetPins();
#endif

#if defined(HOST_PDN_RESET)
    /* Init WL_RST */
    BOARD_InitWlRstPin();
#endif

    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
#if defined(WIFI_88W8987_BOARD_AW_CM358MA)
    /* Set SDIO_RST to 1 */
    GPIO_PinWrite(BOARD_INITM2WIFIRESETPINS_SDIO_RST_GPIO, BOARD_INITM2WIFIRESETPINS_SDIO_RST_GPIO_PIN, 1U);
#endif

#if defined(HOST_PDN_RESET)
    /* Set WL_RST to 1 */
    GPIO_PinWrite(BOARD_INITWLRSTPIN_WL_RST_GPIO, BOARD_INITWLRSTPIN_WL_RST_GPIO_PIN, 1U);
#endif
    CRYPTO_InitHardware();

    xLoggingTaskInitialize(LOGGING_TASK_STACK_SIZE, LOGGING_TASK_PRIORITY, LOGGING_QUEUE_LENGTH);

    vTaskStartScheduler();
    for (;;)
        ;
}

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
