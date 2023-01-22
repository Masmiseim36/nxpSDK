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

///////////////////////////////////////////////////////////////////////////////
//  Includes
///////////////////////////////////////////////////////////////////////////////
/* SDK Included Files */
#include "fsl_debug_console.h"
#include "ksdk_mbedtls.h"

#include "pin_mux.h"
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

#include "fsl_silicon_id.h"
#include "fsl_phy.h"
/* lwIP Includes */
#include "lwip/tcpip.h"
#include "lwip/dhcp.h"
#include "lwip/prot/dhcp.h"
#include "netif/ethernet.h"
#include "ethernetif.h"
#include "lwip/netifapi.h"
#include "fsl_phyksz8081.h"
#include "fsl_iomuxc.h"
#include "fsl_enet.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifndef EXAMPLE_NETIF_INIT_FN
/*! @brief Network interface initialization function. */
#define EXAMPLE_NETIF_INIT_FN ethernetif0_init
#endif /* EXAMPLE_NETIF_INIT_FN */

#define INIT_SUCCESS 0
#define INIT_FAIL    1

/* @TEST_ANCHOR */
extern phy_ksz8081_resource_t g_phy_resource;
#define EXAMPLE_ENET         ENET
#define EXAMPLE_PHY_ADDRESS  0x02U
#define EXAMPLE_PHY_OPS      &phyksz8081_ops
#define EXAMPLE_PHY_RESOURCE &g_phy_resource
#define EXAMPLE_CLOCK_FREQ   CLOCK_GetFreq(kCLOCK_IpgClk)

/* @TEST_ANCHOR */

#define LOGGING_TASK_PRIORITY   (tskIDLE_PRIORITY + 1)
#define LOGGING_TASK_STACK_SIZE (200)
#define LOGGING_QUEUE_LENGTH    (16)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_InitNetwork(void);
/* Declaration of demo function. */
int RunOtaCoreMqttDemo(bool awsIotMqttMode,
                       const char *pIdentifier,
                       void *pNetworkServerInfo,
                       void *pNetworkCredentialInfo,
                       const IotNetworkInterface_t *pNetworkInterface);

extern int initNetwork(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static phy_handle_t phyHandle;

struct netif netif;
phy_ksz8081_resource_t g_phy_resource;

/*******************************************************************************
 * Code
 ******************************************************************************/
int initNetwork(void)
{
    ethernetif_config_t enet_config = {
        .phyHandle   = &phyHandle,
        .phyAddr     = EXAMPLE_PHY_ADDRESS,
        .phyOps      = EXAMPLE_PHY_OPS,
        .phyResource = EXAMPLE_PHY_RESOURCE,
        .srcClockHz  = EXAMPLE_CLOCK_FREQ,
    };

    /* Set MAC address. */
#ifdef configMAC_ADDR
    enet_config.macAddress = configMAC_ADDR,
#else
    (void)SILICONID_ConvertToMacAddr(&enet_config.macAddress);
#endif

    tcpip_init(NULL, NULL);

    err_t ret;
    ret = netifapi_netif_add(&netif, NULL, NULL, NULL, &enet_config, EXAMPLE_NETIF_INIT_FN, tcpip_input);
    if (ret != (err_t)ERR_OK)
    {
        (void)PRINTF("netifapi_netif_add: %d\r\n", ret);
        while (true)
        {
        }
    }
    ret = netifapi_netif_set_default(&netif);
    if (ret != (err_t)ERR_OK)
    {
        (void)PRINTF("netifapi_netif_set_default: %d\r\n", ret);
        while (true)
        {
        }
    }
    ret = netifapi_netif_set_up(&netif);
    if (ret != (err_t)ERR_OK)
    {
        (void)PRINTF("netifapi_netif_set_up: %d\r\n", ret);
        while (true)
        {
        }
    }

    while (ethernetif_wait_linkup(&netif, 5000) != ERR_OK)
    {
        (void)PRINTF("PHY Auto-negotiation failed. Please check the cable connection and link partner setting.\r\n");
    }

    configPRINTF(("Getting IP address from DHCP ...\r\n"));
    ret = netifapi_dhcp_start(&netif);
    if (ret != (err_t)ERR_OK)
    {
        (void)PRINTF("netifapi_dhcp_start: %d\r\n", ret);
        while (true)
        {
        }
    }
    (void)ethernetif_wait_ipv4_valid(&netif, ETHERNETIF_WAIT_FOREVER);
    configPRINTF(("IPv4 Address: %s\r\n", ipaddr_ntoa(&netif.ip_addr)));
    configPRINTF(("DHCP OK\r\n"));

    return INIT_SUCCESS;
}
void BOARD_InitModuleClock(void)
{
    const clock_enet_pll_config_t config = {.enableClkOutput = true, .enableClkOutput25M = false, .loopDivider = 1};
    CLOCK_InitEnetPll(&config);
}

static void MDIO_Init(void)
{
    (void)CLOCK_EnableClock(s_enetClock[ENET_GetInstance(EXAMPLE_ENET)]);
    ENET_SetSMI(EXAMPLE_ENET, EXAMPLE_CLOCK_FREQ, false);
}

static status_t MDIO_Write(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return ENET_MDIOWrite(EXAMPLE_ENET, phyAddr, regAddr, data);
}

static status_t MDIO_Read(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return ENET_MDIORead(EXAMPLE_ENET, phyAddr, regAddr, pData);
}


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
        if (initNetwork() != 0)
        {
            configPRINTF(("Network init failed, stopping demo.\r\n"));
            vTaskDelete(NULL);
        }
        else
        {
            static demoContext_t mqttDemoContext = {.networkTypes                = AWSIOT_NETWORK_TYPE_ETH,
                                                    .demoFunction                = RunOtaCoreMqttDemo,
                                                    .networkConnectedCallback    = NULL,
                                                    .networkDisconnectedCallback = NULL};

            Iot_CreateDetachedThread(runDemoTask, &mqttDemoContext, (tskIDLE_PRIORITY + 1),
                                     (configMINIMAL_STACK_SIZE * 10));
        }
    }
}

int main(void)
{
    gpio_pin_config_t gpio_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};

    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    BOARD_InitModuleClock();

    IOMUXC_EnableMode(IOMUXC_GPR, kIOMUXC_GPR_ENET1TxClkOutputDir, true);

    GPIO_PinInit(GPIO1, 9, &gpio_config);
    GPIO_PinInit(GPIO1, 10, &gpio_config);
    /* Pull up the ENET_INT before RESET. */
    GPIO_WritePinOutput(GPIO1, 10, 1);
    GPIO_WritePinOutput(GPIO1, 9, 0);
    SDK_DelayAtLeastUs(10000, CLOCK_GetFreq(kCLOCK_CpuClk));
    GPIO_WritePinOutput(GPIO1, 9, 1);

    MDIO_Init();
    g_phy_resource.read  = MDIO_Read;
    g_phy_resource.write = MDIO_Write;
    CRYPTO_InitHardware();

    xLoggingTaskInitialize(LOGGING_TASK_STACK_SIZE, LOGGING_TASK_PRIORITY, LOGGING_QUEUE_LENGTH);

    vTaskStartScheduler();
    for (;;)
        ;
}
