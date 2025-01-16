/*
 * Lab-Project-coreMQTT-Agent 201215
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 * Copyright 2022-2023 NXP
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
#else
#include "ksdk_mbedtls.h"
#endif

#include "mflash_file.h"
#include "kvstore.h"
#include "app.h"

#include "mqtt_agent_task.h"
#include "aws_dev_mode_key_provisioning.h"
#include "core_pkcs11_config.h"

#include "fsl_silicon_id.h"

/* lwIP Includes */
#include "ethernetif.h"
#include "lwip/netifapi.h"

#include "logging.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/**
 * @brief Stack size and priority for shadow device sync task.
 */
#define appmainSHADOW_DEVICE_TASK_STACK_SIZE (384)
#define appmainSHADOW_DEVICE_TASK_PRIORITY   (tskIDLE_PRIORITY + 1)

/**
 * @brief Stack size and priority for shadow update application task.
 */
#define appmainSHADOW_UPDATE_TASK_STACK_SIZE (384)
#define appmainSHADOW_UPDATE_TASK_PRIORITY   (tskIDLE_PRIORITY + 1)

/**
 * @brief Stack size and priority for MQTT agent task.
 * Stack size is capped to an adequate value based on requirements from MbedTLS stack
 * for establishing a TLS connection. Task priority of MQTT agent is set to a priority
 * higher than other MQTT application tasks, so that the agent can drain the queue
 * as work is being produced.
 */
#define appmainMQTT_AGENT_TASK_STACK_SIZE (2048)
#define appmainMQTT_AGENT_TASK_PRIORITY   (tskIDLE_PRIORITY + 2)

#define INIT_TASK_STACK_SIZE (1024)
#define INIT_TASK_PRIORITY   (tskIDLE_PRIORITY + 1)

#ifndef EXAMPLE_NETIF_INIT_FN
/*! @brief Network interface initialization function. */
#define EXAMPLE_NETIF_INIT_FN ethernetif0_init
#endif /* EXAMPLE_NETIF_INIT_FN */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern void vShadowDeviceTask(void *pvParameters);
extern void vShadowUpdateTask(void *pvParameters);
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

static phy_handle_t s_phyHandle;
static struct netif s_netif;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Application entry point.
 */
int main(void)
{
    BOARD_InitHardware();

    if (CRYPTO_InitHardware() != 0)
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

int init_network(void)
{
    ethernetif_config_t enet_config = {
        .phyHandle   = &s_phyHandle,
        .phyAddr     = EXAMPLE_PHY_ADDRESS,
        .phyOps      = EXAMPLE_PHY_OPS,
        .phyResource = EXAMPLE_PHY_RESOURCE,
        .srcClockHz  = EXAMPLE_CLOCK_FREQ,
#ifdef configMAC_ADDR
        .macAddress = configMAC_ADDR
#endif
    };

    /* Set MAC address. */
#ifndef configMAC_ADDR
    (void)SILICONID_ConvertToMacAddr(&enet_config.macAddress);
#endif

    tcpip_init(NULL, NULL);

    err_t ret;
    ret = netifapi_netif_add(&s_netif, NULL, NULL, NULL, &enet_config, EXAMPLE_NETIF_INIT_FN, tcpip_input);
    if (ret != (err_t)ERR_OK)
    {
        (void)PRINTF("netifapi_netif_add: %d\r\n", ret);
        while (true)
        {
        }
    }
    ret = netifapi_netif_set_default(&s_netif);
    if (ret != (err_t)ERR_OK)
    {
        (void)PRINTF("netifapi_netif_set_default: %d\r\n", ret);
        while (true)
        {
        }
    }
    ret = netifapi_netif_set_up(&s_netif);
    if (ret != (err_t)ERR_OK)
    {
        (void)PRINTF("netifapi_netif_set_up: %d\r\n", ret);
        while (true)
        {
        }
    }

    while (ethernetif_wait_linkup(&s_netif, 5000) != ERR_OK)
    {
        (void)PRINTF("PHY Auto-negotiation failed. Please check the cable connection and link partner setting.\r\n");
    }

    vLoggingPrintf("Getting IP address from DHCP ...\r\n");
    ret = netifapi_dhcp_start(&s_netif);
    if (ret != (err_t)ERR_OK)
    {
        (void)PRINTF("netifapi_dhcp_start: %d\r\n", ret);
        while (true)
        {
        }
    }
    (void)ethernetif_wait_ipv4_valid(&s_netif, ETHERNETIF_WAIT_FOREVER);
    vLoggingPrintf("IPv4 Address: %s\r\n", ipaddr_ntoa(&s_netif.ip_addr));
    vLoggingPrintf("DHCP OK\r\n");

    return kStatus_Success;
}
