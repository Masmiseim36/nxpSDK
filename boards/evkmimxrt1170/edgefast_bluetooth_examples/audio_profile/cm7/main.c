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
#include "iot_logging_task.h"
#include "iot_system_init.h"
#include "aws_dev_mode_key_provisioning.h"
#include "platform/iot_threads.h"
#include "types/iot_network_types.h"
#include "aws_demo.h"
#include "app_music_control.h"
#include "app_common.h"

#include "fsl_lpuart_edma.h"
#include "fsl_dmamux.h"
#include "usb_host_msd.h"
#include "usb_phy.h"
#include "fsl_adapter_uart.h"
#include "controller.h"
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

void print_string(const char *string)
{
    PRINTF(string);
}

void main_task(void *pvParameters)
{
    /* A simple example to demonstrate key and certificate provisioning in
     * microcontroller flash using PKCS#11 interface. This should be replaced
     * by production ready key provisioning mechanism. */
    vDevModeKeyProvisioning();

    if (SYSTEM_Init() == pdPASS)
    {
        vStartMusicPlayerDemoTask();
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
    CRYPTO_InitHardware();
    USB_HostApplicationInit();

    app_bt_init_task();
    if (xTaskCreate(main_task, "main_task", 2000L / sizeof(portSTACK_TYPE), NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS)
    {
        PRINTF("Main task creation failed!.\r\n");
        while (1)
            ;
    }

    xLoggingTaskInitialize(LOGGING_TASK_STACK_SIZE / sizeof(portSTACK_TYPE), LOGGING_TASK_PRIORITY, LOGGING_QUEUE_LENGTH);

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
