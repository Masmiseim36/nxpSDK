/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// ----------------------------------------------------------------------------
// Copyright 2016-2020 ARM Ltd.
//
// SPDX-License-Identifier: Apache-2.0
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ----------------------------------------------------------------------------

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "ksdk_mbedtls.h"

#include "FreeRTOS.h"

/* lwIP Includes */
#include "lwip/tcpip.h"
#include "lwip/dhcp.h"
#include "lwip/prot/dhcp.h"
#include "netif/ethernet.h"
#include "enet_ethernetif.h"
#include "lwip/netifapi.h"

#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include "fsl_phyksz8081.h"
#include "fsl_enet_mdio.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* MAC address configuration. */
#define configMAC_ADDR                     \
    {                                      \
        0x02, 0x12, 0x13, 0x10, 0x15, 0x25 \
    }

/* Address of PHY interface. */
#define EXAMPLE_PHY_ADDRESS BOARD_ENET0_PHY_ADDRESS

/* PHY operations. */
#define EXAMPLE_PHY_OPS phyksz8081_ops

/* MDIO operations. */
#define EXAMPLE_MDIO_OPS enet_ops

/* ENET clock frequency. */
#define EXAMPLE_CLOCK_FREQ CLOCK_GetFreq(kCLOCK_IpgClk)
#ifndef EXAMPLE_NETIF_INIT_FN
/*! @brief Network interface initialization function. */
#define EXAMPLE_NETIF_INIT_FN ethernetif0_init
#endif /* EXAMPLE_NETIF_INIT_FN */

#define INIT_SUCCESS 0
#define INIT_FAIL 1

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_InitNetwork(void);
extern int mbed_cloud_application_entrypoint(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
struct netif network_interface;

static mdio_handle_t mdioHandle = {.ops = &EXAMPLE_MDIO_OPS};
static phy_handle_t phyHandle   = {.phyAddr = EXAMPLE_PHY_ADDRESS, .mdioHandle = &mdioHandle, .ops = &EXAMPLE_PHY_OPS};

#if defined(FSL_FEATURE_SOC_LPC_ENET_COUNT) && (FSL_FEATURE_SOC_LPC_ENET_COUNT > 0)
mem_range_t non_dma_memory[] = NON_DMA_MEMORY_ARRAY;
#endif /* FSL_FEATURE_SOC_LPC_ENET_COUNT */

/*******************************************************************************
 * Code
 ******************************************************************************/
void BOARD_InitModuleClock(void)
{
    const clock_enet_pll_config_t config = {.enableClkOutput = true, .enableClkOutput25M = false, .loopDivider = 1};
    CLOCK_InitEnetPll(&config);
}

void delay(void)
{
    volatile uint32_t i = 0;
    for (i = 0; i < 1000000; ++i)
    {
        __asm("NOP"); /* delay */
    }
}

int initNetwork(void)
{
    ip4_addr_t netif_ipaddr, netif_netmask, netif_gw;
    ethernetif_config_t enet_config = {
        .phyHandle  = &phyHandle,
        .macAddress = configMAC_ADDR,
#if defined(FSL_FEATURE_SOC_LPC_ENET_COUNT) && (FSL_FEATURE_SOC_LPC_ENET_COUNT > 0)
        .non_dma_memory = non_dma_memory,
#endif /* FSL_FEATURE_SOC_LPC_ENET_COUNT */
    };

    mdioHandle.resource.csrClock_Hz = EXAMPLE_CLOCK_FREQ;

    IP4_ADDR(&netif_ipaddr, 0, 0, 0, 0);
    IP4_ADDR(&netif_netmask, 0, 0, 0, 0);
    IP4_ADDR(&netif_gw, 0, 0, 0, 0);

    tcpip_init(NULL, NULL);

    netifapi_netif_add(&network_interface, &netif_ipaddr, &netif_netmask, &netif_gw, &enet_config, EXAMPLE_NETIF_INIT_FN,
                       tcpip_input);
    netifapi_netif_set_default(&network_interface);
    netifapi_netif_set_up(&network_interface);

    PRINTF(("Getting IP address from DHCP ...\r\n"));
    netifapi_dhcp_start(&network_interface);

    struct dhcp *dhcp;
    dhcp = (struct dhcp *)netif_get_client_data(&network_interface, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);

    while (dhcp->state != DHCP_STATE_BOUND)
    {

        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    if (dhcp->state == DHCP_STATE_BOUND)
    {
        PRINTF("IPv4 Address: %u.%u.%u.%u\r\n", ((u8_t *)&network_interface.ip_addr.u_addr.ip4)[0],
                      ((u8_t *)&network_interface.ip_addr.u_addr.ip4)[1],
                      ((u8_t *)&network_interface.ip_addr.u_addr.ip4)[2],
                      ((u8_t *)&network_interface.ip_addr.u_addr.ip4)[3]);
    }
    PRINTF("DHCP OK\r\n");

    return INIT_SUCCESS;
}

void mainTask(void* pArgument)
{
    PRINTF("mainTask\r\n");

    mbed_cloud_application_entrypoint();
}

void vApplicationDaemonTaskStartupHook(void)
{
    if (initNetwork() != 0)
    {
        PRINTF(("Network init failed, stopping demo.\r\n"));
        vTaskDelete(NULL);
    }
    else
    {
        PRINTF(("Startup complete\r\n"));

        xTaskCreate(mainTask, "mainTask", 1024 * 4, NULL, tskIDLE_PRIORITY + 1, NULL);
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
    /* pull up the ENET_INT before RESET. */
    GPIO_WritePinOutput(GPIO1, 10, 1);
    GPIO_WritePinOutput(GPIO1, 9, 0);
    delay();
    GPIO_WritePinOutput(GPIO1, 9, 1);

    SCB_DisableDCache();

    CRYPTO_InitHardware();

    vTaskStartScheduler();
    for (;;);
}

void vApplicationMallocFailedHook(void)
{
    PRINTF("ERROR: Malloc failed to allocate memory\r\n");
    taskDISABLE_INTERRUPTS();

    /* Loop forever */
    for( ; ; );
}

void vApplicationStackOverflowHook(TaskHandle_t xTask,
                                   char* pcTaskName )
{
    PRINTF("ERROR: stack overflow with task %s\r\n", pcTaskName);
    portDISABLE_INTERRUPTS();

    /* Unused Parameters */
    ( void ) xTask;

    /* Loop forever */
    for( ; ; );
}
