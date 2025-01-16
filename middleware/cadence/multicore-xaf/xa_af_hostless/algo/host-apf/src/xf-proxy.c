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
 * xf-proxy.c
 *
 * Xtensa audio processing framework; application side
 ******************************************************************************/

#define MODULE_TAG                      PROXY

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "xf.h"
#include "xaf-structs.h"
#include "xaf-threads-priority.h"

/* ...check if non-zero value is a power-of-two */
#define xf_is_power_of_two(v)       (((v) & ((v) - 1)) == 0)

/* ...invalid proxy address */
#define XF_PROXY_BADADDR        XF_CFG_REMOTE_IPC_POOL_SIZE

#define XA_EXT_CFG_ID_OFFSET       0
#define XA_EXT_CFG_BUF_PTR_OFFSET  1

#define XAF_4BYTE_ALIGN    4


/* ...check the condition is true */
#define XF_CHK_ERR_UNLOCK(proxy, cond, error)   \
({                                              \
    intptr_t __ret;                             \
                                                \
    if (!(__ret = (intptr_t)(cond)))            \
    {                                           \
        TRACE(ERROR, _x("check failed"));       \
        xf_proxy_unlock(proxy);                 \
        return (error);                         \
    }                                           \
    (int)__ret;                                 \
})

/* ...check null pointer */
#define XF_CHK_PTR_UNLOCK(proxy, ptr)                       \
({                                                          \
    int __ret;                                              \
                                                            \
    if ((__ret = (int)(ptr)) == 0)                          \
    {                                                       \
        TRACE(ERROR, _x("Null pointer error: %d"), __ret);  \
        xf_proxy_unlock(proxy);                             \
        return XAF_INVALIDPTR_ERR;                          \
    }                                                       \
    __ret;                                                  \
})

/* ...check range */
#define XF_CHK_RANGE_UNLOCK(proxy, val, min, max)           \
({                                                          \
    int __ret = val;                                        \
                                                            \
    if ((__ret < (int)min) || (__ret > (int)max))           \
    {                                                       \
        TRACE(ERROR, _x("Invalid value: %d"), __ret);       \
        xf_proxy_unlock(proxy);                             \
        return XAF_INVALIDVAL_ERR;                          \
    }                                                       \
    __ret;                                                  \
})
/*******************************************************************************
 * DSP-ONLY-SOLUTION Global variables, TBD - move
 ******************************************************************************/
#ifdef HIFI_ONLY_XAF
const char proxy_thread_name[] = "proxyListen";
extern xf_ap_t *xf_g_ap;
#endif /* HIFI_ONLY_XAF */

/*******************************************************************************
 * Internal functions definitions
 ******************************************************************************/

/* ...execute proxy command synchronously */
static inline int xf_proxy_cmd_exec(xf_proxy_t *proxy, xf_user_msg_t *msg)
{
    xf_proxy_msg_t  m;

    /* ...send command to DSP Interface Layer */
    m.id = msg->id, m.opcode = msg->opcode, m.length = msg->length, m.error = 0;

    /* ...translate address */
    XF_CHK_ERR((m.address = xf_proxy_b2a(proxy, msg->buffer)) != XF_PROXY_BADADDR, XAF_INVALIDVAL_ERR);

    /* ...pass command to DSP Interface Layer */
    XF_CHK_API(xf_ipc_send(&proxy->ipc, &m, msg->buffer));

    /* ...wait for response reception indication from proxy thread */
#ifdef DELAYED_SYNC_RESPONSE
    if(XF_AP_IPC_CLIENT_SRC(m.id))
    {
        XF_CHK_API(xf_proxy_response_get_delayed(proxy, &m));
    }
    else
#endif //DELAYED_SYNC_RESPONSE
    {
        XF_CHK_API(xf_proxy_response_get(proxy, &m));
    }

    /* ...copy parameters */
    msg->id = m.id, msg->opcode = m.opcode, msg->length = m.length, msg->error = m.error;

    /* ...translate address back to virtual space */
    XF_CHK_ERR((msg->buffer = xf_proxy_a2b(proxy, m.address)) != (void *)-1, XAF_INVALIDVAL_ERR);

    TRACE(EXEC, _b("proxy[%p]: command done: [%08x:%p:%u:%d]"), proxy, msg->opcode, msg->buffer, msg->length, msg->error);

    return 0;
}

/* ...allocate local client-id number */
static inline xf_msg_id_dtype xf_client_alloc(xf_proxy_t *proxy, xf_handle_t *handle)
{
    xf_msg_id_dtype     client;

    if ((client = proxy->cmap[0].next) != 0)
    {
        /* ...pop client from free clients list */
        proxy->cmap[0].next = proxy->cmap[client].next;

        /* ...put client handle into association map */
        handle->client = client, proxy->cmap[client].handle = handle;
    }

    return client;
}

/* ...recycle local client-id number */
static inline void xf_client_free(xf_proxy_t *proxy, xf_handle_t *handle)
{
    xf_msg_id_dtype     client = handle->client;

    /* ...push client into head of the free clients list */
    proxy->cmap[client].next = proxy->cmap[0].next;

    /* ...adjust head of free clients */
    proxy->cmap[0].next = client;
}

/* ...lookup client basing on its local id */
static inline xf_handle_t * xf_client_lookup(xf_proxy_t *proxy, xf_msg_id_dtype client)
{
    /* ...client index must be in proper range */
    BUG(client >= XF_CFG_PROXY_MAX_CLIENTS, _x("Invalid client index: %u"), client);

    /* ...check if client index is small */
    if (proxy->cmap[client].next < XF_CFG_PROXY_MAX_CLIENTS)
        return NULL;
    else
        return proxy->cmap[client].handle;
}

/* ...create new client on remote core */
static inline int xf_client_register(xf_proxy_t *proxy, xf_handle_t *handle, xf_id_t id, UWORD32 core)
{
    void           *b = xf_handle_aux(handle);
    xf_user_msg_t   msg;

    /* ...set session-id: source is proxy at App Interface Layer, destination is proxy at DSP Interface Layer */
    msg.id = __XF_MSG_ID(__XF_AP_PROXY(proxy->core), __XF_DSP_PROXY(core));
    msg.opcode = XF_REGISTER;
    msg.buffer = b;
    //msg.length = strlen(id) + 1;
    msg.length = sizeof(handle->mem_pool_type) + strlen(id) + 1;

    /* ...copy component memory pool IDs */
    memcpy(b, handle->mem_pool_type, sizeof(handle->mem_pool_type));

    /* ...copy component identifier */
    //strncpy(b, id, xf_buffer_length(handle->aux));
    strncpy((void *)((UWORD32)b + sizeof(handle->mem_pool_type)), id, strlen(id)+1);

    /* ...execute command synchronously */
    XF_CHK_API(xf_proxy_cmd_exec(proxy, &msg));

    /* ...check operation is successfull */
    XF_CHK_ERR(msg.opcode == XF_REGISTER, XAF_INVALIDVAL_ERR);

    /* ...save received component global client-id */
    handle->id = XF_MSG_SRC(msg.id);

    TRACE(REG, _b("[%p]=[%s:%u:%u]"), handle, id, XF_PORT_CORE(handle->id), XF_PORT_CLIENT(handle->id));

    return 0;
}

/* ...unregister client from DSP Interface Layer */
static inline int xf_client_unregister(xf_proxy_t *proxy, xf_handle_t *handle)
{
    xf_user_msg_t   msg;

    /* ...make sure the client is consistent */
    BUG(proxy->cmap[handle->client].handle != handle, _x("Invalid handle: %p"), handle);

    /* ...set message parameters */
    msg.id = __XF_MSG_ID(__XF_AP_PROXY(proxy->core), handle->id);
    msg.opcode = XF_UNREGISTER;
    msg.buffer = NULL;
    msg.length = 0;

    /* ...synchronously execute command on DSP Interface Layer */
    XF_CHK_API(xf_proxy_cmd_exec(proxy, &msg));

    /* ...opcode must be XF_UNREGISTER - tbd */
    BUG(msg.opcode != XF_UNREGISTER, _x("Invalid opcode: %X"), msg.opcode);

    TRACE(REG, _b("%p[%u:%u] unregistered"), handle, XF_PORT_CORE(handle->id), XF_PORT_CLIENT(handle->id));

    return 0;
}

/* ...allocate shared buffer */
static inline int xf_proxy_buffer_alloc(xf_proxy_t *proxy, UWORD32 length, void **buffer, xf_pool_type_t mem_pool_type, UWORD32 id, xf_buffer_t *b)
{
    UWORD32             core = proxy->core;
    xf_user_msg_t   msg;

    /* ...prepare command parameters */
    msg.id = __XF_MSG_ID(__XF_AP_PROXY(core), __XF_DSP_PROXY(core));
    msg.opcode = XF_ALLOC;

    if(id == XF_POOL_FRMWK_AUX)
    {
        /* ...AUX buffer pool is the 1st to get allocated, hence it doesnt have mem-pool type info (allocated from XAF_MEM_ID_DEV) */
        msg.length = length;
        msg.buffer = NULL;
    }
    else
    {
        msg.length = sizeof(UWORD32)*2;
        msg.buffer = xf_buffer_data(b);

        ((UWORD32*)msg.buffer)[0] = mem_pool_type;  /* ...memory pool type id       */
        ((UWORD32*)msg.buffer)[1] = length;         /* ...length to be allocated    */
    }

    /* ...synchronously execute command on DSP Interface Layer */
    XF_CHK_API(xf_proxy_cmd_exec(proxy, &msg));

    /* ...check if response is valid */
    XF_CHK_ERR(msg.opcode == XF_ALLOC, XAF_INVALIDVAL_ERR);

    /* ...check if allocation is successful */
    XF_CHK_ERR(msg.buffer != NULL, XAF_MEMORY_ERR);

    /* ...save output parameter */
    *buffer = msg.buffer;

    TRACE(MEM, _b("proxy-%u: allocated [%p:%u]"), core, *buffer, length);

    return 0;
}

/* ... synchronous command execution with proxy lock protection. */
static inline int xf_proxy_cmd_exec_with_lock(xf_proxy_t *proxy, xf_user_msg_t* msg)
{
    int r;

    xf_proxy_lock(proxy);

    /* ...synchronously execute command on DSP Interface Layer */
    if ((r = xf_proxy_cmd_exec(proxy, msg)) != 0)
    {
        TRACE(ERROR, _x("Command failed: %d"), r);
    }

    xf_proxy_unlock(proxy);

    return r;
}

/* ...free shared AP-DSP memory */
static inline int xf_proxy_buffer_free(xf_proxy_t *proxy, void *buffer, UWORD32 length, xf_pool_type_t mem_pool_type, UWORD32 id, xf_buffer_t *b)
{
    UWORD32             core = proxy->core;
    xf_user_msg_t   msg;

    /* ...prepare command parameters */
    msg.id = __XF_MSG_ID(__XF_AP_PROXY(core), __XF_DSP_PROXY(core));
    msg.opcode = XF_FREE;

    //msg.length = length;
    //msg.buffer = buffer;
    msg.length = sizeof(UWORD32)*3;
    msg.buffer = xf_buffer_data(b);
    ((UWORD32*)msg.buffer)[0] = mem_pool_type;      /* ...memory pool type id   */
    ((UWORD32*)msg.buffer)[1] = length;             /* ...length to be freed    */
    ((UWORD32*)msg.buffer)[2] = (UWORD32)buffer;    /* ...pointer to be freed   */

    /* ...synchronously execute command on DSP Interface Layer */
    XF_CHK_API(xf_proxy_cmd_exec(proxy, &msg));

    /* ...check if response is valid */
    XF_CHK_ERR(msg.opcode == XF_FREE, XAF_INVALIDVAL_ERR);

    TRACE(MEM, _b("proxy-%u: free [%p:%u]"), core, buffer, length);

    return 0;
}

/*******************************************************************************
 * Proxy interface asynchronous receiving thread
 ******************************************************************************/
static void * xf_proxy_thread(void *arg)
{
    xf_proxy_t     *proxy = arg;
    xf_handle_t    *client;
    int             r = 0;
    xf_proxy_msg_t  m;
    xf_user_msg_t   msg;

    /* ...retrieve all responses received */
    while (xf_ipc_wait(&proxy->ipc, 0) == 0) {
        while ((r = xf_ipc_recv(&proxy->ipc, &m, &msg.buffer)) == sizeof(m)) {
            /* ...make sure we have proper core identifier of SHMEM interface */
            BUG(XF_MSG_DST_CORE(m.id) != proxy->core, _x("Invalid session-id: %X (core=%u)"), m.id, proxy->core);

            /* ...make sure translation is successful */
            BUG(msg.buffer == (void *)-1, _x("Invalid buffer address: %08x"), m.address);

            /* ...retrieve information fields */
            msg.id = XF_MSG_SRC(m.id), msg.opcode = m.opcode, msg.length = m.length, msg.error = m.error ;

            TRACE(RSP, _b("R[%016llx]:(%08x,%u,%08x,%d)"), (UWORD64)m.id, m.opcode, m.length, m.address, m.error);

#ifndef XA_DISABLE_EVENT
            if (m.opcode == XF_EVENT)
            {
                /* ...submit the event to application via callback. */
                xf_g_ap->cdata->cb(xf_g_ap->cdata, XF_MSG_SRC_ID(msg.id), *(UWORD32*)m.address, (void *)m.address, m.length, m.error);
            }
            else
#endif
            /* ...lookup component basing on destination port specification */
#ifdef DELAYED_SYNC_RESPONSE
            if (XF_AP_IPC_CLIENT(m.id))
            {
                /* ...put proxy response to local IPC queue */
                xf_proxy_response_put_delayed(proxy, &m);
            }
            else
#endif //DELAYED_SYNC_RESPONSE
            if (XF_AP_CLIENT(m.id) == 0)
            {
                /* ...put proxy response to local IPC queue */
                xf_proxy_response_put(proxy, &m);
            }
            else if ((client = xf_client_lookup(proxy, XF_AP_CLIENT(m.id))) != NULL)
            {
                /* ...client is found; invoke its response callback (must be non-blocking) */
                client->response(client, &msg);
            }
            else
            {
                /* ...client has been disconnected already; drop message */
                TRACE(RSP, _b("Client look-up failed - drop message"));
            }
        }
    }
    TRACE(INIT, _b("IPC proxy[%p] thread terminated: %d"), proxy, r);

    return (void *)(intptr_t)r;
}

/*******************************************************************************
 * HiFi proxy API
 ******************************************************************************/

/* ...open HiFi proxy */
int xf_proxy_init(xf_proxy_t *proxy, UWORD32 core)
{
    UWORD32             i;
    int             r;

    /* ...initialize proxy lock */
    __xf_lock_init(&proxy->lock);

#ifdef DELAYED_SYNC_RESPONSE
    /* ...initialize proxy lock for delayed sync response */
    __xf_lock_init(&proxy->lock_delayed_response);
#endif

    /* ...open proxy IPC interface */
    XF_CHK_API(xf_ipc_open(&proxy->ipc, core));

    /* ...save proxy core - hmm, too much core identifiers - tbd */
    proxy->core = core;

    /* ...line-up all clients into single-linked list */
    for (i = 0; i < XF_CFG_PROXY_MAX_CLIENTS - 1; i++)
    {
        proxy->cmap[i].next = i + 1;
    }

    /* ...tail of the list points back to head (list terminator) */
    proxy->cmap[i].next = 0;
    /* ...initialize thread attributes (joinable, with minimal stack) */
#if defined(HAVE_FREERTOS)
    if ((r = __xf_thread_create(&proxy->thread, xf_proxy_thread, proxy, proxy_thread_name, NULL, proxy->proxy_thread_stack_size, proxy->proxy_thread_priority)) < 0)
#else
    if ((r = __xf_thread_create(&proxy->thread, xf_proxy_thread, proxy, proxy_thread_name, xf_g_ap->proxy_thread_stack, proxy->proxy_thread_stack_size, proxy->proxy_thread_priority)) < 0)
#endif
    {
        TRACE(ERROR, _x("Failed to create polling thread: %d"), r);
        xf_ipc_close(&proxy->ipc, core);
        return r;
    }

    TRACE(INIT, _b("proxy-%u[%p] opened"), core, proxy);

    return 0;
}

/* ...close proxy handle */
void xf_proxy_close(xf_proxy_t *proxy)
{
    UWORD32     core = proxy->core;

    /* ...trigger proxy IPC interface close event */
    xf_ipc_close_set_event(&proxy->ipc, core);

    /* TENA-2117*/
    __xf_thread_join(&proxy->thread, NULL); //wait for the proxy to complete

    /* ...close proxy IPC interface */
    xf_ipc_close(&proxy->ipc, core); //close after proxy thread stops waiting in loop with ipc_wait()

    /* ...terminate proxy thread */
    __xf_thread_destroy(&proxy->thread);

    /* ...destroy proxy lock */
    __xf_lock_destroy(&proxy->lock);

#ifdef DELAYED_SYNC_RESPONSE
    /* ...destroy proxy lock for delayed sync response */
    __xf_lock_destroy(&proxy->lock_delayed_response);
#endif

    TRACE(INIT, _b("proxy-%u[%p] closed"), core, proxy);
}

/*******************************************************************************
 * HiFi component API
 ******************************************************************************/

/* ...open component handle */
int xf_open(xf_proxy_t *proxy, xf_handle_t *handle, xf_id_t id, UWORD32 core, xf_response_cb response)
{
    int     r;

    /* ...retrieve auxiliary control buffer from proxy - need I */
    XF_CHK_ERR(handle->aux = xf_buffer_get(proxy->aux), XAF_MEMORY_ERR);

    /* ...initialize IPC data */
    XF_CHK_API(xf_ipc_data_init(&handle->ipc));

    /* ...register client in interlocked fashion */
    xf_proxy_lock(proxy);

    /* ...allocate local client */
    if (xf_client_alloc(proxy, handle) == 0)
    {
        TRACE(ERROR, _x("client allocation failed"));
        r = XAF_MEMORY_ERR;
    }
    else if ((r = xf_client_register(proxy, handle, id, core)) < 0)
    {
        TRACE(ERROR, _x("client registering failed"));
        xf_client_free(proxy, handle);
    }

    xf_proxy_unlock(proxy);

    /* ...if failed, release buffer handle */
    if (r < 0)
    {
        /* ...operation failed; return buffer back to proxy pool */
        xf_buffer_put(handle->aux), handle->aux = NULL;
    }
    else
    {
        /* ...operation completed successfully; assign handle data */
        handle->response = response;
        handle->proxy = proxy;

        TRACE(INIT, _b("component[%p]:(id=%s,core=%u) created"), handle, id, core);
    }

    return XF_CHK_API(r);
}

/* ...close component handle */
int xf_close(xf_handle_t *handle)
{
    int ret;

    xf_proxy_t *proxy = handle->proxy;

    /* ...do I need to take component lock here? guess no - tbd */

    /* ...buffers and stuff? - tbd */

    /* ...acquire global proxy lock */
    xf_proxy_lock(proxy);

    /* ...unregister component from DSP Interface Layer proxy (ignore result code) */
    ret = xf_client_unregister(proxy, handle);

    if(ret < 0)
    {
        /* ...release global proxy lock */
        xf_proxy_unlock(proxy);

        return ret;
    }

    /* ...recycle client-id afterwards */
    xf_client_free(proxy, handle);

    /* ...release global proxy lock */
    xf_proxy_unlock(proxy);

    /* ...destroy IPC data */
    xf_ipc_data_destroy(&handle->ipc);

    /* ...clear handle data */
    xf_buffer_put(handle->aux), handle->aux = NULL;

    /* ...wipe out proxy pointer */
    handle->proxy = NULL;

    TRACE(INIT, _b("component[%p] destroyed"), handle);

    return ret;
}

/* ...port binding function */
int xf_route(xf_handle_t *src, UWORD32 src_port, xf_handle_t *dst, UWORD32 dst_port, UWORD32 num, UWORD32 size, UWORD32 align)
{
    xf_proxy_t             *proxy = src->proxy;
    xf_buffer_t            *b;
    xf_route_port_msg_t    *m;
    xf_user_msg_t           msg;
    int                     r;

    /* ...sanity checks - proxy pointers are same */
    XF_CHK_ERR(proxy == dst->proxy, XAF_INVALIDVAL_ERR);

    /* ...buffer data is sane */
    //XF_CHK_ERR(num && size && xf_is_power_of_two(align), XAF_INVALIDVAL_ERR);
    XF_CHK_ERR(num && xf_is_power_of_two(align), XAF_INVALIDVAL_ERR);

    /* ...get control buffer */
    XF_CHK_ERR(b = xf_buffer_get(proxy->aux), XAF_MEMORY_ERR);

    /* ...get message buffer */
    m = xf_buffer_data(b);

    /* ...fill-in message parameters */
    //m->src = __XF_PORT_SPEC2(src->id, src_port);
    m->dst = __XF_PORT_SPEC2(dst->id, dst_port);
    m->alloc_number = num;
    m->alloc_size = size;
    m->alloc_align = align;

    /* ...set command parameters */
    msg.id = __XF_MSG_ID(__XF_AP_PROXY(proxy->core), __XF_PORT_SPEC2(src->id, src_port));
    msg.opcode = XF_ROUTE;
    msg.length = sizeof(*m);
    msg.buffer = m;

    /* ...synchronously execute command on DSP Interface Layer */
    r = xf_proxy_cmd_exec_with_lock(proxy, &msg);

    /* ...return buffer to proxy */
    xf_buffer_put(b);

    /* ...check command execution is successful */
    XF_CHK_API(r);

    /* ...check result is successfull */
    XF_CHK_ERR(msg.opcode == XF_ROUTE, XAF_INVALIDVAL_ERR);

    /* ...port binding completed */
    TRACE(GRAPH, _b("[%p]:%u bound to [%p]:%u"), src, src_port, dst, dst_port);

    return msg.error;
}

/* ...port unbinding function */
int xf_unroute(xf_handle_t *src, UWORD32 src_port)
{
    xf_proxy_t             *proxy = src->proxy;
    xf_buffer_t            *b;
    xf_unroute_port_msg_t  *m;
    xf_user_msg_t           msg;
    int                     r;

    /* ...get control buffer */
    XF_CHK_ERR(b = xf_buffer_get(proxy->aux), XAF_MEMORY_ERR);

    /* ...get message buffer */
    m = xf_buffer_data(b);

    /* ...fill-in message parameters */
    //m->src = __XF_PORT_SPEC2(src->id, src_port);

    /* ...set command parameters */
#ifdef DELAYED_SYNC_RESPONSE
    msg.id = __XF_MSG_ID(__XF_AP_IPC_CLIENT(proxy->core, XF_AP_IPC_CLIENT_ID_UNROUTE), __XF_PORT_SPEC2(src->id, src_port));
#else
    msg.id = __XF_MSG_ID(__XF_AP_PROXY(proxy->core), __XF_PORT_SPEC2(src->id, src_port));
#endif
    msg.opcode = XF_UNROUTE;
    msg.length = sizeof(*m);
    msg.buffer = m;

#ifdef DELAYED_SYNC_RESPONSE
    xf_proxy_lock_delayed(proxy);
    /* ...synchronously execute command on DSP Interface Layer, no proxy lock is required */
    if ((r = xf_proxy_cmd_exec(proxy, &msg)) != 0)
    {
        TRACE(ERROR, _x("Command failed: %d"), r);
    }
    xf_proxy_unlock_delayed(proxy);
#else //DELAYED_SYNC_RESPONSE
    /* ...synchronously execute command on DSP Interface Layer */
    r = xf_proxy_cmd_exec_with_lock(proxy, &msg);
#endif //DELAYED_SYNC_RESPONSE

    /* ...return buffer to proxy */
    xf_buffer_put(b);

    /* ...check command execution is successfull */
    XF_CHK_API(r);

    /* ...check result is successful */
    XF_CHK_ERR(msg.opcode == XF_UNROUTE, XAF_INVALIDVAL_ERR);

    /* ...port unbinding completed */
    TRACE(GRAPH, _b("[%p]:%u unbound"), src, src_port);

    return msg.error;
}

#ifndef XA_DISABLE_EVENT
int xf_create_event_channel(xf_handle_t *src, UWORD32 src_config_param, xf_handle_t *dst, UWORD32 dst_config_param, UWORD32 num, UWORD32 size, UWORD32 align)
{
    xf_proxy_t             *proxy = src->proxy;
    xf_buffer_t            *b;
    xf_event_channel_msg_t *m;
    xf_user_msg_t           msg;
    int                     r;

    /* ...buffer data is sane */
    XF_CHK_ERR(num && xf_is_power_of_two(align), XAF_INVALIDVAL_ERR);

    /* ...get control buffer */
    XF_CHK_ERR(b = xf_buffer_get(proxy->aux), XAF_MEMORY_ERR);

    /* ...get message buffer */
    m = xf_buffer_data(b);

    /* ...fill-in message parameters, port is irrelevant so default value 0 */
    m->src              = __XF_PORT_SPEC2(src->id, 0);
    m->src_cfg_param    = src_config_param;

    if (dst == NULL)
    {
        m->dst          = __XF_AP_PROXY(proxy->core);
    }
    else
    {
        m->dst          = __XF_PORT_SPEC2(dst->id, 0);
    }

    m->dst_cfg_param    = dst_config_param;
    m->alloc_number     = num;
    m->alloc_size       = size;
    m->alloc_align      = align;

    /* ...set command parameters */
    msg.id = __XF_MSG_ID(__XF_AP_PROXY(proxy->core), __XF_PORT_SPEC2(src->id, 0));
    msg.opcode = XF_EVENT_CHANNEL_CREATE;
    msg.length = sizeof(*m);
    msg.buffer = m;

    /* ...synchronously execute command on DSP Interface Layer */
    r = xf_proxy_cmd_exec_with_lock(proxy, &msg);

    /* ...return buffer to proxy */
    xf_buffer_put(b);

    /* ...check command execution is successful */
    XF_CHK_API(r);

    /* ...check result is successfull */
    XF_CHK_ERR(msg.opcode == XF_EVENT_CHANNEL_CREATE, XAF_INVALIDVAL_ERR);

    TRACE(INIT, _b("Event channel created"));

    return XAF_NO_ERR;
}

int xf_delete_event_channel(xf_handle_t *src, UWORD32 src_config_param, xf_handle_t *dst, UWORD32 dst_config_param)
{
    xf_proxy_t                      *proxy = src->proxy;
    xf_buffer_t                     *b;
    xf_event_channel_delete_msg_t   *m;
    xf_user_msg_t                   msg;
    int                             r;

    /* ...get control buffer */
    XF_CHK_ERR(b = xf_buffer_get(proxy->aux), XAF_MEMORY_ERR);

    /* ...get message buffer */
    m = xf_buffer_data(b);

    /* ...fill-in message parameters */
    m->src              = __XF_PORT_SPEC2(src->id, 0);
    m->src_cfg_param    = src_config_param;
    m->dst              = __XF_AP_PROXY(proxy->core);
    m->dst_cfg_param    = dst_config_param;

    /* ...set command parameters */
    msg.id      = __XF_MSG_ID(__XF_AP_PROXY(proxy->core), __XF_PORT_SPEC2(src->id, 0));
    msg.opcode  = XF_EVENT_CHANNEL_DELETE;
    msg.length  = sizeof(*m);
    msg.buffer  = m;

    /* ...synchronously execute command on DSP Interface Layer */
    r = xf_proxy_cmd_exec_with_lock(proxy, &msg);

    /* ...return buffer to proxy */
    xf_buffer_put(b);

    /* ...check command execution is successful */
    XF_CHK_API(r);

    /* ...check result is successfull */
    XF_CHK_ERR(msg.opcode == XF_EVENT_CHANNEL_DELETE, XAF_INVALIDVAL_ERR);

    return XAF_NO_ERR;
}
#endif /* XA_DISABLE_EVENT */

/* ...send a command message to component */
int xf_command(xf_handle_t *handle, UWORD32 port, UWORD32 opcode, void *buffer, UWORD32 length)
{
    xf_proxy_t     *proxy = handle->proxy;
    xf_proxy_msg_t  msg;

    /* ...fill-in message parameters */
    msg.id = __XF_MSG_ID(__XF_AP_CLIENT(proxy->core, handle->client), __XF_PORT_SPEC2(handle->id, port));
    msg.opcode = opcode;
    msg.length = length;
    msg.error  = 0;
    XF_CHK_ERR((msg.address = xf_proxy_b2a(proxy, buffer)) != XF_PROXY_BADADDR, XAF_INVALIDVAL_ERR);

    TRACE(CMD, _b("[%p]:[%016llx]:(%08x,%u,%p,%d)"), handle, (UWORD64)msg.id, opcode, length, buffer, msg.error);

    /* ...pass command to IPC layer */
    return XF_CHK_API(xf_ipc_send(&proxy->ipc, &msg, buffer));
}

/* ...port pause function */
int xf_pause(xf_handle_t *comp, WORD32 port)
{
    xf_proxy_t             *proxy = comp->proxy;
    xf_user_msg_t           msg;

    /* ...set command parameters */
    msg.id = __XF_MSG_ID(__XF_AP_PROXY(proxy->core), __XF_PORT_SPEC2(comp->id, port));
    msg.opcode = XF_PAUSE;
    msg.length = 0;
    msg.buffer = NULL;

    /* ...synchronously execute command on DSP Interface Layer */
    XF_CHK_API(xf_proxy_cmd_exec_with_lock(proxy, &msg));

    /* ...check result is successful */
    XF_CHK_ERR(msg.opcode == XF_PAUSE, XAF_INVALIDVAL_ERR);

	return msg.error;
}

/* ...port resume function */
int xf_resume(xf_handle_t *comp, WORD32 port)
{
    xf_proxy_t             *proxy = comp->proxy;
    xf_user_msg_t           msg;

    /* ...set command parameters */
    msg.id = __XF_MSG_ID(__XF_AP_PROXY(proxy->core), __XF_PORT_SPEC2(comp->id, port));
    msg.opcode = XF_RESUME;
    msg.length = 0;
    msg.buffer = NULL;

    /* ...synchronously execute command on DSP Interface Layer */
    XF_CHK_API(xf_proxy_cmd_exec_with_lock(proxy, &msg));

    /* ...check result is successful */
    XF_CHK_ERR(msg.opcode == XF_RESUME, XAF_INVALIDVAL_ERR);

	return msg.error;
}

static inline int xf_prepare_ext_config(WORD32 *p_param, WORD32 num_param, void *buffer, UWORD32 set_cfg_flag /* 1 for set-cfg, 0 for get-cfg */)
{
    UWORD32                 message_size_total = 0;
    UWORD32                 desc_len;
    WORD32                 i, k;
    xaf_ext_buffer_t *app_ext_buf, *xf_ext_buf;

    /* ...Querying buffer needed for sending message to DSP layer */
    xf_ext_param_msg_t *smsg = (xf_ext_param_msg_t *)buffer;

    for(i = 0, k = 0; i < num_param; i++, k += 2)
    {
        smsg->desc.id = p_param[k + XA_EXT_CFG_ID_OFFSET];

        app_ext_buf   = (xaf_ext_buffer_t *) p_param[k + XA_EXT_CFG_BUF_PTR_OFFSET];

        /* ...4 bytes to hold the buffer pointer in smsg */
        desc_len = sizeof(UWORD32);

        if (XAF_CHK_EXT_PARAM_FLAG(app_ext_buf->ext_config_flags, XAF_EXT_PARAM_FLAG_OFFSET_ZERO_COPY)) //(zero_copy_flag)
        {
            /* ...zero copy - assign the buffer from application directly */
            *(UWORD32 *)smsg->data = (UWORD32) app_ext_buf;
        }
        else
        {
            /* ...non zero copy */
            xf_ext_buf    = (xaf_ext_buffer_t *) ((UWORD32)smsg + sizeof(xf_ext_param_msg_t) + sizeof(UWORD32));

            xf_ext_buf->max_data_size    = app_ext_buf->max_data_size;
            xf_ext_buf->valid_data_size  = app_ext_buf->valid_data_size;
            xf_ext_buf->ext_config_flags = app_ext_buf->ext_config_flags;
            xf_ext_buf->data             = (UWORD8 *) ((UWORD32)xf_ext_buf + sizeof(xaf_ext_buffer_t));

            if (set_cfg_flag)
            {
                /* ...copy data bytes from external buffer to internal buffer. */
                memcpy(xf_ext_buf->data, app_ext_buf->data, app_ext_buf->max_data_size);
            }
            else
            {
                /* ...zero fill the buffer before sending out */
                memset(xf_ext_buf->data, 0, xf_ext_buf->max_data_size);
            }

            /* ...4 byte alignment is required for accessing next param structure + sizeof(xaf_ext_buffer_t) */
            desc_len += ((xf_ext_buf->max_data_size + (XAF_4BYTE_ALIGN-1)) & ~(XAF_4BYTE_ALIGN-1)) + sizeof(xaf_ext_buffer_t);

            /* ... ext buffer contains the data */
            *(UWORD32 *)smsg->data = (UWORD32) xf_ext_buf;
        }

        smsg->desc.length = desc_len;

        /* ...for next parameter */
        smsg = (xf_ext_param_msg_t *)((UWORD32)smsg + sizeof(xf_ext_param_msg_t) + desc_len);

        message_size_total += sizeof(xf_ext_param_msg_t) + desc_len;
    }

    return message_size_total;
}

int xf_set_config_with_lock(xf_handle_t *comp, void *buffer, UWORD32 length, WORD32 num_param, WORD32 *p_param, UWORD32 cfg_ext_flag)
{
    xf_proxy_t             *proxy = comp->proxy;
    xf_user_msg_t           msg;
    UWORD32                 message_size_total;
    WORD32                 i, k;

    xf_proxy_lock(proxy);

    if(cfg_ext_flag)
    {
        message_size_total = xf_prepare_ext_config(p_param, num_param, buffer, 1);
        XF_CHK_RANGE_UNLOCK(proxy, message_size_total, sizeof(UWORD32), length);
    }
    else
    {
        xf_set_param_msg_t *smsg = (xf_set_param_msg_t *)buffer;

        k = 0;
        for (i=0; i<num_param; i++)
        {
            smsg->item[i].id    = p_param[k++];
            smsg->item[i].value = p_param[k++];
        }
    }

    {
        int r;
        /* ...set command parameters */
        /* ...tbd - command goes port 0 always, check if okay */
        msg.id = __XF_MSG_ID(__XF_AP_PROXY(proxy->core), __XF_PORT_SPEC2(comp->id, 0));
        msg.opcode = (cfg_ext_flag ? XF_SET_PARAM_EXT : XF_SET_PARAM);
        msg.length = (cfg_ext_flag ? message_size_total : length);
        msg.buffer = buffer;

        /* ...synchronously execute command on DSP Interface Layer */
        if ((r = xf_proxy_cmd_exec(proxy, &msg)) != 0)
        {
            TRACE(ERROR, _x("Command failed: %d"), r);
        }

        /* ...check result is successful */
        XF_CHK_ERR_UNLOCK(proxy, (msg.opcode == (cfg_ext_flag ? XF_SET_PARAM_EXT : XF_SET_PARAM)), XAF_INVALIDVAL_ERR);
    }

    xf_proxy_unlock(proxy);

	return msg.error;
}

int xf_get_config_with_lock(xf_handle_t *comp, void *buffer, UWORD32 length, WORD32 num_param, WORD32 *p_param, UWORD32 cfg_ext_flag)
{
    xf_proxy_t             *proxy = comp->proxy;
    xf_user_msg_t           msg;
    UWORD32                 message_size_total = 0;
    UWORD32                 desc_len;
    int                     i, k;

    xf_proxy_lock(proxy);

    if(cfg_ext_flag)
    {
        message_size_total = xf_prepare_ext_config(p_param, num_param, buffer, 0);
        XF_CHK_RANGE_UNLOCK(proxy, message_size_total, sizeof(UWORD32), length);
    }
    else
    {
        xf_get_param_msg_t  *smsg = (xf_get_param_msg_t *)buffer;

        for (i=0, k=0; i<num_param; i++,k+=2)
        {
            smsg->c.id[i] = p_param[k];
            p_param[k+1] = 0;
        }
    }

    {
        int r;
        /* ...get command parameters */
        /* ...tbd - command goes port 0 always, check if okay */
        msg.id      = __XF_MSG_ID(__XF_AP_PROXY(proxy->core), __XF_PORT_SPEC2(comp->id, 0));
        msg.opcode  = (cfg_ext_flag ? XF_GET_PARAM_EXT : XF_GET_PARAM);
        msg.length  = (cfg_ext_flag ? message_size_total : length);
        msg.buffer  = buffer;

        /* ...synchronously execute command on DSP Interface Layer */
        if ((r = xf_proxy_cmd_exec(proxy, &msg)) != 0)
        {
            TRACE(ERROR, _x("Command failed: %d"), r);
        }

        /* ...check result is successful */
        XF_CHK_ERR_UNLOCK(proxy, (msg.opcode == (cfg_ext_flag ? XF_GET_PARAM_EXT : XF_GET_PARAM)), XAF_INVALIDVAL_ERR);
    }

    /* ...response path, return get-config data to the application */
    if(cfg_ext_flag)
    {
        xf_ext_param_msg_t *smsg = (xf_ext_param_msg_t *)buffer;

        for(i = 0, k = 0; i < num_param; i++, k += 2)
        {
            xaf_ext_buffer_t *app_ext_buf   = (xaf_ext_buffer_t *) p_param[k + XA_EXT_CFG_BUF_PTR_OFFSET];
            XF_CHK_PTR_UNLOCK(proxy, app_ext_buf);

            /* ...4 bytes buffer pointer in smsg */
            desc_len = sizeof(UWORD32);

            if (!XAF_CHK_EXT_PARAM_FLAG(app_ext_buf->ext_config_flags, XAF_EXT_PARAM_FLAG_OFFSET_ZERO_COPY))
            {
                xaf_ext_buffer_t *xf_ext_buf    = (xaf_ext_buffer_t *) ((UWORD32)smsg + sizeof(xf_ext_param_msg_t) + sizeof(UWORD32));

                app_ext_buf->max_data_size      = xf_ext_buf->max_data_size;
                app_ext_buf->valid_data_size    = xf_ext_buf->valid_data_size;
                app_ext_buf->ext_config_flags   = xf_ext_buf->ext_config_flags;

                /* ...copy data bytes from internal to external buffer pointer. */
                memcpy(app_ext_buf->data, xf_ext_buf->data, xf_ext_buf->max_data_size);

                /* ...4 byte alignment is required for accessing next param structure + sizeof(xaf_ext_buffer_t) */
                desc_len += ((xf_ext_buf->max_data_size + (XAF_4BYTE_ALIGN-1)) & ~(XAF_4BYTE_ALIGN-1)) + sizeof(xaf_ext_buffer_t);
            }

            smsg = (xf_ext_param_msg_t *)((UWORD32)smsg + sizeof(xf_ext_param_msg_t) + desc_len);
        }
    }
    else
    {
        xf_get_param_msg_t *smsg = (xf_get_param_msg_t *)buffer;

        for (i=0,k=1; i<num_param; i++,k+=2)
        {
            p_param[k] = smsg->r.value[i];
        }
    }

    xf_proxy_unlock(proxy);

	return msg.error;
}

int xf_set_priorities(xf_proxy_t *proxy, UWORD32 core, UWORD32 n_rt_priorities,
                      UWORD32 rt_priority_base, UWORD32 bg_priority)
{
    xf_user_msg_t msg;
    xf_buffer_t *b;
    xf_set_priorities_msg_t *buffer;
    int         r;

    XF_CHK_ERR(b = xf_buffer_get(proxy->aux), XAF_MEMORY_ERR);

    /* ...get message buffer */
    buffer = xf_buffer_data(b);

    *buffer = (xf_set_priorities_msg_t){
        .n_rt_priorities = n_rt_priorities,
        .rt_priority_base = rt_priority_base,
        .bg_priority = bg_priority,
    };

    /* ...set session-id: source is proxy at App Interface Layer, destination is proxy at DSP Interface Layer */
    msg.id = __XF_MSG_ID(__XF_AP_PROXY(proxy->core), __XF_DSP_PROXY(core));
    msg.opcode = XF_SET_PRIORITIES;
    msg.buffer = buffer;
    msg.length = sizeof(*buffer);

    /* ...execute command synchronously */
    r = xf_proxy_cmd_exec_with_lock(proxy, &msg);

    /* ...return buffer to proxy */
    xf_buffer_put(b);

    /* ...check command execution is successful */
    XF_CHK_API(r);

    /* ...check operation is successfull */
    XF_CHK_ERR(msg.opcode == XF_SET_PRIORITIES, XAF_INVALIDVAL_ERR);

    return 0;
}

/*******************************************************************************
 * Buffer pool API
 ******************************************************************************/

/* ...allocate buffer pool */
int xf_pool_alloc(xf_proxy_t *proxy, UWORD32 number, UWORD32 length, xf_pool_type_t mem_pool_type, xf_pool_t **pool, WORD32 id)
{
    xf_pool_t      *p;
    xf_buffer_t    *b = NULL;
    void           *data;
    int             r, ret;

    /* ...basic sanity checks; number of buffers is positive */
    XF_CHK_ERR(number > 0, XAF_INVALIDVAL_ERR);

    /* ...get properly aligned buffer length */
    length = (length + XF_PROXY_ALIGNMENT - 1) & ~(XF_PROXY_ALIGNMENT - 1);

    /* ...allocate data structure */

    ret = xaf_malloc((void **)&p, (offset_of(xf_pool_t, buffer) + number * sizeof(xf_buffer_t)), id);
    if(ret != XAF_NO_ERR)
        return ret;

    XF_CHK_ERR(p, XAF_MEMORY_ERR);

    /* ...get control buffer */
    if(id != XF_POOL_FRMWK_AUX)
    {
        XF_CHK_ERR(b = xf_buffer_get(proxy->aux), XAF_MEMORY_ERR);
    }

    /* ...issue memory pool allocation request to DSP Interface Layer */
    xf_proxy_lock(proxy);
    r = xf_proxy_buffer_alloc(proxy, number * length, &p->p, mem_pool_type, id, b);
    xf_proxy_unlock(proxy);

    /* ...return buffer to proxy */
    if(id != XF_POOL_FRMWK_AUX)
    {
        xf_buffer_put(b);
    }

    /* ...if operation is failed, do cleanup */
    if (r < 0)
    {
        TRACE(ERROR, _x("failed to allocate buffer: %d"), r);
        xaf_free((void *)p, id);
        return r;
    }
    else
    {
        /* ...set pool parameters */
        p->number = number, p->length = length;
        p->proxy = proxy;
    }

    /* ...create individual buffers and link them into free list */
    for (p->free = b = &p->buffer[0], data = p->p; number > 0; number--, b++)
    {
        /* ...set address of the buffer (no length there) */
        b->address = data;

        /* ...file buffer into the free list */
        b->link.next = b + 1;

        /* ...advance data pointer in contiguous buffer */
        data += length;
    }

    /* ...terminate list of buffers (not too good - tbd) */
    b[-1].link.next = NULL;

    TRACE(BUFFER, _b("[%p]: pool[%p] created: %u * %u"), proxy, p, p->number, p->length);

    /* ...return buffer pointer */
    *pool = p;

    return 0;
}

/* ...buffer pool destruction */
int xf_pool_free(xf_pool_t *pool, WORD32 id, xf_pool_type_t mem_pool_type)
{
    xf_proxy_t     *proxy = pool->proxy;
    int ret;
    xf_buffer_t    *b = NULL;

    /* ...get control buffer */
    XF_CHK_ERR(b = xf_buffer_get(proxy->aux), XAF_MEMORY_ERR);

    /* ...check buffers are all freed - tbd */

    /* ...use global proxy lock for pool operations protection */
    xf_proxy_lock(proxy);

    /* ...release allocated buffer on DSP Interface Layer */
    ret = xf_proxy_buffer_free(proxy, pool->p, pool->length * pool->number, mem_pool_type, id, b);

    /* ...release global proxy lock */
    xf_proxy_unlock(proxy);

    /* ...return buffer to proxy */
    xf_buffer_put(b);

    if(ret < 0)
        return ret;

    /* ...deallocate pool structure itself */
    xaf_free((void *)pool, id);

    TRACE(BUFFER, _b("[%p]::pool[%p] destroyed"), proxy, pool);

    return ret;
}

/* ...get new buffer from a pool */
xf_buffer_t * xf_buffer_get(xf_pool_t *pool)
{
    xf_buffer_t    *b;

    /* ...use global proxy lock for pool operations protection */
    xf_proxy_lock(pool->proxy);

    /* ...take buffer from a head of the free list */
    if ((b = pool->free) != NULL)
    {
        /* ...advance free list head */
        pool->free = b->link.next, b->link.pool = pool;

        TRACE(BUFFER, _b("pool[%p]::get[%p]"), pool, b);
    }
    else
    {
        TRACE(ERROR, _x("aux buffer_get failed"));
    }

    xf_proxy_unlock(pool->proxy);

    return b;
}

/* ...return buffer back to pool */
void xf_buffer_put(xf_buffer_t *buffer)
{
    xf_pool_t  *pool = buffer->link.pool;

    /* ...use global proxy lock for pool operations protection */
    xf_proxy_lock(pool->proxy);

    /* ...put buffer back to a pool */
    buffer->link.next = pool->free, pool->free = buffer;

    TRACE(BUFFER, _b("pool[%p]::put[%p]"), pool, buffer);

    xf_proxy_unlock(pool->proxy);
}
