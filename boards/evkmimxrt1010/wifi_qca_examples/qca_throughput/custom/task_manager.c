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
/*
 * $FileName: Task_Manager.c$
 * $Version : $
 * $Date    : May-20-2011$
 *
 * Comments: Handles all task management functions including mutithreaded support.
 *
 *
 *
 *END************************************************************************/

#include "main.h"
#include <string.h>
#include <stdlib.h>

#include "throughput.h"
#include "worker_shell.h"

#if READ_HOST_MEMORY
extern uint32_t __END_BSS;
extern uint32_t __START_BSS;
extern uint32_t __END_TEXT;
extern uint32_t __START_TEXT;
extern uint32_t __END_DATA;
extern uint32_t __START_DATA;
extern uint32_t __END_RODATA;
extern uint32_t __START_RODATA;
#endif

/* MMJ: A simpler, leaner and more scalable implementation of the functionality
 * of this module would be:
 *
 * Have one (atomically accessed) variable, task_pool_bits, where each bit
 * corresponds to one task thread. The bit is set if the task is busy.
 * I.e. bit 0 set means task 0 is running, bit 1 set means task 1 is running, etc.
 *
 * Tasks are woken up by *one* EventGroup where each bit corresponds to one task.
 *
 * And that's it.
 *
 * The scheme could be extended for even greater scalability by creating the
 * threads on the fly.
 *
 * MMJ: Current implementation has following issues:
 *   - task_counter indicates the number of tasks currently running and NOT
 *     which thread(s) that are active/idle. Nevertheless, task_counter is used
 *     to wakeup an idle thread, meaning that sometimes, the wrong task is woken
 *   - There is no need for two task_event's when you can have one bit per task
 */

/*************************** GLOBALS **************************/

extern WMI_POWER_MODE power_mode;
uint8_t wps_in_progress = 0;
uint8_t number_streams  = 0; // Counter to control number of streams

// I wonder if this is always the same as p_Driver_Cxt[0] (from the driver)
_qca_handle handle;

/*************************** Function Declarations **************************/

#if ENABLE_P2P_MODE
extern uint32_t task2_msec;
extern int32_t p2p_callback(uint8_t evt_flag);
#endif

/************************************************************************
 * NAME: DoPeriodic
 *
 * DESCRIPTION: Periodic WPS query.
 ************************************************************************/
static void DoPeriodic(void)
{
#if USE_ATH_CHANGES
    wps_query(0);
#endif
#if ENABLE_P2P_MODE
    p2p_callback(1);
#endif
}

/************************************************************************
 * NAME: worker_cmd_quit
 *
 * DESCRIPTION: Called when user enters "benchquit"
 ************************************************************************/
int worker_cmd_quit(shell_handle_t context, int argc, char **argv)
{
    bench_quit = 1;
    return 0;
}

extern QueueHandle_t shell_q;
worker_shell_msg_t g_msg = {0};

/************************************************************************
 * NAME: worker_cmd_handler
 *
 * DESCRIPTION: Handles incoming commands from Shell task
 ************************************************************************/
int32_t worker_cmd_handler(shell_handle_t context, int32_t argc, char **argv)
{
    assert(argc < WORKER_SHELL_ARGC);
    g_msg.argc = argc;

    /* copy parsed argv strings into 'packed' line and set msg.argv pointers */
    for (int32_t argc_i = 0, line_i = 0; argc_i < argc; argc_i++)
    {
        int32_t argv_len = strlen(argv[argc_i]) + 1;
        assert(line_i + argv_len < WORKER_SHELL_ARGV_LINE);
        memcpy(&g_msg.argv_line[line_i], argv[argc_i], argv_len);
        /* queueSend copy whole 'msg' so we need to store 'argv' indexes instead of
         * pointers, worker task then needs to add 'line' address on queueReceive */
        g_msg.argv[argc_i] = (void *)line_i;
        line_i += argv_len;
    }

    /* Send shell 'argc', 'argv', 'line' as message to worker task.
     * Receive performs shallow copy. TODO: how long to block ?? */
    if (pdFALSE == xQueueSend(shell_q, &g_msg, (TickType_t)0))
    {
        PRINTF("Worker tasks are busy");
    }

    return 0;
}

/************************************************************************
 * NAME: DoWork
 *
 * DESCRIPTION: Calls functions corresponding to shell command.
 ************************************************************************/
static void DoWork(int argc, char *argv[])
{
    if (strcmp(argv[0], "benchtx") == 0)
    {
        if (wifi_connected_flag)
        {
            if (number_streams < MAX_STREAMS_ALLOWED)
            {
                number_streams++;
                tx_command_parser(argc, argv);
                number_streams--;
            }
            else
            {
                PRINTF("ERROR: Only 2 simultaneous traffic streams allowed" NL);
            }
        }
        else
            PRINTF("ERROR: No WiFi connection available, please connect to an Access Point" NL);
    }
    else if (strcmp(argv[0], "benchrx") == 0)
    {
        if (wifi_connected_flag)
        {
            if (number_streams < MAX_STREAMS_ALLOWED)
            {
                number_streams++;
                rx_command_parser(argc, argv);
                number_streams--;
            }
            else
            {
                PRINTF("ERROR: Only 2 simultaneous traffic streams allowed" NL);
            }
        }
        else
            PRINTF("No WiFi connection available, please connect to an Access Point" NL);
    }
#if READ_HOST_MEMORY
    else if (strcmp(task_argv[0], "hostmemmap") == 0)
    {
        PRINTF("text   : %d bytes" NL, (uint32_t)&__END_TEXT - (uint32_t)&__START_TEXT);
        PRINTF("bss    : %d bytes" NL, (uint32_t)&__END_BSS - (uint32_t)&__START_BSS);
        PRINTF("data   : %d bytes" NL, (uint32_t)&__END_DATA - (uint32_t)&__START_DATA);
        PRINTF("rodata : %d bytes" NL, (uint32_t)&__END_RODATA - (uint32_t)&__START_RODATA);
    }
#endif
#if (ENABLE_STACK_OFFLOAD && MULTI_SOCKET_SUPPORT)
    else if (strcmp(argv[0], "benchrx_multi_socket") == 0)
    {
        if (wifi_connected_flag)
        {
            rx_command_parser_multi_socket(argc, argv);
        }
        else
            PRINTF("No WiFi connection available, please connect to an Access Point" NL);
    }
#endif
#if USE_ATH_CHANGES
    else if (strcmp(argv[0], "wmiconfig") == 0)
    {
        wmiconfig_handler(argc, argv);
    }
    else if (strcmp(argv[0], "iwconfig") == 0)
    {
        wmi_iwconfig(argc, argv);
    }
#endif
#if ENABLE_STACK_OFFLOAD
    else if (strcmp(argv[0], "benchmode") == 0)
    {
        if (number_streams == 0)
            setBenchMode(argc, argv);
        else
            PRINTF("Cannot change mode while test is running" NL);
    }
#if ENABLE_SSL
    else if (strcmp(argv[0], "getcert") == 0)
    {
        ssl_get_cert_handler(argc, argv);
    }
#if ENABLE_HTTPS_SERVER
    else if (strcmp(argv[0], "httpss") == 0)
    {
        https_server_handler(argc, argv);
    }
#endif
#if ENABLE_HTTPS_CLIENT
    else if (strcmp(argv[0], "httpsc") == 0)
    {
        https_client_handler(argc, argv);
    }
#endif
#endif // ENABLE_SSL
#endif // ENABLE_STACK_OFFLOAD
    else
    {
        PRINTF("ERROR: Bad command: %s" NL, argv[0]);
    }
}

// two worker tasks - rx/tx bench at the same time
/************************************************************************
 * NAME: wmiconfig_Task1
 *
 * DESCRIPTION: Handler for 1st task, waits for Shell task to provide command
 ************************************************************************/

void wmiconfig_Task1(uint32_t temp)
{
    worker_shell_msg_t shell_msg = {0};

    while (1)
    {
        // TODO: wait forever ?? - rx/tx becch
        if (pdTRUE == xQueueReceive(shell_q, &shell_msg, (TickType_t)1000))
        {
            /* recalculate argv indexes to pointers */
            for (int32_t argc_i = 0; argc_i < shell_msg.argc; argc_i++)
            {
                shell_msg.argv[argc_i] = &shell_msg.argv_line[0] + (uint32_t)shell_msg.argv[argc_i];
            }
            DoWork(shell_msg.argc, shell_msg.argv);
        }
        else
        {
            DoPeriodic();
        }
    }
}

void wmiconfig_Task2(uint32_t temp)
{
    worker_shell_msg_t shell_msg = {0};

    while (1)
    {
        // TODO: wait forever ?? - rx/tx becch
        if (pdTRUE == xQueueReceive(shell_q, &shell_msg, (TickType_t)1000))
        {
            /* recalculate argv indexes to pointers */
            for (int32_t argc_i = 0; argc_i < shell_msg.argc; argc_i++)
            {
                shell_msg.argv[argc_i] = &shell_msg.argv_line[0] + (uint32_t)shell_msg.argv[argc_i];
            }
            DoWork(shell_msg.argc, shell_msg.argv);
        }
        else
        {
            DoPeriodic();
        }
    }
}
