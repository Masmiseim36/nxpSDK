/*
* Copyright (c) 2015-2024 Cadence Design Systems Inc.
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "osal-thread.h"
#include "osal-msgq.h"

/*******************************************************************************
 * Global Definitions
 ******************************************************************************/

#define SEND_MSGQ_ENTRIES              16
#define RECV_MSGQ_ENTRIES              16

#define CMD_MSGQ_READY                 0x1
#define RESP_MSGQ_READY                0x2
#define DIE_MSGQ_ENTRY                 0x4
#define DSP_DIE_MSGQ_ENTRY             0x8

#define SEND_LOCAL_MSGQ_ENTRIES         16

typedef struct IPC_MSGQ {

    xf_msgq_t     cmd_msgq;
    xf_msgq_t     resp_msgq;
    xf_event_t    msgq_event;
    int           init_done;

} ipc_msgq_t;

int ipc_msgq_init(xf_msgq_t *cmdq, xf_msgq_t *respq, xf_event_t **msgq_event);
int ipc_msgq_delete(xf_msgq_t *cmdq, xf_msgq_t *respq);
