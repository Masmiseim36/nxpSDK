/*
 * Copyright 2020 - 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* Board includes */
#include "board.h"
#include "cmd.h"
#include "fsl_sd.h"
#include "ff.h"
#include "diskio.h"
#include "sdmmc_config.h"

#include "usb_host_config.h"
#include "usb_host.h"
#include "usb_host_fatfs.h"
#include "fsl_debug_console.h"

#include "ksdk_mbedtls.h"
#include "pin_mux.h"

#include "FreeRTOS.h"
#include "task.h"

#include "ksdk_mbedtls.h"
#include "mflash_file.h"
#include "kvstore.h"

#include "mqtt_agent_task.h"
#include "aws_dev_mode_key_provisioning.h"
#include "core_pkcs11_config.h"

#include "aws_clientcredential.h"
#include "wpl.h"
#include "app_music_control.h"
#include "app_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define LOGGING_TASK_PRIORITY   (tskIDLE_PRIORITY + 1)
#define LOGGING_TASK_STACK_SIZE (2 * 1024)
#define LOGGING_QUEUE_LENGTH    (16)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

void BOARD_InitHardware(void);
int BOARD_CODEC_Init(void);
extern void vStartMusicPlayerDemoTask(void);
extern int app_main (int argc, char **argv);

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

/**
 * @brief Stack size and priority for shadow device sync task.
 */
#define appmainSHADOW_DEVICE_TASK_STACK_SIZE (2048)
#define appmainSHADOW_DEVICE_TASK_PRIORITY   (tskIDLE_PRIORITY + 1)

/**
 * @brief Stack size and priority for shadow update application task.
 */
#define appmainSHADOW_UPDATE_TASK_STACK_SIZE (2048)
#define appmainSHADOW_UPDATE_TASK_PRIORITY   (tskIDLE_PRIORITY + 1)

/**
 * @brief Stack size and priority for MQTT agent task.
 * Stack size is capped to an adequate value based on requirements from MbedTLS stack
 * for establishing a TLS connection. Task priority of MQTT agent is set to a priority
 * higher than other MQTT application tasks, so that the agent can drain the queue
 * as work is being produced.
 */
#define appmainMQTT_AGENT_TASK_STACK_SIZE (2048) // 6144
#define appmainMQTT_AGENT_TASK_PRIORITY   (tskIDLE_PRIORITY + 2)

#define WIFI_NETWORK_LABEL "aws_wifi"

extern void vShadowDeviceTask(void *pvParameters);
extern void vShadowUpdateTask(void *pvParameters);
int init_network(void);

void main_task(void *pvParameters)
{
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
    vDevModeKeyProvisioning();

    /* Initialize network. */
    if (init_network() != kStatus_Success)
    {
        PRINTF("\r\nInitialization of network failed.\r\n");
        while (true)
        {
        }
    }

    BaseType_t xResult = pdFAIL;

    xResult = KVStore_init();

    if (xResult == pdFAIL)
    {
        configPRINTF(("Failed to initialize key value configuration store.\r\n"));
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

    app_main(0 , NULL);
    vTaskDelete(NULL);
}

#if defined(__CC_ARM) || (defined(__ARMCC_VERSION)) || defined(__GNUC__)
int main(void)
#else
void main(void)
#endif
{
    BOARD_InitHardware();
    CRYPTO_InitHardware();
    USB_HostApplicationInit();

    app_bt_init_task();
    if (xTaskCreate(main_task, "main_task", 1024L, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS)
    {
        PRINTF("Main task creation failed!.\r\n");
        while (1)
            ;
    }

    if (xTaskCreate(USB_HostTask, "usb host task", 2000L / sizeof(portSTACK_TYPE), g_HostHandle, tskIDLE_PRIORITY + 5, NULL) != pdPASS)
    {
        usb_echo("create host task error\r\n");
        while (1)
            ;
    }

    /* audio control function */
    music_start_control_task();

    /* Run RTOS */
    vTaskStartScheduler();

    while (1)
    {
        ;
    }
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
    configPRINTF(("Initializing Wi-Fi...\r\n"));

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

    configPRINTF(("Wi-Fi initialized successfully.\r\n"));

    /* Add Wi-Fi network */
    result = WPL_AddNetwork(clientcredentialWIFI_SSID, clientcredentialWIFI_PASSWORD, WIFI_NETWORK_LABEL);
    if (result != WPLRET_SUCCESS)
    {
        PRINTF("WPL_AddNetwork failed: %d\r\n", (uint32_t)result);
        return kStatus_Fail;
    }

    configPRINTF(("Connecting to: %s\r\n", clientcredentialWIFI_SSID));
    result = WPL_Join(WIFI_NETWORK_LABEL);
    if (result != WPLRET_SUCCESS)
    {
        PRINTF("WPL_Join failed: %d\r\n", result);
        return kStatus_Fail;
    }
    configPRINTF(("Wi-Fi connected\r\n"));

    /* Get IP Address */
    char ip[16] = {0};
    result      = WPL_GetIP(ip, 1);
    if (result != WPLRET_SUCCESS)
    {
        PRINTF("WPL_GetIP failed: %d\r\n", result);
        return kStatus_Fail;
    }
    configPRINTF(("IP Address acquired: %s\r\n", ip));

    return kStatus_Success;
}

#ifndef __GNUC__
void __assert_func(const char *file, int line, const char *func, const char *failedExpr)
{
    PRINTF("ASSERT ERROR \" %s \": file \"%s\" Line \"%d\" function name \"%s\" \n", failedExpr, file, line, func);
    for (;;)
    {
        __BKPT(0);
    }
}
#endif