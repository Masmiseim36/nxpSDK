/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* Kernel includes. */
#include "os.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "pin_mux.h"
#include <stdbool.h>
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define STACK_SIZE (512)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void hello_task(void *p_arg);

/*******************************************************************************
 * Globals
 ******************************************************************************/
CPU_STK stack_hello[STACK_SIZE];
OS_TCB tcb_hello;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    OS_ERR err;
    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    OSInit(&err);
    if (err != OS_ERR_NONE)
    {
        PRINTF("Error.");
    }

    OSTaskCreate(&tcb_hello,   /* Task control block pointer. */
                 "hello_task", /* Task name. */
                 hello_task,   /* Task function pointer. */
                 NULL,         /* Task creation parameter. */
                 5,            /* Task priority. */
                 stack_hello,  /* Task stack base address. */
                 0,            /* Task stack limit address. */
                 STACK_SIZE,   /* Task stack size. */
                 0,            /* Maximum number of messages that can be sent to the task.  */
                 0,            /* Time slice. */
                 NULL,         /* Pointer to TCB extension. */
                 0,            /* Additional options. */
                 &err);        /* Error code. */
    if (err != OS_ERR_NONE)
    {
        PRINTF("Error.");
    }

    OSStart(&err);
    if (err != OS_ERR_NONE)
    {
        PRINTF("Error.");
    }

    return 0;
}

/*!
 * @brief Task hello_task is responsible for printing of "Hello world." message.
 */
static void hello_task(void *p_arg)
{
    OS_ERR err;
    uint32_t i = 0;

    OS_CPU_SysTickInit(CLOCK_GetFreq(kCLOCK_CoreSysClk));

    for (;;)
    {
        for (i = 0; i < 10; i++)
        {
            PRINTF("Hello world.\r\n");
            OSTimeDlyHMSM(0, 0, 1, 0, OS_OPT_TIME_HMSM_STRICT, &err);
        }
        OSTaskSuspend(NULL, &err);
        if (err != OS_ERR_NONE)
        {
            PRINTF("Error.");
        }
    }
}
