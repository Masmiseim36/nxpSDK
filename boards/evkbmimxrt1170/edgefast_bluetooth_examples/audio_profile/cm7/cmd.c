/*
 * Copyright 2009 - 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "cmd.h"
#include <string.h>
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "ff.h"
#include <stdbool.h>
#include "fsl_debug_console.h"
#include "fsl_shell.h"
#include "diskio.h"

#include "app_streamer.h"
#include "app_music_control.h"
#include "app_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_TASK_STACK_SIZE (8 * 1024)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static shell_status_t shell_bt(shell_handle_t shellHandle, int32_t argc, char **argv);

/*******************************************************************************
 * Variables
 ******************************************************************************/

SHELL_COMMAND_DEFINE(bt,
                     "\r\n\"bt\": BT related function\r\n"
                     "  USAGE: bt [connectaddress|finddevice|connectdevice|disconnect|deletedevice]\r\n"
                     "    connectaddress connect to the device of the address parameter,for example: bt connectaddress xx:xx:xx:xx:xx:xx. Address format(LSB-MSB): xx:xx:xx:xx:xx:xx\r\n"
                     "    finddevice     start to find BT devices\r\n"
                     "    connectdevice  connect to the device that is found, for example: bt connectdevice n (from 1)\r\n"
                     "    disconnect     disconnect current connection\r\n"
                     "    deletedevice   delete all devices. Ensure to disconnect the HCI link connection with the peer device before attempting to delete the bonding information.\r\n",
                     shell_bt,
                     SHELL_IGNORE_PARAMETER_COUNT);

SDK_ALIGN(static uint8_t shell_handle_buffer[SHELL_HANDLE_SIZE], 4);
static shell_handle_t shell_handle;

/*******************************************************************************
 * Code
 ******************************************************************************/

static shell_status_t shell_bt(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    if (argc < 1)
    {
        PRINTF("the parameter count is wrong\r\n");
    }

    if (strcmp(argv[1], "connectaddress") == 0)
    {
        if (argc < 2)
        {
            PRINTF("the parameter count is wrong\r\n");
            return kStatus_SHELL_Error;
        }
        app_bt_connect_addr((char *)argv[2]);
    }
    else if (strcmp(argv[1], "finddevice") == 0)
    {
        app_bt_inquiry();
    }
    else if (strcmp(argv[1], "connectdevice") == 0)
    {
        uint8_t select_index = 0;
        char *ch = argv[2];

        if (argc < 2)
        {
            PRINTF("the parameter count is wrong\r\n");
            return kStatus_SHELL_Error;
        }

        for (select_index = 0; select_index < strlen(ch); ++select_index)
        {
            if ((ch[select_index] < '0') || (ch[select_index] > '9'))
            {
                PRINTF("the parameter is wrong\r\n");
                return kStatus_SHELL_Error;
            }
        }

        switch (strlen(ch))
        {
        case 1:
            select_index = ch[0] - '0';
            break;
        case 2:
            select_index = (ch[0] - '0') * 10 + (ch[1] - '0');
            break;
        default:
            PRINTF("the parameter is wrong\r\n");
            break;
        }

        if ((select_index == 0U) || (app_bt_connect_index(select_index - 1) < 0))
        {
            PRINTF("the parameter is wrong\r\n");
        }
    }
    else if (strcmp(argv[1], "disconnect") == 0)
    {
        app_bt_disconnect();
    }
    else if (strcmp(argv[1], "deletedevice") == 0)
    {
        app_bt_delete_device();
    }
    else
    {
    }

    return kStatus_SHELL_Success;
}

void shell_app_task(void *arg)
{
    DbgConsole_Flush();
    /* Init SHELL */
    shell_handle = &shell_handle_buffer[0];
    SHELL_Init(shell_handle, g_serialHandle, ">> ");

    /* Add new command to commands list */
    SHELL_RegisterCommand(shell_handle, SHELL_COMMAND(bt));

#if !(defined(SHELL_NON_BLOCKING_MODE) && (SHELL_NON_BLOCKING_MODE > 0U))
    SHELL_Task(shell_handle);
#endif
    vTaskDelete(NULL);
}

void shell_task_init(void)
{
    /* Set shell command task priority = 1 */
    if (xTaskCreate(shell_app_task, "Shell Task", APP_TASK_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL) != pdPASS)
    {
        PRINTF("\r\nFailed to create application task\r\n");
        while (1)
            ;
    }
}
