/****************************************************************************
 *
 *  Copyright (c) 2017, Michael Becker (michael.f.becker@gmail.com)
 *
 *  This file is part of the FreeRTOS Add-ons project.
 *
 *  Source Code:
 *  https://github.com/michaelbecker/freertos-addons
 *
 *  Project Page:
 *  http://michaelbecker.github.io/freertos-addons/
 *
 *  On-line Documentation:
 *  http://michaelbecker.github.io/freertos-addons/docs/html/index.html
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a
 *  copy of this software and associated documentation files
 *  (the "Software"), to deal in the Software without restriction, including
 *  without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to
 *  permit persons to whom the Software is furnished to do so,subject to the
 *  following conditions:
 *
 *  + The above copyright notice and this permission notice shall be included
 *    in all copies or substantial portions of the Software.
 *  + Credit is appreciated, but not required, if you find this project
 *    useful enough to include in your application, product, device, etc.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 *  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 *  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 *  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 *  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 ***************************************************************************/

/* Copyright 2018-2019 Google LLC
 *
 * This is part of the Google Cloud IoT Device SDK for Embedded C.
 * It is licensed under the BSD 3-Clause license; you may not use this file
 * except in compliance with the License.
 *
 * You may obtain a copy of the License at:
 *  https://opensource.org/licenses/BSD-3-Clause
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "board.h"
#include "fsl_debug_console.h"
#include "ksdk_mbedtls.h"

#include "FreeRTOS.h"
#include "task.h"

#include <iotc.h>
#include <iotc_jwt.h>
#include <iotc_error.h>
#include "commandline.h"
#include "example_utils.h"
#include "google_iot_config.h"

/* lwIP Includes */
#include "lwip/tcpip.h"
#include "lwip/dhcp.h"
#include "lwip/prot/dhcp.h"
#include "netif/ethernet.h"
#include "enet_ethernetif.h"
#include "lwip/netifapi.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifndef EXAMPLE_NETIF_INIT_FN
/*! @brief Network interface initialization function. */
#define EXAMPLE_NETIF_INIT_FN ethernetif0_init
#endif /* EXAMPLE_NETIF_INIT_FN */

#define INIT_SUCCESS 0
#define INIT_FAIL 1

/* MAC address configuration. */
#define configMAC_ADDR                     \
    {                                      \
        0x02, 0x12, 0x13, 0x10, 0x15, 0x25 \
    }

/* Address of PHY interface. */
#define EXAMPLE_PHY_ADDRESS BOARD_ENET0_PHY_ADDRESS

/* System clock name. */
#define EXAMPLE_CLOCK_NAME kCLOCK_CoreSysClk


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_InitNetwork(void);
extern int initNetwork(void);
void createTasks(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
struct netif netif;
#if defined(FSL_FEATURE_SOC_LPC_ENET_COUNT) && (FSL_FEATURE_SOC_LPC_ENET_COUNT > 0)
mem_range_t non_dma_memory[] = NON_DMA_MEMORY_ARRAY;
#endif /* FSL_FEATURE_SOC_LPC_ENET_COUNT */
iotc_crypto_key_data_t iotc_connect_private_key_data;
char ec_private_key_pem[PRIVATE_KEY_BUFFER_SIZE] = keyCLIENT_PRIVATE_KEY_PEM;

/*******************************************************************************
 * Code
 ******************************************************************************/
int initNetwork(void)
{
    ip4_addr_t netif_ipaddr, netif_netmask, netif_gw;
    ethernetif_config_t enet_config = {
        .phyAddress = EXAMPLE_PHY_ADDRESS,
        .clockName  = EXAMPLE_CLOCK_NAME,
        .macAddress = configMAC_ADDR,
#if defined(FSL_FEATURE_SOC_LPC_ENET_COUNT) && (FSL_FEATURE_SOC_LPC_ENET_COUNT > 0)
        .non_dma_memory = non_dma_memory,
#endif /* FSL_FEATURE_SOC_LPC_ENET_COUNT */
    };

    IP4_ADDR(&netif_ipaddr, 0, 0, 0, 0);
    IP4_ADDR(&netif_netmask, 0, 0, 0, 0);
    IP4_ADDR(&netif_gw, 0, 0, 0, 0);

    tcpip_init(NULL, NULL);

    netifapi_netif_add(&netif, &netif_ipaddr, &netif_netmask, &netif_gw, &enet_config, EXAMPLE_NETIF_INIT_FN,
                       tcpip_input);
    netifapi_netif_set_default(&netif);
    netifapi_netif_set_up(&netif);

    PRINTF(("Getting IP address from DHCP ...\r\n"));
    netifapi_dhcp_start(&netif);

    struct dhcp *dhcp;
    dhcp = (struct dhcp *)netif_get_client_data(&netif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);

    while (dhcp->state != DHCP_STATE_BOUND)
    {
        vTaskDelay(1000);
    }

    if (dhcp->state == DHCP_STATE_BOUND)
    {
        PRINTF("IPv4 Address: %u.%u.%u.%u\r\n", ((u8_t *)&netif.ip_addr.addr)[0], ((u8_t *)&netif.ip_addr.addr)[1],
               ((u8_t *)&netif.ip_addr.addr)[2], ((u8_t *)&netif.ip_addr.addr)[3]);
    }
    PRINTF("DHCP OK\r\n");

    return INIT_SUCCESS;
}
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


/**
 * The FreeRTOS task for Embedded C Client. Initializes the client
 * then starts a while loop calling the client's tick function inside.
 */
void task_function_gcpiot_embedded_c(void *parameters)
{
    const size_t task_delay = (size_t)parameters;

    PRINTF("Starting GCP IoT Embedded C Client...\n");

    iotc_project_id      = iotcPROJECT_ID;
    iotc_device_path     = iotcDEVICE_PATH;
    iotc_publish_topic   = iotcPUBLISH_TOPIC;
    iotc_publish_message = iotcPUBLISH_MESSAGE;

    iotc_initialize();

    iotc_context_handle_t context_handle = iotc_create_context();

    /* generate the client authentication JWT, which will serve as the MQTT
     * password */
    char jwt[IOTC_JWT_SIZE] = {0};
    size_t bytes_written    = 0;
    iotc_state_t state      = iotc_create_iotcore_jwt(iotc_project_id,
                                                 /*jwt_expiration_period_sec=*/3600, &iotc_connect_private_key_data,
                                                 jwt, IOTC_JWT_SIZE, &bytes_written);

    if (IOTC_STATE_OK != state)
    {
        PRINTF("iotc_create_iotcore_jwt returned with error: %ul : %s\n", state, iotc_get_state_string(state));
        iotc_shutdown();
        return;
    }

    const uint16_t connection_timeout = 10;
    const uint16_t keepalive_timeout  = 3;

    iotc_connect(context_handle, /*username=*/NULL, /*password=*/jwt,
                 /*client_id=*/iotc_device_path, connection_timeout, keepalive_timeout, &on_connection_state_changed);

    while (1)
    {
        PRINTF(".");

#if 0
    /* Calling the non-blocking tick call processes a single client cycle making
     * the client a bit slower than calling the blocking call. */
    iotc_events_process_tick();
#else
        /* Calling the blocking function processes more client cycles thus makes the
         * client faster. At some point this blocking call will wait in a socket
         * select call. And at some point the FreeRTOS tick interrupt will kick in
         * which makes the select call to exit with errno 4:
         * #define EINTR        4  // Interrupted system call
         * Making this blocking call return.
         */
        iotc_events_process_blocking();
#endif

        vTaskDelay(task_delay);
        (void)task_delay;
    }
}

/**
 * The FreeRTOS task for an examplary custom application action.
 */
void task_function_custom_application_logic(void *parameters)
{
    const size_t task_delay = (size_t)parameters;

    PRINTF("Starting Custom Application Logic Task...\n");

    while (1)
    {
        PRINTF("[ custom application logic ] ");
        PRINTF("[ heap left: %lu bytes ]\n", xPortGetFreeHeapSize());

        vTaskDelay(task_delay);
    }
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
        createTasks();
    }
}

int main(int argc, char *argv[])
{
    gpio_pin_config_t gpio_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};

    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
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

    PRINTF("Google Cloud Platform IoT Hello example start.\n");

    /* Format the key type descriptors so the client understands
       what type of key is being represeted. In this case, a PEM encoded
       byte array of a ES256 key. */
    iotc_connect_private_key_data.crypto_key_signature_algorithm = IOTC_CRYPTO_KEY_SIGNATURE_ALGORITHM_ES256;
    iotc_connect_private_key_data.crypto_key_union_type          = IOTC_CRYPTO_KEY_UNION_TYPE_PEM;
    iotc_connect_private_key_data.crypto_key_union.key_pem.key   = ec_private_key_pem;

    /**
     *  Start FreeRTOS here.
     */
    vTaskStartScheduler();

    /*
     *  We shouldn't ever get here unless someone calls
     *  vTaskEndScheduler(). Note that there appears to be a
     *  bug in the Linux FreeRTOS simulator that crashes when
     *  this is called.
     */
    PRINTF("Scheduler ended!\n");

    return 0;
}

void vAssertCalled(unsigned long ulLine, const char *const pcFileName)
{
    PRINTF("ASSERT: %s : %d\n", pcFileName, (int)ulLine);
    while (1)
        ;
}

unsigned long ulGetRunTimeCounterValue(void)
{
    return 0;
}

void vConfigureTimerForRunTimeStats(void)
{
    return;
}

void vApplicationMallocFailedHook(void)
{
    while (1)
        ;
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

void createTasks(void)
{
    BaseType_t rc;
    const uint16_t stack_depth = 2000;
    /**
     * Create the Embedded C Client task.
     */
    rc = xTaskCreate(task_function_gcpiot_embedded_c, "gcpiot embedded c client", stack_depth,
                     /* task delay */ (void *)100,
                     /* task priority */ 1, /* task handle */ NULL);
    /**
     *  Make sure our task was created.
     */
    configASSERT(rc == pdPASS);

    /**
     * Create an application task for demonstration purposes.
     */
    rc = xTaskCreate(task_function_custom_application_logic, "custom application logic", stack_depth,
                     /* task delay */ (void *)1000, /* task priority */ 2,
                     /* task handle */ NULL);
    /**
     *  Make sure our task was created.
     */
    configASSERT(rc == pdPASS);
}
