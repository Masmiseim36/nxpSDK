// Copyright (c) Qualcomm Atheros, Inc.
// All rights reserved.
// Redistribution and use in source and binary forms, with or without modification, are permitted (subject to
// the limitations in the disclaimer below) provided that the following conditions are met:
//
// - Redistributions of source code must retain the above copyright notice, this list of conditions and the
//   following disclaimer.
// - Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
//   following disclaimer in the documentation and/or other materials provided with the distribution.
// - Neither the name of nor the names of its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
//
// NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE. THIS SOFTWARE IS
// PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
// BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//==============================================================================
// Author(s): ="Atheros"
//==============================================================================

#include "main.h" // Only for DEMOCFG_ENABLE_SERIAL_SHELL
#include "wlan_qcom.h"
#include "qcom_api.h"

#include "fsl_shell.h"
#include "board.h"
#include "worker_shell.h"

#include "fsl_device_registers.h"
#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

// Priorities go from 0 to 15 with 0 being the highest priority
#define TASK_WMI_STACK_SIZE 3000U
portSTACK_TYPE *task_wmi1_stack = NULL;
portSTACK_TYPE *task_wmi2_stack = NULL;
#define TASK_WMI_PRIO (configMAX_PRIORITIES - 3)
TaskHandle_t task_wmi1_handler;
TaskHandle_t task_wmi2_handler;

// task size
#define TASK_SHELL_STACK_SIZE 3000U
portSTACK_TYPE *task_shell_stack = NULL;
#define TASK_SHELL_PRIO (configMAX_PRIORITIES - 4)
TaskHandle_t task_shell_handler;

// ============================================================================
// Prototypes
// ============================================================================

extern void wmiconfig_Task1(void *param);
extern void wmiconfig_Task2(void *param);
extern int32_t get_version(void);

extern serial_handle_t g_serialHandle;

SDK_ALIGN(static uint8_t s_shellHandleBuffer[SHELL_HANDLE_SIZE], 4);
static shell_handle_t context = &s_shellHandleBuffer[0];

// ============================================================================
// Task definitions
// ============================================================================

// task define
// OSA_TASK_DEFINE(task_wmi1,  TASK_WMICONFIG_STACK_SIZE);
// OSA_TASK_DEFINE(task_wmi2,  TASK_WMICONFIG_STACK_SIZE);
// OSA_TASK_DEFINE(task_shell, TASK_SHELL_STACK_SIZE);

// ============================================================================
// OS State Information Command
// ============================================================================

int cmd_osinfo(shell_handle_t context, int argc, char **argv)
{
    extern TaskHandle_t atheros_wifi_task_id;
    extern int numIrqs;
    (void)atheros_wifi_task_id;

    PRINTF("wifiIrqs=%d" NL, numIrqs);
#if (INCLUDE_uxTaskGetStackHighWaterMark == 1)
    PRINTF("shellTask watermark=%4d" NL, uxTaskGetStackHighWaterMark(NULL) * sizeof(StackType_t));
    PRINTF("drvTask   watermark=%4d" NL, uxTaskGetStackHighWaterMark(atheros_wifi_task_id) * sizeof(StackType_t));
    PRINTF("wmi1Task  watermark=%4d" NL, uxTaskGetStackHighWaterMark(task_wmi1_task_handler) * sizeof(StackType_t));
    PRINTF("wmi2Task  watermark=%4d" NL, uxTaskGetStackHighWaterMark(task_wmi2_task_handler) * sizeof(StackType_t));
#endif

    return 0;
}

// ============================================================================
// Shell
// ============================================================================

extern _qca_handle handle;
static int startWmiTasks(void)
{
    BaseType_t result;

    atheros_driver_setup();

    // The handle variable is used in various places, e.g. for t_select()
    // I wonder if this is always the same as p_Driver_Cxt[0] (from the driver) or
    // if it sometimes corresponds to device 1, e.g. p_Driver_Cxt[1]
    // If that is the case, we could search and replace all occurences of 'handle'
    handle = p_Driver_Cxt[0];

    result =
        xTaskCreate(wmiconfig_Task1, "wmi1", TASK_WMI_STACK_SIZE, task_wmi1_stack, TASK_WMI_PRIO, &task_wmi1_handler);
    if (result != pdPASS)
    {
        PRINTF("Failed to create wmi1 task" NL NL);
        return -1;
    }

    result =
        xTaskCreate(wmiconfig_Task2, "wmi2", TASK_WMI_STACK_SIZE, task_wmi2_stack, TASK_WMI_PRIO, &task_wmi2_handler);
    if (result != pdPASS)
    {
        PRINTF("Failed to create wmi2 task" NL NL);
        return -1;
    }
    return 0;
}

void SHELL_SendDataCallback(uint8_t *buf, uint32_t len)
{
    while (len--)
    {
        PUTCHAR(*(buf++));
    }
}

void SHELL_ReceiveDataCallback(uint8_t *buf, uint32_t len)
{
    while (len--)
    {
        *(buf++) = GETCHAR();
    }
}

QueueHandle_t shell_q;

extern shell_command_t *shell_commands[];

void wmi_shell(void *param)
{
    int32_t result = 0;
    (void)result;

    do
    {
        /* Initialize WIFI shield */
        result = WIFISHIELD_Init();
        assert(!(A_OK != result));
        if (A_OK != result)
            break;

        /* Initialize the WIFI driver (thus starting the driver task) */
        result = wlan_driver_start();
        assert(!(A_OK != result));
        if (A_OK != result)
            break;

        get_version();

        shell_q = xQueueCreate(2, sizeof(worker_shell_msg_t));
        assert(!(shell_q == NULL));
        if (shell_q == NULL)
            break;

        // lower prio ???
        result = startWmiTasks();
        assert(!(A_OK != result));
        if (A_OK != result)
            break;

        // TODO: ?????
        cmd_osinfo(NULL, 0, 0);

        /* Init SHELL */
        SHELL_Init(context, g_serialHandle, "SHELL>> ");

        /* Add new command to commands list */
        for (int i = 0; (NULL != shell_commands[i]) && (NULL != shell_commands[i]->pFuncCallBack); i++)
        {
            SHELL_RegisterCommand(context, shell_commands[i]);
        }

#if !(defined(SHELL_NON_BLOCKING_MODE) && (SHELL_NON_BLOCKING_MODE > 0U))
        SHELL_Task(context);
#endif

    } while (0);
    while (1)
        ;
}

// ============================================================================
// Main
// ============================================================================
int main(void)
{
    BaseType_t result;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    result =
        xTaskCreate(wmi_shell, "shell", TASK_SHELL_STACK_SIZE, task_shell_stack, TASK_SHELL_PRIO, &task_shell_handler);
    assert(!(result != pdPASS));
    if (result != pdPASS)
    {
        PRINTF("Failed to create shell task" NL NL);
        for (;;)
            ;
    }

    vTaskStartScheduler();

    for (;;)
        ;
}
