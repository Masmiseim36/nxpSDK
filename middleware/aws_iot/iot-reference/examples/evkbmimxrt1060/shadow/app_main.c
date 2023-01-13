/*
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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


/**
 * @brief Contains the application main function which spawns different demo tasks
 * included in the project.
 */

#include "FreeRTOS.h"
#include "task.h"
#include "kvstore.h"
#include "mqtt_agent_task.h"

/**
 * @brief Flag which enables OTA update task in background along with other demo tasks.
 * OTA update task polls regularly for firmware update jobs or acts on a new firmware update
 * available notification from OTA service.
 */
#define appmainINCLUDE_OTA_UPDATE_TASK            ( 1 )

/**
 * @brief Flag to enable or disable provisioning mode for the demo.
 * Enabling the flags starts a CLI task, so that user can perform provisioning of the device through
 * a serial terminal. Provisioning involves running commands to fetch or set the PKI and configuration
 * information for the device to connect to broker and perform OTA updates. Disabling the flag results
 * in disabling the CLI task and execution of the demo tasks in normal device operation mode.
 */
#define appmainPROVISIONING_MODE                  ( 1 )

/**
 * @brief Stack size and priority for shadow device sync task.
 */
#define appmainSHADOW_DEVICE_TASK_STACK_SIZE      ( 2048 )
#define appmainSHADOW_DEVICE_TASK_PRIORITY        ( tskIDLE_PRIORITY + 1 )

/**
 * @brief Stack size and priority for shadow update application task.
 */
#define appmainSHADOW_UPDATE_TASK_STACK_SIZE      ( 2048 )
#define appmainSHADOW_UPDATE_TASK_PRIORITY        ( tskIDLE_PRIORITY + 1 )

/**
 * @brief Stack size and priority for OTA Update task.
 */
#define appmainMQTT_OTA_UPDATE_TASK_STACK_SIZE    ( 4096 )
#define appmainMQTT_OTA_UPDATE_TASK_PRIORITY      ( tskIDLE_PRIORITY + 1 )

/**
 * @brief Stack size and priority for MQTT agent task.
 * Stack size is capped to an adequate value based on requirements from MbedTLS stack
 * for establishing a TLS connection. Task priority of MQTT agent is set to a priority
 * higher than other MQTT application tasks, so that the agent can drain the queue
 * as work is being produced.
 */
#define appmainMQTT_AGENT_TASK_STACK_SIZE         ( 6144 )
#define appmainMQTT_AGENT_TASK_PRIORITY           ( tskIDLE_PRIORITY + 2 )

/**
 * @brief Stack size and priority for CLI task.
 */
#define appmainCLI_TASK_STACK_SIZE                ( 6144 )
#define appmainCLI_TASK_PRIORITY                  ( tskIDLE_PRIORITY + 1 )

extern void vShadowDeviceTask( void * pvParameters );

extern void vShadowUpdateTask( void * pvParameters );

extern void vOTAUpdateTask( void * pvParam );

extern void vCLITask( void * pvParam );

int app_main( void )
{
    BaseType_t xResult = pdFAIL;

    xResult = KVStore_init();

    if( xResult == pdFAIL )
    {
        configPRINTF( ( "Failed to initialize key value configuration store.\r\n" ) );
    }

#if ( appmainPROVISIONING_MODE == 1 )
    {
        if( xResult == pdPASS )
        {
            xResult = xTaskCreate( vCLITask,
                                   "CLI",
                                   appmainCLI_TASK_STACK_SIZE,
                                   NULL,
                                   appmainCLI_TASK_PRIORITY,
                                   NULL );
        }
    }
#else /* if ( appmainPROVISIONING_MODE == 1 ) */
    {
        if( xResult == pdPASS )
        {
            xResult = xMQTTAgentInit( appmainMQTT_AGENT_TASK_STACK_SIZE, appmainMQTT_AGENT_TASK_PRIORITY );
        }

#if ( appmainINCLUDE_OTA_UPDATE_TASK == 1 )
        {
            if( xResult == pdPASS )
            {
                xResult = xTaskCreate( vOTAUpdateTask,
                                       "OTA",
                                       appmainMQTT_OTA_UPDATE_TASK_STACK_SIZE,
                                       NULL,
                                       appmainMQTT_OTA_UPDATE_TASK_PRIORITY,
                                       NULL );
            }
        }
#endif /* if ( appmainINCLUDE_OTA_AGENT == 1 ) */

        if( xResult == pdPASS )
        {
            xResult = xTaskCreate( vShadowDeviceTask,
                                   "SHADOW_DEV",
                                   appmainSHADOW_DEVICE_TASK_STACK_SIZE,
                                   NULL,
                                   appmainSHADOW_DEVICE_TASK_PRIORITY,
                                   NULL );
        }

        if( xResult == pdPASS )
        {
            xResult = xTaskCreate( vShadowUpdateTask,
                                   "SHADOW_APP",
                                   appmainSHADOW_UPDATE_TASK_STACK_SIZE,
                                   NULL,
                                   appmainSHADOW_UPDATE_TASK_PRIORITY,
                                   NULL );
        }
    }
#endif /* if ( appmainPROVISIONING_MODE == 1 ) */

    return pdPASS;
}
