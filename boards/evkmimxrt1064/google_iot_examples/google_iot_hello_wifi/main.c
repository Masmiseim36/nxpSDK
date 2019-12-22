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

#include "lwip/tcpip.h"
#include "wwd.h"
#include "wwd_wiced.h"
#include "wwd_network.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_common.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define INIT_SUCCESS 0
#define INIT_FAIL -1

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern void test_join(void);
extern wiced_result_t wiced_wlan_connectivity_init(void);
extern void add_wlan_interface(void);
extern int initNetwork(void);
void createTasks(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
iotc_crypto_key_data_t iotc_connect_private_key_data;
char ec_private_key_pem[PRIVATE_KEY_BUFFER_SIZE] = keyCLIENT_PRIVATE_KEY_PEM;

/*******************************************************************************
 * Code
 ******************************************************************************/
int initNetwork()
{
    wiced_result_t wiced_result = WICED_SUCCESS;
    wwd_result_t wwd_result     = WWD_SUCCESS;

    wiced_ssid_t ap_ssid = {
        .length = sizeof(WIFI_SSID) - 1,
        .value  = WIFI_SSID,
    };

    PRINTF("Initializing WiFi connection... \r\n");

    tcpip_init(NULL, NULL);

    wiced_result = wiced_wlan_connectivity_init();
    if (wiced_result != WICED_SUCCESS)
    {
        PRINTF("Could not initialize WiFi module %d\r\n", (uint32_t)wiced_result);
        return INIT_FAIL;
    }
    else
    {
        PRINTF("Successfully initialized WiFi module\r\n");
    }

    PRINTF("Joining: " WIFI_SSID "\r\n");
    (void)host_rtos_delay_milliseconds((uint32_t)1000);
    wwd_result = wwd_wifi_join(&ap_ssid, WIFI_SECURITY, (uint8_t *)WIFI_PASSWORD, sizeof(WIFI_PASSWORD) - 1, NULL,
                               WWD_STA_INTERFACE);
    if (wwd_result != WWD_SUCCESS)
    {
        PRINTF("Failed to join: " WIFI_SSID "\r\n");
        return INIT_FAIL;
    }
    else
    {
        PRINTF("Successfully joined: " WIFI_SSID "\r\n");
        (void)host_rtos_delay_milliseconds((uint32_t)1000);
        add_wlan_interface();
    }

    return INIT_SUCCESS;
}
static void BOARD_USDHCClockConfiguration(void)
{
    CLOCK_InitSysPll(&sysPllConfig_BOARD_BootClockRUN);
    /*configure system pll PFD2 fractional divider to 24*/
    CLOCK_InitSysPfd(kCLOCK_Pfd2, 24U);
    /* Configure USDHC clock source and divider */
    CLOCK_SetDiv(kCLOCK_Usdhc1Div, 0U);
    CLOCK_SetMux(kCLOCK_Usdhc1Mux, 0U);
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
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_USDHCClockConfiguration();
    BOARD_InitDebugConsole();

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
