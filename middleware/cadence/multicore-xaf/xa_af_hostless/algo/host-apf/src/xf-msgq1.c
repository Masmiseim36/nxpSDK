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
/*******************************************************************************
 * xf-msgq1.c
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

/* ...remote IPC is coherent for hostless xaf (say) */
#define XF_REMOTE_IPC_NON_COHERENT      0

/*******************************************************************************
 * Global abstractions
 ******************************************************************************/

#if XF_REMOTE_IPC_NON_COHERENT
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
#endif
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

        __xf_unlock(&(xf_g_ap->g_msgq_lock));

        return 0;
    }

    void *pbuf = NULL;
#if !defined(HAVE_FREERTOS)
    if(xaf_malloc(&pbuf, __xf_msgq_get_size(SEND_MSGQ_ENTRIES, sizeof(xf_proxy_msg_t)), XF_POOL_FRMWK_MSGQ))
    {
        return XAF_MEMORY_ERR;
    }
    xf_g_ap->g_ipc_msgq.cmd_msgq = __xf_msgq_create(SEND_MSGQ_ENTRIES, sizeof(xf_proxy_msg_t), pbuf);
    /* ...allocation mustn't fail on App Interface Layer */
    BUG(xf_g_ap->g_ipc_msgq.cmd_msgq == NULL, _x("Out-of-memeory"));

    if(xaf_malloc(&pbuf, __xf_msgq_get_size(RECV_MSGQ_ENTRIES, sizeof(xf_proxy_msg_t)), XF_POOL_FRMWK_MSGQ))
    {
        return XAF_MEMORY_ERR;
    }
    xf_g_ap->g_ipc_msgq.resp_msgq = __xf_msgq_create(RECV_MSGQ_ENTRIES, sizeof(xf_proxy_msg_t), pbuf);
    /* ...allocation mustn't fail on App Interface Layer */
    BUG(xf_g_ap->g_ipc_msgq.resp_msgq == NULL, _x("Out-of-memeory"));

#else //!defined(HAVE_FREERTOS)

    xf_g_ap->g_ipc_msgq.cmd_msgq = __xf_msgq_create(SEND_MSGQ_ENTRIES, sizeof(xf_proxy_msg_t), pbuf);
    /* ...allocation mustn't fail on App Interface Layer */
    BUG(xf_g_ap->g_ipc_msgq.cmd_msgq == NULL, _x("Out-of-memeory"));

    xf_g_ap->g_ipc_msgq.resp_msgq = __xf_msgq_create(RECV_MSGQ_ENTRIES, sizeof(xf_proxy_msg_t), pbuf);
    /* ...allocation mustn't fail on App Interface Layer */
    BUG(xf_g_ap->g_ipc_msgq.resp_msgq == NULL, _x("Out-of-memeory"));

#endif //!defined(HAVE_FREERTOS)

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

    __xf_msgq_destroy(xf_g_ap->g_ipc_msgq.resp_msgq);
    __xf_msgq_destroy(xf_g_ap->g_ipc_msgq.cmd_msgq);
#if !defined(HAVE_FREERTOS)
    xaf_free(xf_g_ap->g_ipc_msgq.cmd_msgq, XF_POOL_FRMWK_MSGQ);
    xaf_free(xf_g_ap->g_ipc_msgq.resp_msgq, XF_POOL_FRMWK_MSGQ);
#endif

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

    TRACE(CMD, _b("C[%016llx]:(%x,%08x,%u,%d)"), (UWORD64)msg->id, msg->opcode, msg->address, msg->length, msg->error);

#if XF_REMOTE_IPC_NON_COHERENT
    {
        if (msg->address) XF_PROXY_FLUSH((void *)msg->address, msg->length);
    }
#endif

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

    TRACE(RSP, _b("R[%016llx]:(%x,%u,%08x,%d)"), (UWORD64)msg->id, msg->opcode, msg->length, msg->address, msg->error);

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
    /* ...open msgq interface */
    XF_CHK_API(ipc_msgq_init(&ipc->cmd_msgq, &ipc->resp_msgq, &ipc->msgq_event));

    void *pbuf = NULL;
#if !defined(HAVE_FREERTOS)
    if(xaf_malloc(&pbuf, __xf_msgq_get_size(SEND_LOCAL_MSGQ_ENTRIES, sizeof(xf_user_msg_t)), XF_POOL_FRMWK_MSGQ))
    {
        return XAF_MEMORY_ERR;
    }
    ipc->lresp_msgq = __xf_msgq_create(SEND_LOCAL_MSGQ_ENTRIES, sizeof(xf_user_msg_t), pbuf);
    /* ...allocation mustn't fail on App Interface Layer */
    BUG(ipc->lresp_msgq == NULL, _x("Out-of-memeory lresp_msgq"));

#ifdef DELAYED_SYNC_RESPONSE
    if(xaf_malloc(&pbuf, __xf_msgq_get_size(SEND_LOCAL_MSGQ_ENTRIES, sizeof(xf_user_msg_t)), XF_POOL_FRMWK_MSGQ))
    {
        return XAF_MEMORY_ERR;
    }
    ipc->lresp_msgq_delayed = __xf_msgq_create(SEND_LOCAL_MSGQ_ENTRIES, sizeof(xf_user_msg_t), pbuf);
    /* ...allocation mustn't fail on App Interface Layer */
    BUG(ipc->lresp_msgq_delayed == NULL, _x("Out-of-memeory lresp_msgq_delayed"));
#endif

#else //!defined(HAVE_FREERTOS)
    ipc->lresp_msgq = __xf_msgq_create(SEND_LOCAL_MSGQ_ENTRIES, sizeof(xf_user_msg_t), pbuf);
    /* ...allocation mustn't fail on App Interface Layer */
    BUG(ipc->lresp_msgq == NULL, _x("Out-of-memeory lresp_msgq"));

#ifdef DELAYED_SYNC_RESPONSE
    ipc->lresp_msgq_delayed = __xf_msgq_create(SEND_LOCAL_MSGQ_ENTRIES, sizeof(xf_user_msg_t), pbuf);
    /* ...allocation mustn't fail on App Interface Layer */
    BUG(ipc->lresp_msgq_delayed == NULL, _x("Out-of-memeory lresp_msgq_delayed"));
#endif
#endif //!defined(HAVE_FREERTOS)

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
#if !defined(HAVE_FREERTOS)
    xaf_free(ipc->lresp_msgq, XF_POOL_FRMWK_MSGQ);
#endif //!defined(HAVE_FREERTOS)
    ipc->lresp_msgq = NULL;

#ifdef DELAYED_SYNC_RESPONSE
    __xf_msgq_destroy(ipc->lresp_msgq_delayed);
#if !defined(HAVE_FREERTOS)
    xaf_free(ipc->lresp_msgq_delayed, XF_POOL_FRMWK_MSGQ);
#endif //!defined(HAVE_FREERTOS)
    ipc->lresp_msgq_delayed = NULL;
#endif

    TRACE(INIT, _b("proxy-%u interface closed"), core);
}

/*******************************************************************************
 * Local component message queue API implementation
 ******************************************************************************/

int xf_ipc_data_init(xf_ipc_data_t *ipc)
{
    void *pbuf = NULL;
#if !defined(HAVE_FREERTOS)
    if(xaf_malloc(&pbuf, __xf_msgq_get_size(SEND_LOCAL_MSGQ_ENTRIES, sizeof(xf_user_msg_t)), XF_POOL_FRMWK_MSGQ))
    {
        return XAF_MEMORY_ERR;
    }
#endif
    ipc->resp_msgq = __xf_msgq_create(SEND_LOCAL_MSGQ_ENTRIES, sizeof(xf_user_msg_t), pbuf);
    /* ...allocation mustn't fail on App Interface Layer */
    BUG(ipc->resp_msgq == NULL, _x("Out-of-memeory"));
    return 0;
}

int xf_ipc_data_destroy(xf_ipc_data_t *ipc)
{
    __xf_msgq_destroy(ipc->resp_msgq);
#if !defined(HAVE_FREERTOS)
    xaf_free(ipc->resp_msgq, XF_POOL_FRMWK_MSGQ);
#endif
    ipc->resp_msgq = NULL;
    return 0;
}

