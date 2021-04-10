/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <sys/printk.h>
#include <sys/byteorder.h>
#include <porting.h>

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
#include <bluetooth/addr.h>
#include "fsl_debug_console.h"
#include "fsl_shell.h"
#include "app_shell.h"
#include "app_discover.h"
#include "app_connect.h"
#include "bluetooth/spp.h"

#if 0
static char spp_send_str_options[] = "\n\
--------- Sample Strings ---------\n\
    1.  AT+CIND=?\\r\n\
    2.  AT+CIND?\\r\n\
    3.  ATEP\\r\n\
    4.  AT+CKPD=E\\r\n\
\n\
Your Choice[1-4] -> \0";
#endif

#define BT_DEVICE_ADDR_POINTER(ref)\
        (ref)[0],(ref)[1],(ref)[2],(ref)[3],(ref)[4],(ref)[5]

SDK_ALIGN(static uint8_t spp_shell_handle_buffer[SHELL_HANDLE_SIZE], 4);
static shell_handle_t spp_shell_handle;

/* Send Str Buffer */
static uint8_t appl_spp_buffer[31];

static struct bt_spp *spp_handle;

static uint8_t spp_connection_count;

static void spp_connected(struct bt_spp *spp, int error)
{
    if(0 == error)
    {
        spp_handle = spp;
        spp_connection_count++;
        PRINTF("SPP connection is created successfully!\n");
    }
    else
    {
        PRINTF("SPP connection is created failed, reason = 0x%04X.\n", error);
    }
}

static void spp_disconnected(struct bt_spp *spp, int error)
{
    if(0 == error)
    {
        spp_handle = NULL;
        spp_connection_count--;
        PRINTF("SPP connection is disconnected successfully!\n");
    }
    else
    {
        PRINTF("SPP connection is disconnected failed, reason = 0x%04X.\n", error);
    }
}

static void spp_data_received(struct bt_spp *spp, uint8_t *data, uint16_t len, int error)
{
    uint32_t index;

    PRINTF("\nStatus of SPP data received callback: 0x%04X.\n", error);
    PRINTF("Received %d data, dumped here:\n", len);
    PRINTF("\n----------------CHAR DUMP-----------------------\n");
    for (index = 0; index < len; index++)
    {
        PRINTF("%c ", data[index]);
    }
    PRINTF("\n------------------------------------------------\n");
    PRINTF("\n----------------HEX DUMP------------------------\n");
    for (index = 0; index < len; index++)
    {
        PRINTF("%02X ", data[index]);
    }
    PRINTF("\n------------------------------------------------\n");
}

static void spp_data_sent(struct bt_spp *spp, uint8_t *data, uint16_t len, int error)
{
    uint32_t index;

    PRINTF("\nStatus of SPP data sent callback: 0x%04X.\n", error);
    PRINTF("Sent %d data, dumped here:\n", len);
    PRINTF("\n----------------CHAR DUMP-----------------------\n");
    for (index = 0; index < len; index++)
    {
        PRINTF("%c ", data[index]);
    }
    PRINTF("\n------------------------------------------------\n");
}

static bt_spp_callback spp_application_callback =
{
    .connected     = spp_connected,
    .disconnected  = spp_disconnected,
    .data_received = spp_data_received,
    .data_sent     = spp_data_sent,
};

static int spp_sdp_discover_callback(struct bt_conn *conn, uint8_t count, uint16_t *channel);

static discover_cb_t discover_callback =
{
    .cb = spp_sdp_discover_callback,
};

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_TASK_STACK_SIZE (2 * 1024)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static shell_status_t shell_bt(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t shell_spp(shell_handle_t shellHandle, int32_t argc, char **argv);

/*******************************************************************************
 * Variables
 ******************************************************************************/

SHELL_COMMAND_DEFINE(bt,
                     "\r\n\"bt\": BT related function\r\n"
                     "  USAGE: bt [discover|connect|disconnect|delete]\r\n"
                     "    bt discover    start to find BT devices\r\n"
                     "    bt connect     connect to the device that is found, for example: bt connectdevice n (from 1)\r\n"
                     "    bt disconnect  disconnect current connection.\r\n"
                     "    bt delete      delete all devices. Ensure to disconnect the HCI link connection with the peer device before attempting to delete the bonding information.\r\n",
                     shell_bt,
                     SHELL_IGNORE_PARAMETER_COUNT);

SHELL_COMMAND_DEFINE(spp,
                     "\r\n\"spp\": SPP related function\r\n"
                     "  USAGE: \r\n"
                     "    spp register [5|3]    register a spp server channel\r\n"
                     "    spp discover          discover spp server channel on peer device\r\n"
                     "    spp connect [channel] create spp connection\r\n"
                     "    spp disconnect        disconnect current spp connection.\r\n"
                     "    spp send [1|2|3|4]    send data over spp connection.\r\n",
                     shell_spp,
                     SHELL_IGNORE_PARAMETER_COUNT);

/*******************************************************************************
 * Code
 ******************************************************************************/
static int spp_sdp_discover_callback(struct bt_conn *conn, uint8_t count, uint16_t *channel)
{
    uint8_t index;

    PRINTF("Discover %d SPP server channel from device %02X:%02X:%02X:%02X:%02X:%02X!\n", count, BT_DEVICE_ADDR_POINTER(bt_conn_get_dst_br(conn)->val));

    for(index = 0U; index < count; index++)
    {
        PRINTF("0x%04x\n", channel[index]);
    }

    return 0;
}

static shell_status_t shell_bt(shell_handle_t shellHandle, int32_t argc, char **argv)
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

        if (select_index == 0U)
        {
            PRINTF("the parameter is wrong\r\n");
        }
        addr = app_get_addr(select_index - 1);
        app_connect(addr);
    }
    else if (strcmp(argv[1], "disconnect") == 0)
    {
        app_disconnect();
    }
    else if (strcmp(argv[1], "delete") == 0)
    {
        int err = 0;
        err = bt_unpair(BT_ID_DEFAULT, NULL);
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

static shell_status_t shell_spp(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    int     retval;
    uint8_t select_index = 0;
    char  * ch = argv[2];

    if (argc < 1)
    {
        PRINTF("the parameter count is wrong\r\n");
        return kStatus_SHELL_Error;
    }

    if (strcmp(argv[1], "register") == 0)
    {
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

        retval = bt_spp_server_register(select_index, &spp_application_callback);
        if (0 == retval)
        {
            PRINTF("Register spp server channel %d successfully!\n", select_index);
        }
        else
        {
            PRINTF("Register channel %d failed, reason = %d\n",select_index, retval);
        }
    }
    else if (strcmp(argv[1], "discover") == 0)
    {
        retval = bt_spp_discover(default_conn, &discover_callback);
    }
    else if (strcmp(argv[1], "connect") == 0)
    {
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

        if ( 0U == spp_connection_count)
        {
            retval = bt_spp_client_connect(default_conn, select_index, &spp_application_callback, &spp_handle);

            if (0 == retval)
            {
                PRINTF("Connect SPP Successful!\n");

            }
            else
            {
                PRINTF("Connect SPP failed. retval = %d\n", retval);
            }
        }
        else
        {
            PRINTF("Just 1 SPP connection is supported, please disconnect previous connection and try!\n");
        }
    }
    else if (strcmp(argv[1], "disconnect") == 0)
    {
        retval = bt_spp_disconnect(spp_handle);
        if (0 != retval)
        {
            PRINTF("SPP disconnect failed, reason = %d\n", retval);
        }
    }
    else if (strcmp(argv[1], "send") == 0)
    {
        /*
         * 1.  AT+CIND=?\\r\n\
         * 2.  AT+CIND?\\r\n\
         * 3.  ATEP\\r\n\
         * 4.  AT+CKPD=E\\r\n\
         */

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

        switch( select_index )
        {
        case 1:
            sprintf((char *)appl_spp_buffer,"AT+CIND=?\\r");
            break;

        case 2:
            sprintf((char *)appl_spp_buffer,"AT+CIND?\\r");
            break;

        case 3:
            sprintf((char *)appl_spp_buffer,"ATEP\\r");
            break;

        case 4:
            sprintf((char *)appl_spp_buffer,"AT+CKPD=E\\r");
            break;

        default:
            PRINTF("Invalid choice. Defaulting to AT+CIND=?\\r\n");
            sprintf((char *)appl_spp_buffer,"AT+CIND=?\\r");
            break;
        } /* switch */

        retval = bt_spp_data_send
                 (
                     spp_handle,
                     appl_spp_buffer,
                     (uint16_t)sizeof(appl_spp_buffer)
                 );
        if (0 != retval)
        {
            PRINTF("Send string failed, reason = %d\n",retval);
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
    spp_shell_handle = &spp_shell_handle_buffer[0];
    SHELL_Init(spp_shell_handle, g_serialHandle, ">> ");

    /* Add new command to commands list */
    SHELL_RegisterCommand(spp_shell_handle, SHELL_COMMAND(bt));
    SHELL_RegisterCommand(spp_shell_handle, SHELL_COMMAND(spp));
}
