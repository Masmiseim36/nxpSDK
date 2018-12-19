/*
 * Amazon FreeRTOS V1.0.0
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
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

///////////////////////////////////////////////////////////////////////////////
//  Includes
///////////////////////////////////////////////////////////////////////////////
/* SDK Included Files */
#include "board.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "ksdk_mbedtls.h"

/* Amazon FreeRTOS Demo Includes */
#include "FreeRTOS.h"
#include "task.h"
#include "aws_clientcredential.h"
#include "aws_logging_task.h"
#include "aws_wifi.h"
#include "aws_system_init.h"

#include "device_configuration.h"

#include "clock_config.h"
#include "wifi_shield_gt202.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define LOGGING_TASK_PRIORITY (tskIDLE_PRIORITY + 1)
#define LOGGING_TASK_STACK_SIZE (200)
#define LOGGING_QUEUE_LENGTH (16)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern void vStartShadowDemoTasks(void);
static int prvWifiConnect(const WIFINetworkParams_t *pxNetworkParams);

/*******************************************************************************
 * Variables
 ******************************************************************************/
const WIFINetworkParams_t pxDefaultNetworkParams = {
    .pcSSID = "aws_iot_ap", .pcPassword = "admin1234", .xSecurity = eWiFiSecurityOpen, /*eWiFiSecurityWPA2,*/
};

/*******************************************************************************
 * Code
 ******************************************************************************/
void wifiCreateAP()
{
    WIFIReturnCode_t result = eWiFiSuccess;
    (void)result;
    result = WIFI_ConfigureAP(&pxDefaultNetworkParams);
    assert(eWiFiSuccess == result);
    (void)result;
    configPRINTF(("WiFi created AP %s.\r\n", pxDefaultNetworkParams.pcSSID));
    dev_cfg_init_config_server(Custom_Api_GetDriverCxt(0), kDEV_CFG_WIFI_AP);
}

void vApplicationDaemonTaskStartupHook(void)
{
    configPRINTF(("uxTaskGetStackHighWaterMark(): %d\n", uxTaskGetStackHighWaterMark(NULL)));
    if (SYSTEM_Init() == pdPASS)
    {
        /* Initialize mflash file system for device configuration */
        if (dev_cfg_init() != 0)
        {
            PRINTF("Failed to initialze device for configuration\r\n");
            while (1)
                ;
        }

        /* Turns on WiFi */
        configPRINTF(("Starting WiFi...\r\n"));
        if (WIFI_On() != eWiFiSuccess)
        {
            configPRINTF(("WiFi initialization failed.\r\n"));
            return;
        }
        configPRINTF(("WiFi module initialized.\r\n"));

        /* Check if the device has configured parameters of WiFi network */
        WIFINetworkParams_t xNetworkParams;
        memset(&xNetworkParams, 0, sizeof(xNetworkParams));
        if (dev_cfg_get_wifi_params(&xNetworkParams) != 0)
        {
            configPRINTF(("Device has not set WIFI credentials\r\n"));
            configPRINTF(("Creating default WIFI AP SSID: %s, use mobile app to configure the device\r\n",
                          pxDefaultNetworkParams.pcSSID));
            wifiCreateAP();

            return;
        }
        else
        {
            if (prvWifiConnect(&xNetworkParams) != 0)
            {
                configPRINTF(("Failed to connect to wifi\r\n"));
                configPRINTF(("Creating default WIFI AP SSID: %s, use mobile app to configure the device\r\n",
                              pxDefaultNetworkParams.pcSSID));
                wifiCreateAP();

                return;
            }
        }

        /* Initialize device for configuration by mobile app - start SSL server and mDNS responder */
        dev_cfg_init_config_server(Custom_Api_GetDriverCxt(0), kDEV_CFG_WIFI_CLIENT);

        /* Check if the device is configured */
        if (dev_cfg_check_aws_credentials() != 0)
        {
            configPRINTF(("Device has not set aws credentials, use mobile app to configure the device\r\n"));
            return;
        }

        /* Device is configured, start AWS demo */
        vStartShadowDemoTasks();
    }
    configPRINTF(("uxTaskGetStackHighWaterMark(): %d\n", uxTaskGetStackHighWaterMark(NULL)));
}

int main(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /*Set clock source for LPSPI*/
    CLOCK_SetMux(kCLOCK_LpspiMux, WIFISHIELD_SPI_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_LpspiDiv, WIFISHIELD_SPI_CLOCK_SOURCE_DIVIDER);

    SCB_DisableDCache();
    CRYPTO_InitHardware();

    xLoggingTaskInitialize(LOGGING_TASK_STACK_SIZE, LOGGING_TASK_PRIORITY, LOGGING_QUEUE_LENGTH);

    vTaskStartScheduler();
    for (;;)
        ;
}

static int prvWifiConnect(const WIFINetworkParams_t *pxNetworkParams)
{
    WIFIReturnCode_t result;
    configPRINTF(("Connecting to WiFi AP: %s\r\n", pxNetworkParams->pcSSID));
    result = WIFI_ConnectAP(pxNetworkParams);
    if (eWiFiSuccess != result)
    {
        return -1;
    }
    configPRINTF(("Connected\r\n"));

    uint8_t tmp_ip[4] = {0};
    result = WIFI_GetIP(tmp_ip);
    if (result != eWiFiSuccess)
    {
        configPRINTF(("Could not get IP address, reason %d.\r\n", result));
        return result;
    }
    configPRINTF(("IP Address acquired %d.%d.%d.%d\r\n", tmp_ip[0], tmp_ip[1], tmp_ip[2], tmp_ip[3]));

    return result;
}

/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
 * implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
 * used by the Idle task. */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize)
{
    /* If the buffers to be provided to the Idle task are declared inside this
     * function then they must be declared static - otherwise they will be allocated on
     * the stack and so not exists after this function exits. */
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle
     * task's state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
 * application must provide an implementation of vApplicationGetTimerTaskMemory()
 * to provide the memory that is used by the Timer service task. */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize)
{
    /* If the buffers to be provided to the Timer task are declared inside this
     * function then they must be declared static - otherwise they will be allocated on
     * the stack and so not exists after this function exits. */
    static StaticTask_t xTimerTaskTCB;
    static StackType_t uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH];

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
     * task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configTIMER_TASK_STACK_DEPTH is specified in words, not bytes. */
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
/**
 * @brief Warn user if pvPortMalloc fails.
 *
 * Called if a call to pvPortMalloc() fails because there is insufficient
 * free memory available in the FreeRTOS heap.  pvPortMalloc() is called
 * internally by FreeRTOS API functions that create tasks, queues, software
 * timers, and semaphores.  The size of the FreeRTOS heap is set by the
 * configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h.
 *
 */
void vApplicationMallocFailedHook()
{
    configPRINTF(("ERROR: Malloc failed to allocate memory\r\n"));
}

/**
 * @brief Loop forever if stack overflow is detected.
 *
 * If configCHECK_FOR_STACK_OVERFLOW is set to 1,
 * this hook provides a location for applications to
 * define a response to a stack overflow.
 *
 * Use this hook to help identify that a stack overflow
 * has occurred.
 *
 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    portDISABLE_INTERRUPTS();

    PRINTF("vApplicationStackOverflowHook\n");
    /* Loop forever */
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
