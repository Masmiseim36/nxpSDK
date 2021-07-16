/*
 * Copyright 2019-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "ff.h"
#include <stdbool.h>
#include <sys/atomic.h>
#include <sys/byteorder.h>
#include <sys/util.h>
#include <sys/slist.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include "fsl_debug_console.h"
#include "fsl_shell.h"
#include "app_shell.h"
#include "app_discover.h"
#include "app_connect.h"
#include "app_handsfree_ag.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static shell_status_t shellBt(shell_handle_t shellHandle, int32_t argc, char **argv);

/*******************************************************************************
 * Variables
 ******************************************************************************/

SHELL_COMMAND_DEFINE(bt,
                     "\r\n\"bt\": BT related function\r\n"
                     "  USAGE: bt [discover|connect|disconnect|delete]\r\n"
                     "    discover    start to find BT devices\r\n"
                     "    connect     connect to the device that is found, for example: bt connect n (from 1)\r\n"
                     "    sincall     start an incoming call.\r\n"
                     "    aincall     accept the incoming call.\r\n"
                     "    eincall     end an incoming call.\r\n"
                     "    disconnect  disconnect current connection.\r\n"
                     "    delete      delete all devices. Ensure to disconnect the HCI link connection with the peer "
                     "device before attempting to delete the bonding information.\r\n",
                     shellBt,
                     SHELL_IGNORE_PARAMETER_COUNT);

SDK_ALIGN(static uint8_t s_shellHandleBuffer[SHELL_HANDLE_SIZE], 4);
static shell_handle_t s_shellHandle;

/*******************************************************************************
 * Code
 ******************************************************************************/

static shell_status_t shellBt(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    uint8_t *addr;

    if (argc < 1)
    {
        PRINTF("the parameter count is wrong\r\n");
    }

    if (strcmp(argv[1], "discover") == 0)
    {
        app_discover();
    }
    else if (strcmp(argv[1], "connect") == 0)
    {
        uint8_t selectIndex = 0;
        char *ch            = argv[2];

        if (argc < 2)
        {
            PRINTF("the parameter count is wrong\r\n");
            return kStatus_SHELL_Error;
        }

        for (selectIndex = 0; selectIndex < strlen(ch); ++selectIndex)
        {
            if ((ch[selectIndex] < '0') || (ch[selectIndex] > '9'))
            {
                PRINTF("the parameter is wrong\r\n");
                return kStatus_SHELL_Error;
            }
        }

        switch (strlen(ch))
        {
            case 1:
                selectIndex = ch[0] - '0';
                break;
            case 2:
                selectIndex = (ch[0] - '0') * 10 + (ch[1] - '0');
                break;
            default:
                PRINTF("the parameter is wrong\r\n");
                break;
        }

        if (selectIndex == 0U)
        {
            PRINTF("the parameter is wrong\r\n");
        }
        addr = app_get_addr(selectIndex - 1);
        app_connect(addr);
    }
    else if (strcmp(argv[1], "disconnect") == 0)
    {
        app_disconnect();
    }
    else if (strcmp(argv[1], "sincall") == 0)
    {
        app_hfp_ag_start_incoming_call();
    }
    else if (strcmp(argv[1], "aincall") == 0)
    {
        app_hfp_ag_accept_incoming_call();
    }
    else if (strcmp(argv[1], "eincall") == 0)
    {
        app_hfp_ag_stop_incoming_call();
    }
    else if (strcmp(argv[1], "delete") == 0)
    {
        int err = 0;
        err     = bt_unpair(BT_ID_DEFAULT, NULL);
        if (err != 0)
        {
            PRINTF("failed reason = %d\r\n", err);
        }
        else
        {
            PRINTF("success\r\n");
        }
    }
    else
    {
    }

    return kStatus_SHELL_Success;
}

void app_shell_init(void)
{
    DbgConsole_Flush();
    /* Init SHELL */
    s_shellHandle = &s_shellHandleBuffer[0];
    SHELL_Init(s_shellHandle, g_serialHandle, ">> ");
    PRINTF("\r\n");

    /* Add new command to commands list */
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(bt));
}
