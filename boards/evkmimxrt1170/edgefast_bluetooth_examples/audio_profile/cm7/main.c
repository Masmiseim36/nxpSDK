/*
 * Copyright 2020 - 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* Board includes */
#include "pin_mux.h"
#include "clock_config.h"
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

#include "fsl_lpuart_edma.h"
#include "fsl_dmamux.h"
#include "usb_host_msd.h"
#include "usb_phy.h"
#include "fsl_adapter_uart.h"
#include "controller_hci_uart.h"
#if (((defined(CONFIG_BT_SMP)) && (CONFIG_BT_SMP)))
#endif /* CONFIG_BT_SMP */
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#if defined(__GIC_PRIO_BITS)
#define USB_HOST_INTERRUPT_PRIORITY (25U)
#elif defined(__NVIC_PRIO_BITS) && (__NVIC_PRIO_BITS >= 3)
#define USB_HOST_INTERRUPT_PRIORITY (6U)
#else
#define USB_HOST_INTERRUPT_PRIORITY (3U)
#endif


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
extern usb_host_handle g_HostHandle;


static const mflash_file_t dir_template[] = {{.path = KVSTORE_FILE_PATH, .max_size = 3000},
                                             {.path = pkcs11palFILE_NAME_CLIENT_CERTIFICATE, .max_size = 2000},
                                             {.path = pkcs11palFILE_NAME_KEY, .max_size = 2000},
                                             {.path = pkcs11palFILE_CODE_SIGN_PUBLIC_KEY, .max_size = 2000},
                                             {0}};

/*******************************************************************************
 * Code
 ******************************************************************************/

#if (defined(WIFI_88W8987_BOARD_AW_CM358MA) || defined(WIFI_88W8987_BOARD_MURATA_1ZM_M2) || \
     defined(WIFI_IW416_BOARD_MURATA_1XK_M2))
int controller_hci_uart_get_configuration(controller_hci_uart_config_t *config)
{
    if (NULL == config)
    {
        return -1;
    }
    config->clockSrc        = CLOCK_GetRootClockFreq(kCLOCK_Root_Lpuart2);
    config->defaultBaudrate = 115200u;
    config->runningBaudrate = BOARD_BT_UART_BAUDRATE;
    config->instance        = 2;
#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
    config->dma_instance     = 0U;
    config->rx_channel       = 0U;
    config->tx_channel       = 1U;
    config->dma_mux_instance = 0U;
    config->rx_request       = kDmaRequestMuxLPUART2Rx;
    config->tx_request       = kDmaRequestMuxLPUART2Tx;
#endif
    config->enableRxRTS = 1u;
    config->enableTxCTS = 1u;
    return 0;
}
#elif defined(WIFI_IW416_BOARD_AW_AM510MA)
int controller_hci_uart_get_configuration(controller_hci_uart_config_t *config)
{
    if (NULL == config)
    {
        return -1;
    }
    config->clockSrc = CLOCK_GetRootClockFreq(kCLOCK_Root_Lpuart2);
    config->defaultBaudrate = BOARD_BT_UART_BAUDRATE;
    config->runningBaudrate = BOARD_BT_UART_BAUDRATE;
    config->instance = 2;
#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
    config->dma_instance = 0U;
    config->rx_channel = 0U;
    config->tx_channel = 1U;
    config->dma_mux_instance = 0U;
    config->rx_request = kDmaRequestMuxLPUART2Rx;
    config->tx_request = kDmaRequestMuxLPUART2Tx;
#endif
    config->enableRxRTS = 1u;
    config->enableTxCTS = 1u;
    return 0;
}
#elif defined(WIFI_88W8987_BOARD_MURATA_1ZM_USD) || defined(WIFI_IW416_BOARD_MURATA_1XK_USD)
int controller_hci_uart_get_configuration(controller_hci_uart_config_t *config)
{
    if (NULL == config)
    {
        return -1;
    }
    config->clockSrc         = BOARD_BT_UART_CLK_FREQ;
    config->defaultBaudrate  = 115200u;
    config->runningBaudrate  = BOARD_BT_UART_BAUDRATE;
    config->instance         = BOARD_BT_UART_INSTANCE;
    config->enableRxRTS      = 1u;
    config->enableTxCTS      = 1u;
#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
    config->dma_instance     = 0U;
    config->rx_channel       = 0U;
    config->tx_channel       = 1U;
    config->dma_mux_instance = 0U;
    config->rx_request       = kDmaRequestMuxLPUART7Rx;
    config->tx_request       = kDmaRequestMuxLPUART7Tx;
#endif
    return 0;
}
#elif defined(WIFI_IW416_BOARD_AW_AM457_USD)
int controller_hci_uart_get_configuration(controller_hci_uart_config_t *config)
{
    if (NULL == config)
    {
        return -1;
    }
    config->clockSrc         = BOARD_BT_UART_CLK_FREQ;
    config->defaultBaudrate  = BOARD_BT_UART_BAUDRATE;
    config->runningBaudrate  = BOARD_BT_UART_BAUDRATE;
    config->instance         = BOARD_BT_UART_INSTANCE;
#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
    config->dma_instance     = 0U;
    config->rx_channel       = 0U;
    config->tx_channel       = 1U;
    config->dma_mux_instance = 0U;
    config->rx_request       = kDmaRequestMuxLPUART7Rx;
    config->tx_request       = kDmaRequestMuxLPUART7Tx;
#endif
    config->enableRxRTS      = 1u;
    config->enableTxCTS      = 1u;
    return 0;
}
#else
#endif

void USB_OTG1_IRQHandler(void)
{
    USB_HostEhciIsrFunction(g_HostHandle);
    SDK_ISR_EXIT_BARRIER;
}

void USB_OTG2_IRQHandler(void)
{
    USB_HostEhciIsrFunction(g_HostHandle);
    SDK_ISR_EXIT_BARRIER;
}

void USB_HostClockInit(void)
{
    uint32_t usbClockFreq;
    usb_phy_config_struct_t phyConfig = {
        BOARD_USB_PHY_D_CAL,
        BOARD_USB_PHY_TXCAL45DP,
        BOARD_USB_PHY_TXCAL45DM,
    };
    usbClockFreq = 24000000;
    if (CONTROLLER_ID == kUSB_ControllerEhci0)
    {
        CLOCK_EnableUsbhs0PhyPllClock(kCLOCK_Usbphy480M, usbClockFreq);
        CLOCK_EnableUsbhs0Clock(kCLOCK_Usb480M, usbClockFreq);
    }
    else
    {
        CLOCK_EnableUsbhs1PhyPllClock(kCLOCK_Usbphy480M, usbClockFreq);
        CLOCK_EnableUsbhs1Clock(kCLOCK_Usb480M, usbClockFreq);
    }
    USB_EhciPhyInit(CONTROLLER_ID, BOARD_XTAL0_CLK_HZ, &phyConfig);
}

void USB_HostIsrEnable(void)
{
    uint8_t irqNumber;

    uint8_t usbHOSTEhciIrq[] = USBHS_IRQS;
    irqNumber                = usbHOSTEhciIrq[CONTROLLER_ID - kUSB_ControllerEhci0];

/* Install isr, set priority, and enable IRQ. */
#if defined(__GIC_PRIO_BITS)
    GIC_SetPriority((IRQn_Type)irqNumber, USB_HOST_INTERRUPT_PRIORITY);
#else
    NVIC_SetPriority((IRQn_Type)irqNumber, USB_HOST_INTERRUPT_PRIORITY);
#endif
    EnableIRQ((IRQn_Type)irqNumber);
}

void USB_HostTaskFn(void *param)
{
    USB_HostEhciTaskFunction(param);
}

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
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
#if (defined(WIFI_IW416_BOARD_AW_AM510MA) || defined(WIFI_88W8987_BOARD_AW_CM358MA) || \
     defined(WIFI_88W8987_BOARD_MURATA_1ZM_M2) || defined(WIFI_IW416_BOARD_MURATA_1XK_M2))
    BOARD_InitM2UARTPins();
#else
    BOARD_InitArduinoUARTPins();
#endif
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    SCB_DisableDCache();
#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
    DMAMUX_Type *dmaMuxBases[] = DMAMUX_BASE_PTRS;
    edma_config_t config;
    DMA_Type *dmaBases[] = DMA_BASE_PTRS;
    DMAMUX_Init(dmaMuxBases[0]);
    EDMA_GetDefaultConfig(&config);
    EDMA_Init(dmaBases[0], &config);
#endif
#if (((defined(CONFIG_BT_SMP)) && (CONFIG_BT_SMP)))
    CRYPTO_InitHardware();
#endif /* CONFIG_BT_SMP */
    CRYPTO_InitHardware();
    USB_HostApplicationInit();

    app_bt_init_task();
    if (xTaskCreate(main_task, "main_task", 2200L / sizeof(portSTACK_TYPE), NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS)
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

#if 0
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
#endif

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
                                    uint32_t *pulTimerTaskStackSize)
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
