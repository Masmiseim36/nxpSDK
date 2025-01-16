/*
 * Lab-Project-coreMQTT-Agent 201215
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 * Copyright 2022-2024 NXP
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 */

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "board.h"

#if defined(MBEDTLS_MCUX_ELE_S400_API)
#include "ele_mbedtls.h"
#elif defined(MBEDTLS_MCUX_ELS_PKC_API)
#include "platform_hw_ip.h"
#include "els_pkc_mbedtls.h"
#elif defined(PSA_CRYPTO_DRIVER_ELS_PKC)
#include "psa/crypto.h"
#include "threading_alt.h"
#else
#include "ksdk_mbedtls.h"
#endif

#include "mflash_file.h"
#include "kvstore.h"
#include "app.h"

#include "mqtt_agent_task.h"
#include "aws_dev_mode_key_provisioning.h"
#include "core_pkcs11_config.h"

#include "aws_clientcredential.h"
#include "wpl.h"

#include "logging.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/**
 * @brief Stack size and priority for shadow device sync task.
 */
#define appmainSHADOW_DEVICE_TASK_STACK_SIZE (256)
#define appmainSHADOW_DEVICE_TASK_PRIORITY   (tskIDLE_PRIORITY + 1)

/**
 * @brief Stack size and priority for shadow update application task.
 */
#define appmainSHADOW_UPDATE_TASK_STACK_SIZE (256)
#define appmainSHADOW_UPDATE_TASK_PRIORITY   (tskIDLE_PRIORITY + 1)

/**
 * @brief Stack size and priority for MQTT agent task.
 * Stack size is capped to an adequate value based on requirements from MbedTLS stack
 * for establishing a TLS connection. Task priority of MQTT agent is set to a priority
 * higher than other MQTT application tasks, so that the agent can drain the queue
 * as work is being produced.
 */
#ifndef appmainMQTT_AGENT_TASK_STACK_SIZE
#define appmainMQTT_AGENT_TASK_STACK_SIZE (2048)
#endif
#define appmainMQTT_AGENT_TASK_PRIORITY   (tskIDLE_PRIORITY + 2)

#define INIT_TASK_STACK_SIZE (1024)
#define INIT_TASK_PRIORITY   (tskIDLE_PRIORITY + 1)

#define WIFI_NETWORK_LABEL "aws_wifi"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern void vShadowDeviceTask(void *pvParameters);
extern void vShadowUpdateTask(void *pvParameters);
static void LinkStatusChangeCallback(bool linkState);
int init_network(void);
int app_main(void);
void init_task(void *pvParameters);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static const mflash_file_t dir_template[] = {{.path = KVSTORE_FILE_PATH, .max_size = 3000},
                                             {.path = pkcs11palFILE_NAME_CLIENT_CERTIFICATE, .max_size = 2000},
                                             {.path = pkcs11palFILE_NAME_KEY, .max_size = 2000},
                                             {.path = pkcs11palFILE_CODE_SIGN_PUBLIC_KEY, .max_size = 2000},
                                             {0}};

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Application entry point.
 */
int main(void)
{
    BOARD_InitHardware();

#if defined(PSA_CRYPTO_DRIVER_ELS_PKC)
    config_mbedtls_threading_alt();
    if (psa_crypto_init( ) != PSA_SUCCESS)
#else
    if (CRYPTO_InitHardware() != 0)
#endif
    {
        PRINTF("\r\nFailed to initialize MBEDTLS crypto.\r\n");
        while (true)
        {
        }
    }

    BaseType_t xResult;
    xResult = xTaskCreate(init_task, "init_task", INIT_TASK_STACK_SIZE, NULL, INIT_TASK_PRIORITY, NULL);
    if (xResult != pdPASS)
    {
        PRINTF("\r\nFailed to create init task.\r\n");
        for (;;)
        {
        }
    }

    vTaskStartScheduler();

    /* Should not reach here. */
    for (;;)
    {
    }
}

void init_task(void *pvParameters)
{
    (void)pvParameters;

    /* Initialize file system. */
    if (mflash_init(dir_template, true) != kStatus_Success)
    {
        PRINTF("\r\nFailed to initialize file system.\r\n");
        for (;;)
        {
        }
    }

    /* A simple example to demonstrate key and certificate provisioning in
     * microcontroller flash using PKCS#11 interface. This should be replaced
     * by production ready key provisioning mechanism. */
    CK_RV ret_prov = vDevModeKeyProvisioning();
    if (ret_prov != CKR_OK)
    {
        PRINTF("\r\nDevice provisioning failed: %d\r\n", ret_prov);
        for (;;)
        {
        }
    }

    /* Initialize network. */
    if (init_network() != kStatus_Success)
    {
        while (true)
        {
        }
    }

    BaseType_t xResult = pdFAIL;

    xResult = KVStore_init();

    if (xResult == pdFAIL)
    {
        vLoggingPrintf("Failed to initialize key value configuration store.\r\n");
    }

    if (xResult == pdPASS)
    {
        xResult = xMQTTAgentInit(appmainMQTT_AGENT_TASK_STACK_SIZE, appmainMQTT_AGENT_TASK_PRIORITY);
    }

    if (xResult == pdPASS)
    {
        xResult = xTaskCreate(vShadowDeviceTask, "SHADOW_DEV", appmainSHADOW_DEVICE_TASK_STACK_SIZE, NULL,
                              appmainSHADOW_DEVICE_TASK_PRIORITY, NULL);
    }

    if (xResult == pdPASS)
    {
        xResult = xTaskCreate(vShadowUpdateTask, "SHADOW_APP", appmainSHADOW_UPDATE_TASK_STACK_SIZE, NULL,
                              appmainSHADOW_UPDATE_TASK_PRIORITY, NULL);
    }

    if (xResult != pdPASS)
    {
        PRINTF("\r\nApp main initialization failed.\r\n");

        for (;;)
        {
        }
    }

    vTaskDelete(NULL);
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
    PRINTF("ERROR: stack overflow on task %s.\r\n", pcTaskName);

    portDISABLE_INTERRUPTS();

    /* Unused Parameters */
    (void)xTask;
    (void)pcTaskName;

    /* Loop forever */
    for (;;)
    {
    }
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
    PRINTF("ERROR: Malloc failed to allocate memory\r\n");
    taskDISABLE_INTERRUPTS();

    /* Loop forever */
    for (;;)
    {
    }
}

/*-----------------------------------------------------------*/

/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
 * implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
 * used by the Idle task. */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   configSTACK_DEPTH_TYPE *pulIdleTaskStackSize)
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
/*-----------------------------------------------------------*/

/**
 * @brief This is to provide the memory that is used by the RTOS daemon/time task.
 *
 * If configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
 * implementation of vApplicationGetTimerTaskMemory() to provide the memory that is
 * used by the RTOS daemon/time task.
 */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    configSTACK_DEPTH_TYPE *pulTimerTaskStackSize)
{
    /* If the buffers to be provided to the Timer task are declared inside this
     * function then they must be declared static - otherwise they will be allocated on
     * the stack and so not exists after this function exits. */
    static StaticTask_t xTimerTaskTCB;
    static StackType_t uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle
     * task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
/*-----------------------------------------------------------*/

/* Link lost callback */
static void LinkStatusChangeCallback(bool linkState)
{
    if (linkState == false)
    {
        /* -------- LINK LOST -------- */
        /* DO SOMETHING */
        PRINTF("-------- LINK LOST --------\r\n");
    }
    else
    {
        /* -------- LINK REESTABLISHED -------- */
        /* DO SOMETHING */
        PRINTF("-------- LINK REESTABLISHED --------\r\n");
    }
}

int init_network(void)
{
    uint32_t result;

    /* Initialize Wi-Fi */
    vLoggingPrintf("Initializing Wi-Fi...\r\n");

    result = WPL_Init();
    if (result != WPLRET_SUCCESS)
    {
        PRINTF("WPL_Init failed: %d\r\n", (uint32_t)result);
        return kStatus_Fail;
    }

    result = WPL_Start(LinkStatusChangeCallback);
    if (result != WPLRET_SUCCESS)
    {
        PRINTF("WPL_Start failed: %d\r\n", (uint32_t)result);
        return kStatus_Fail;
    }

    vLoggingPrintf("Wi-Fi initialized successfully.\r\n");

    /* Add Wi-Fi network */
    result = WPL_AddNetwork(clientcredentialWIFI_SSID, clientcredentialWIFI_PASSWORD, WIFI_NETWORK_LABEL);
    if (result != WPLRET_SUCCESS)
    {
        PRINTF("WPL_AddNetwork failed: %d\r\n", (uint32_t)result);
        return kStatus_Fail;
    }

    vLoggingPrintf("Connecting to: %s\r\n", clientcredentialWIFI_SSID);
    result = WPL_Join(WIFI_NETWORK_LABEL);
    if (result != WPLRET_SUCCESS)
    {
        PRINTF("WPL_Join failed: %d\r\n", result);
        return kStatus_Fail;
    }
    vLoggingPrintf("Wi-Fi connected\r\n");

    /* Get IP Address */
    char ip[16] = {0};
    result      = WPL_GetIP(ip, 1);
    if (result != WPLRET_SUCCESS)
    {
        PRINTF("WPL_GetIP failed: %d\r\n", result);
        return kStatus_Fail;
    }
    vLoggingPrintf("IP Address acquired: %s\r\n", ip);

    return kStatus_Success;
}
