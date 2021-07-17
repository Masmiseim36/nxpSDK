/*
* Copyright (c) 2015-2021 Cadence Design Systems Inc.
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
/*******************************************************************************
 * xf-msgq.c
 *
 * System-specific IPC layer Implementation
 ******************************************************************************/

#define MODULE_TAG                      MSGQ

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "xf.h"
#include "xaf-api.h"
#include "xaf-structs.h"
#include "osal-msgq.h"

/*******************************************************************************
 * Global Definitions
 ******************************************************************************/

extern xf_ap_t *xf_g_ap;
extern XAF_ERR_CODE xaf_malloc(void **buf_ptr, int size, int id);

/* ...remote IPC is coherent for hostless xaf (say) */
#define XF_REMOTE_IPC_NON_COHERENT      0

/*******************************************************************************
 * Global abstractions
 ******************************************************************************/

/* ...prevent instructions reordering */
#define barrier()                           \
    __asm__ __volatile__("": : : "memory")

/* ...memory barrier */
#define XF_PROXY_BARRIER()                  \
    __asm__ __volatile__("memw": : : "memory")

/* ...memory invalidation */
#define XF_PROXY_INVALIDATE(buf, length)    \
    ({ if ((length)) { xthal_dcache_region_invalidate((buf), (length)); barrier(); } buf; })

/* ...memory flushing */
#define XF_PROXY_FLUSH(buf, length)         \
    ({ if ((length)) { barrier(); xthal_dcache_region_writeback((buf), (length)); XF_PROXY_BARRIER(); } buf; })


/*******************************************************************************
 * Local constants - tbd
 ******************************************************************************/

#define SEND_LOCAL_MSGQ_ENTRIES     16

/*******************************************************************************
 * Internal IPC API implementation
 ******************************************************************************/

int ipc_msgq_init(xf_msgq_t *cmdq, xf_msgq_t *respq, xf_event_t **msgq_event)
{

    *cmdq = *respq = NULL;

    __xf_lock(&(xf_g_ap->g_msgq_lock));
    
    if (xf_g_ap->g_ipc_msgq.init_done) 
    {
        *cmdq       = xf_g_ap->g_ipc_msgq.cmd_msgq;
        *respq      = xf_g_ap->g_ipc_msgq.resp_msgq;
        *msgq_event = &xf_g_ap->g_ipc_msgq.msgq_event;
        
        return 0;
    }

    xf_g_ap->g_ipc_msgq.cmd_msgq = __xf_msgq_create(SEND_MSGQ_ENTRIES, sizeof(xf_proxy_msg_t));
    /* ...allocation mustn't fail on App Interface Layer */
    BUG(xf_g_ap->g_ipc_msgq.cmd_msgq == NULL, _x("Out-of-memeory"));


    xf_g_ap->g_ipc_msgq.resp_msgq = __xf_msgq_create(RECV_MSGQ_ENTRIES, sizeof(xf_proxy_msg_t));
    /* ...allocation mustn't fail on App Interface Layer */
    BUG(xf_g_ap->g_ipc_msgq.resp_msgq == NULL, _x("Out-of-memeory"));

    __xf_event_init(&xf_g_ap->g_ipc_msgq.msgq_event, 0xffff);

    *cmdq       = xf_g_ap->g_ipc_msgq.cmd_msgq;
    *respq      = xf_g_ap->g_ipc_msgq.resp_msgq;
    *msgq_event = &xf_g_ap->g_ipc_msgq.msgq_event;

    xf_g_ap->g_ipc_msgq.init_done = 1;

    __xf_unlock(&(xf_g_ap->g_msgq_lock));

    return 0;
}

int ipc_msgq_delete(xf_msgq_t *cmdq, xf_msgq_t *respq)
{

    __xf_msgq_destroy(xf_g_ap->g_ipc_msgq.cmd_msgq);
    __xf_msgq_destroy(xf_g_ap->g_ipc_msgq.resp_msgq);
    __xf_event_destroy(&xf_g_ap->g_ipc_msgq.msgq_event);

    xf_g_ap->g_ipc_msgq.cmd_msgq   = NULL;
    xf_g_ap->g_ipc_msgq.resp_msgq  = NULL;
    xf_g_ap->g_ipc_msgq.init_done  = 0;

    *cmdq = *respq = NULL;

    return 0;
}

/* ...pass command to remote DSP */
int xf_ipc_send(xf_proxy_ipc_data_t *ipc, xf_proxy_msg_t *msg, void *b)
{

    TRACE(CMD, _b("C[%08x]:(%x,%08x,%u)"), msg->id, msg->opcode, msg->address, msg->length);

    if (XF_REMOTE_IPC_NON_COHERENT)
    {
        if (msg->address) XF_PROXY_FLUSH((void *)msg->address, msg->length);
    }
    
    /* ...pass message to xos message queue */
    __xf_msgq_send(ipc->cmd_msgq, msg, sizeof(*msg));
	__xf_event_set(ipc->msgq_event, CMD_MSGQ_READY);

    return 0;
}

int xf_ipc_wait(xf_proxy_ipc_data_t *ipc, UWORD32 timeout)
{
    __xf_event_wait_any(ipc->msgq_event, RESP_MSGQ_READY | DIE_MSGQ_ENTRY);
    if (__xf_event_get(ipc->msgq_event) & DIE_MSGQ_ENTRY)
        return -1;
    __xf_event_clear(ipc->msgq_event, RESP_MSGQ_READY);
    return 0;
}

/* ...read response from proxy - blocking API */
int xf_ipc_recv(xf_proxy_ipc_data_t *ipc, xf_proxy_msg_t *msg, void **buffer)
{
    if (__xf_msgq_empty(ipc->resp_msgq) ||
        __xf_msgq_recv(ipc->resp_msgq, msg, sizeof(*msg)) < 0)
        return 0;

    TRACE(RSP, _b("R[%08x]:(%x,%u,%08x)"), msg->id, msg->opcode, msg->length, msg->address);

    /* ...translate shared address into local pointer */
    XF_CHK_ERR((*buffer = xf_ipc_a2b(ipc, msg->address)) != (void *)-1, XAF_INVALIDVAL_ERR);

    /* ...return positive result indicating the message has been received */
    return sizeof(*msg);
}

/*******************************************************************************
 * Internal API functions implementation
 ******************************************************************************/

/* ...open proxy interface on proper DSP partition */
int xf_ipc_open(xf_proxy_ipc_data_t *ipc, UWORD32 core)
{
    /* ...open xos-msgq interface */
    XF_CHK_API(ipc_msgq_init(&ipc->cmd_msgq, &ipc->resp_msgq, &ipc->msgq_event));

    ipc->lresp_msgq = __xf_msgq_create(SEND_LOCAL_MSGQ_ENTRIES, sizeof(xf_user_msg_t));
    /* ...allocation mustn't fail on App Interface Layer */
    BUG(ipc->lresp_msgq == NULL, _x("Out-of-memeory"));

    TRACE(INIT, _b("proxy-%u interface opened"), core);

    return 0;
}

/* ...set event to close proxy handle */
void xf_ipc_close_set_event(xf_proxy_ipc_data_t *ipc, UWORD32 core)
{
	__xf_event_set(ipc->msgq_event, DIE_MSGQ_ENTRY);
}

/* ...close proxy handle */
void xf_ipc_close(xf_proxy_ipc_data_t *ipc, UWORD32 core)
{
    /* ...close xos-msgq interface */
    ipc_msgq_delete(&ipc->cmd_msgq, &ipc->resp_msgq);

    /* ...close proxy local queue */
    __xf_msgq_destroy(ipc->lresp_msgq);
    //xf_g_ap->xf_mem_free_fxn(ipc->lresp_msgq, XAF_MEM_ID_DEV);
    ipc->lresp_msgq = NULL;

    TRACE(INIT, _b("proxy-%u interface closed"), core);
}

/*******************************************************************************
 * Local component message queue API implementation
 ******************************************************************************/

int xf_ipc_data_init(xf_ipc_data_t *ipc)
{
    ipc->resp_msgq = __xf_msgq_create(SEND_LOCAL_MSGQ_ENTRIES, sizeof(xf_user_msg_t));
    /* ...allocation mustn't fail on App Interface Layer */
    BUG(ipc->resp_msgq == NULL, _x("Out-of-memeory"));
    return 0;
}

int xf_ipc_data_destroy(xf_ipc_data_t *ipc)
{
    __xf_msgq_destroy(ipc->resp_msgq);
    ipc->resp_msgq = NULL;
    return 0;
}

