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
 * xf-io.c
 *
 * Generic input/output ports handling
 ******************************************************************************/

#define MODULE_TAG                      IO

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "xf-dp.h"

/*******************************************************************************
 * Input port API
 ******************************************************************************/

/* ...initialize input port structure */
int xf_input_port_init(xf_input_port_t *port, UWORD32 size, UWORD32 align, UWORD32 core, UWORD32 mem_pool_type)
{
    /* ...allocate local internal buffer of particular size and alignment */
    if (size)
    {
        /* ...internal buffer is used */
        XF_CHK_ERR(port->buffer = xf_mem_alloc(size, align, core, 0 /* shared */, mem_pool_type), XAF_MEMORY_ERR);
    }
    else
    {
        /* ...no internal buffering is used */
        port->buffer = NULL;
    }

    /* ...initialize message queue */
    xf_msg_queue_init(&port->queue);

    /* ...set buffer size */
    port->length = size;

    /* ...enable input by default */
    port->flags = XF_INPUT_FLAG_ENABLED | XF_INPUT_FLAG_CREATED;

    /* ...mark buffer is empty */
    port->filled = 0, port->access = NULL;

    TRACE(INIT, _b("input-port[%p] created - %p@%u[%u]"), port, port->buffer, align, size);

    return 0;
}

/* ...put message into input port queue; return non-zero if queue was empty */
int xf_input_port_put(xf_input_port_t *port, xf_message_t *m)
{
    /* ...check if input is enabled */
    if ((port->flags & XF_INPUT_FLAG_ENABLED) == 0)
    {
        /* ...input disabled; this is an error condition, likely */
        TRACE(INPUT, _b("input-port[%p] disabled"), port);

        /* ...release the message instantly */
        xf_response_ok(m);

        /* ...buffer has not been accepted - no actions to take */
        return 0;
    }
    else if (m->length == 0)
    {
        /* ...it is forbidden to pass more than one zero-length message */
        BUG(port->flags & XF_INPUT_FLAG_EOS, _x("invalid state: %x"), port->flags);

        /* ...received a message with zero-length; mark end-of-stream condition */
        port->flags ^= XF_INPUT_FLAG_ENABLED | XF_INPUT_FLAG_EOS;

        /* ...still enqueue that zero-length message; it will be processed afterwards */
        TRACE(INPUT, _b("input-port[%p]: zero-length buffer received"), port);
    }
    else
    {
        TRACE(INPUT, _b("input-port[%p]: buffer received - %u bytes"), port, m->length);
    }

    /* ...enqueue message and set access pointer as needed */
    if (xf_msg_enqueue(&port->queue, m))
    {
        /* ...first message put - set access pointer and length */
        port->access = m->buffer, port->remaining = m->length;

        /* ...return non-zero to indicate the first buffer is placed into port */
        return 1;
    }
    else
    {
        /* ...subsequent message placed into buffer */
        return 0;
    }
}

/* ...internal helper - input message completion */
static inline int xf_input_port_complete(xf_input_port_t *port)
{
    /* ...dequeue message from queue */
    xf_message_t   *m = xf_msg_dequeue(&port->queue);

    /* ...message cannot be NULL */
    BUG(m == NULL, _x("invalid port state"));

    /* ...complete current message (EMPTY-THIS-BUFFER always; no length adjustment) */
    xf_response(m);

    /* ...set up next head */
    if ((m = xf_msg_queue_head(&port->queue)) != NULL)
    {
        /* ...set new access pointers */
        port->access = m->buffer, port->remaining = m->length;

        /* ...return indication that there is an input message */
        return 1;
    }
    else
    {
        /* ...no more messages; reset access pointer */
        port->access = NULL;

        /* ...return indication that input port has no data available */
        return 0;
    }
}

/* ...fill-in required amount of data into input port buffer */
int xf_input_port_fill(xf_input_port_t *port)
{
    UWORD32     filled = port->filled;
    UWORD32     remaining = port->remaining;
    UWORD32     copied = 0;
    WORD32     n;

    if (xf_input_port_bypass(port))
    {
        /* ...port is in bypass mode; advance access pointer */
        if (port->remaining == 0)
        {
            /* ...if there is no message pending, bail out */
            if (!xf_msg_queue_head(&port->queue))
            {
                TRACE(INPUT, _b("Input-port-bypass: no message ready"));
                return 0;
            }

            /* ...check if end-of-stream flag is set */
            if (!port->access)
            {
                BUG((port->flags & XF_INPUT_FLAG_EOS) == 0, _x("port[%p]: invalid state: %x"), port, port->flags);

                /* ...mark stream is completed */
                port->flags ^= XF_INPUT_FLAG_EOS | XF_INPUT_FLAG_DONE;

                TRACE(INPUT, _b("Input-port-bypass[%p]: setting port flag DONE"), port);
                return 0;
            }
        }
        return (port->remaining); /* non-zero value indicates fill is success */
    }

    /* ...function shall not be called if no internal buffering is used */
    BUG(xf_input_port_bypass(port), _x("Invalid transaction"));

    /* ...if there is no message pending, bail out */
    if (!xf_msg_queue_head(&port->queue))
    {
        TRACE(INPUT, _b("No message ready"));
        return 0;
    }

    /* ...calculate total amount of bytes we need to copy */
    n = (WORD32)(port->length - filled);

    /* ...get at most "n" bytes from input message(s) buffer(s) */
    while (n > 0)
    {
        UWORD32     k;

        /* ...determine the size of the chunk to copy */
        ((k = remaining) > (UWORD32)n ? k = (UWORD32)n : 0);

        /* ...process zero-length input message separately */
        if (k == 0)
        {
            /* ...end-of-stream condition must be set */
            BUG((port->flags & XF_INPUT_FLAG_EOS) == 0, _x("port[%p]: invalid state: %x"), port, port->flags);

            /* ...mark stream is completed */
            port->flags ^= XF_INPUT_FLAG_EOS | XF_INPUT_FLAG_DONE;

            /* ...reset total amount of bytes to fill */
            n = 0;

            /* ...do not release message yet */
            TRACE(INPUT, _b("input-port[%p] done"), port);

            /* ...and break the loop */
            break;
        }

        /* ...buffer must be set */
        BUG(!port->access, _x("invalid port state"));

        /* ...get required amount from input buffer */
        memcpy(port->buffer + filled, port->access, k), port->access += k;

        /* ...advance buffer positions */
        filled += k, copied += k, n -= k;

        /* ...check if input buffer is processed completely */
        if ((remaining -= k) == 0)
        {
            if (!xf_input_port_complete(port))
            {
                /* ...no more input messages; break the loop */
                break;
            }
            else
            {
                /* ...update remaining counter */
                remaining = port->remaining;
            }
        }
    }

    /* ...update buffer positions */
    port->filled = filled, port->remaining = remaining;

    /* ...return indicator whether input buffer is prefilled */
    return (n == 0);
}

/* ...consume input buffer data */
void xf_input_port_consume(xf_input_port_t *port, UWORD32 n)
{
    /* ...check whether input port is in bypass mode */
    if (xf_input_port_bypass(port))
    {
        if (port->remaining >= n)
        {
            /* ...port is in bypass mode; advance access pointer */
            port->remaining -= n;

            /* ...advance message buffer pointer */
            port->access += n;
        }
        else
        {
            TRACE(CRITICAL, _b("input-port[%p] consumed %d is greater than available %d"), port, n, port->remaining);
            port->access += port->remaining;

            port->remaining = 0;
        }

        if (port->remaining == 0)
        {
            if (xf_msg_queue_head(&port->queue))
            {
                /* ...complete message, send the input buffer back and try to rearm input port */
                xf_input_port_complete(port);
            }
        }
    }
    else if (port->filled > n)
    {
        UWORD32     k = port->filled - n;

        /* ...move tail of buffer to the head */
        memmove(port->buffer, port->buffer + n, k); /* ...instead of memcpy, TENA-4254 */

        /* ...adjust filled position */
        port->filled = k;
    }
    else
    {
        /* ...entire buffer is consumed; reset fill level */
        port->filled = 0;
    }
}

/* ...purge input port queue */
void xf_input_port_purge(xf_input_port_t *port)
{
    xf_message_t   *m;

    /* ...bail out early if port is not created */
    if (!xf_input_port_created(port))   return;

    /* ...free all queued messages with generic "ok" response */
    while ((m = xf_msg_dequeue(&port->queue)) != NULL)
    {
        xf_response_ok(m);
    }

    /* ...reset internal buffer position */
    port->filled = 0, port->access = NULL;

    /* ...reset port flags */
    port->flags = (port->flags & ~__XF_INPUT_FLAGS(~0)) | XF_INPUT_FLAG_ENABLED | XF_INPUT_FLAG_CREATED;

    TRACE(INPUT, _b("input-port[%p] purged"), port);
}

/* ...save flow-control message for propagated input port purging sequence */
void xf_input_port_control_save(xf_input_port_t *port, xf_message_t *m)
{
    /* ...make sure purging sequence is not active */
    BUG(port->flags & XF_INPUT_FLAG_PURGING, _x("invalid state: %x"), port->flags);

    /* ...place message into internal queue */
    xf_msg_enqueue(&port->queue, m);

    /* ...set port purging flag */
    port->flags ^= XF_INPUT_FLAG_PURGING;

    TRACE(INPUT, _b("port[%p] start purge sequence"), port);
}

/* ...mark flushing sequence is completed */
void xf_input_port_purge_done(xf_input_port_t *port)
{
    xf_message_t   *m;

    /* ...make sure flushing sequence is ongoing */
    BUG((port->flags & XF_INPUT_FLAG_PURGING) == 0, _x("invalid state: %x"), port->flags);

    m = xf_msg_dequeue(&port->queue);

    /* ...message cannot be NULL */
    BUG(m == NULL, _x("invalid port state"));

    /* ...complete saved flow-control message */
    xf_response_ok(m);

    /* ...clear port purging flag */
    port->flags ^= XF_INPUT_FLAG_PURGING;

    TRACE(INPUT, _b("port[%p] purge sequence completed"), port);
}

/* ...destroy input port data */
void xf_input_port_destroy(xf_input_port_t *port, UWORD32 core, UWORD32 mem_pool_type)
{
    /* ...bail out earlier if port is not created */
    if (!xf_input_port_created(port))   return;

    /* ...deallocate input buffer if needed */
    (port->buffer ? xf_mem_free(port->buffer, port->length, core, 0, mem_pool_type), port->buffer = NULL : 0);

    /* ...reset input port flags */
    port->flags = 0;

    TRACE(INIT, _b("input-port[%p] destroyed"), port);
}

/*******************************************************************************
 * Output port API
 ******************************************************************************/

/* ...initialize output port (structure must be zero-initialized) */
int xf_output_port_init(xf_output_port_t *port, UWORD32 size)
{
    /*  ...initialize message queue */
    xf_msg_queue_init(&port->queue);

    /* ...set output buffer length */
    port->length = size;

    /* ...mark port is created */
    port->flags = XF_OUTPUT_FLAG_CREATED | XF_OUTPUT_FLAG_IDLE;

    TRACE(INIT, _b("output-port[%p] initialized"), port);

    return 0;
}

int xf_output_port_route_alloc(xf_output_port_t *port, UWORD32 length, UWORD32 mem_pool_type)
{
    if(!(port->flags & XF_OUTPUT_FLAG_ROUTING) || port->length)
    {
        TRACE(ROUTE, _b("output-port[%p] route:%s buffer_length:%d"), port, (port->length)?"already allocated":"alloc error", port->length);
        return 1;
    }

    UWORD32 i, n;
    xf_message_t   *m = xf_output_port_control_msg(port);
    UWORD64 id = m->id;
    UWORD32 core = XF_MSG_DST_CORE(id);
    UWORD32 shared = XF_MSG_SHARED(id);
    n = port->pool.n-1;

    if ((mem_pool_type >= XAF_MEM_ID_COMP) && (mem_pool_type <= XAF_MEM_ID_COMP_MAX))
    {
        /* ...if routed to component on different core, use DSP mem_pool. */
        mem_pool_type = (shared ? XAF_MEM_ID_DSP: mem_pool_type);
    }

    /* ...allocate required amount of buffers */
    for (i = 1; i <= n; i++)
    {
        /* ...get message from pool (directly; bypass that "get" interface) */
        m = xf_msg_pool_item(&port->pool, i);

        /* ...wipe out message link pointer (debugging) */
        m->next = NULL;

        /* ...set message parameters */
        m->id = id;
        m->opcode = XF_FILL_THIS_BUFFER;
        m->length = length;
        m->buffer = xf_mem_alloc(length, port->align, core, shared, mem_pool_type);
        m->error = 0;

        /* ...if allocation failed, do a cleanup */
        if (!m->buffer)     goto error;

        /* ...place message into output port */
        xf_msg_enqueue(&port->queue, m);
    }

    /* ...save port length */
    port->length = length;

    /* ...mark port is routed */
    port->flags ^= (XF_OUTPUT_FLAG_ROUTING | XF_OUTPUT_FLAG_ROUTED);

    TRACE(ROUTE, _b("output-port[%p] routed: %08x -> %08x. buffer_length:%d mem_pool_type:%d"), port, XF_MSG_DST(id), XF_MSG_SRC(id), length, mem_pool_type);

    return 0;

error:
    /* ...allocation failed; do a cleanup */
    while (--i)
    {
        m = xf_msg_pool_item(&port->pool, i);

        /* ...free item */
        xf_mem_free(m->buffer, port->length, core, shared, mem_pool_type);
    }

    /* ...destroy pool data */
    xf_msg_pool_destroy(&port->pool, core, shared, mem_pool_type);

    /* ...reset message queue (it is empty again) */
    xf_msg_queue_init(&port->queue);

    return XAF_MEMORY_ERR;
}

/* ...route output port */
int xf_output_port_route(xf_output_port_t *port, xf_msg_id_dtype id, UWORD32 n, UWORD32 length, UWORD32 align, UWORD32 mem_pool_type)
{
    UWORD32             core = XF_MSG_DST_CORE(id);
    xf_message_t   *m;
    UWORD32             shared = XF_MSG_SHARED(id);

    /* ...allocate message pool for a port; extra message for control */
    XF_CHK_API(xf_msg_pool_init(&port->pool, n + 1, core, shared, mem_pool_type));

#if 0
    UWORD32             i;
    /* ...allocate required amount of buffers */
    for (i = 1; i <= n; i++)
    {
        /* ...get message from pool (directly; bypass that "get" interface) */
        m = xf_msg_pool_item(&port->pool, i);

        /* ...wipe out message link pointer (debugging) */
        m->next = NULL;

        /* ...set message parameters */
        m->id = id;
        m->opcode = XF_FILL_THIS_BUFFER;
        m->length = length;
        m->buffer = xf_mem_alloc(length, align, core, shared, mem_pool_type);

        /* ...if allocation failed, do a cleanup */
        if (!m->buffer)     goto error;

        /* ...place message into output port */
        xf_msg_enqueue(&port->queue, m);
    }
#endif
    /* ...setup flow-control message */
    m = xf_output_port_control_msg(port);
    m->id = id;
    m->length = 0;
    m->buffer = NULL;
    m->error = 0;

    /* ...wipe out message link pointer (debugging) */
    m->next = NULL;

#if 0
    /* ...save port length */
    port->length = length;

    /* ...mark port is routed */
    port->flags |= XF_OUTPUT_FLAG_ROUTED;
#else
    /* ...save port length, will be updated in buffer allocation function */
    port->length = 0;

    /* ...mark port is routed */
    port->flags |= XF_OUTPUT_FLAG_ROUTING;

    /* ...save port buffer alignment */
    port->align = align;
#endif

    /* ...clear port idle flag */
    port->flags &= ~XF_OUTPUT_FLAG_IDLE;

    TRACE(ROUTE, _b("output-port[%p] in routing: %08x -> %08x"), port, XF_MSG_DST(id), XF_MSG_SRC(id));

    if(length)
    {
        return(xf_output_port_route_alloc(port, length, mem_pool_type));
    }

    return 0;

#if 0
error:
    /* ...allocation failed; do a cleanup */
    while (--i)
    {
        m = xf_msg_pool_item(&port->pool, i);

        /* ...free item */
        xf_mem_free(m->buffer, length, core, shared, mem_pool_type);
    }

    /* ...destroy pool data */
    xf_msg_pool_destroy(&port->pool, core, shared, mem_pool_type);

    /* ...reset message queue (it is empty again) */
    xf_msg_queue_init(&port->queue);

    return XAF_MEMORY_ERR;
#endif
}

/* ...start output port unrouting sequence */
void xf_output_port_unroute_start(xf_output_port_t *port, xf_message_t *m)
{
    /* ...port must be routed */
    BUG(!xf_output_port_routed(port), _x("invalid state: %x"), port->flags);

    /* ...save message in the queue */
    port->unroute = m;

    /* ...put port unrouting flag */
    port->flags |= XF_OUTPUT_FLAG_UNROUTING;
}

/* ...complete port unrouting sequence */
void xf_output_port_unroute_done(xf_output_port_t *port, UWORD32 mem_pool_type)
{
    xf_message_t   *m;

    /* ...make sure we have an outstanding port unrouting sequence */
    BUG(!xf_output_port_unrouting(port), _x("invalid state: %x"), port->flags);

    /* ...retrieve enqueued control-flow message */
    m = port->unroute, port->unroute = NULL;

    /* ...destroy port buffers */
    xf_output_port_unroute(port, mem_pool_type);

    /* ...message cannot be NULL */
    BUG(m == NULL, _x("invalid port state"));

    /* ...and pass response to the caller */
    xf_response_ok(m);
}

/* ...unroute output port and destroy all memory buffers allocated */
void xf_output_port_unroute(xf_output_port_t *port, UWORD32 mem_pool_type)
{
    xf_message_t   *m = xf_output_port_control_msg(port);
    UWORD32             core = XF_MSG_DST_CORE(m->id);
    UWORD32             shared = XF_MSG_SHARED(m->id);
    UWORD32             n = port->pool.n - 1;
    UWORD32             i;

    if ((mem_pool_type >= XAF_MEM_ID_COMP) && (mem_pool_type <= XAF_MEM_ID_COMP_MAX))
    {
        /* ...if routed to component on different core, use DSP mem_pool. */
        mem_pool_type = (shared ? XAF_MEM_ID_DSP: mem_pool_type);
    }

    /* ...free all messages (we are running on "dst" core) */
    for (i = 1; i <= n; i++)
    {
        /* ...directly obtain message item */
        m = xf_msg_pool_item(&port->pool, i);

        /* ...free message buffer (must exist) */
        xf_mem_free(m->buffer, port->length, core, shared, mem_pool_type);
    }

    /* ...destroy pool data */
    xf_msg_pool_destroy(&port->pool, core, shared, mem_pool_type);

    /* ...reset all flags */
    port->flags = XF_OUTPUT_FLAG_CREATED | XF_OUTPUT_FLAG_IDLE;

    /* ...reset message queue (it is empty again) */
    xf_msg_queue_init(&port->queue);

    TRACE(ROUTE, _b("output-port[%p] unrouted"), port);
}

/* ...put next message to the port */
int xf_output_port_put(xf_output_port_t *port, xf_message_t *m)
{
    /* ...in case of port unrouting sequence the flag returned will always be 0 */
    return xf_msg_enqueue(&port->queue, m);
}

/* ...retrieve next message from the port */
void * xf_output_port_data(xf_output_port_t *port)
{
    xf_message_t   *m = xf_msg_queue_head(&port->queue);

    /* ...bail out if there is nothing enqueued */
    if (m == NULL)      return NULL;

    /* ...it is not permitted to access port data when port is being unrouted */
    BUG(xf_output_port_unrouting(port), _x("invalid transaction"));

    /* ...make sure message length is sane */
    BUG(m->length < port->length, _x("Insufficient buffer length: %u < %u"), m->length, port->length);

    /* ...return access buffer pointer */
    return m->buffer;
}

/* ...produce output message marking amount of bytes produced */
int xf_output_port_produce(xf_output_port_t *port, UWORD32 n)
{
    xf_message_t   *m = xf_msg_dequeue(&port->queue);

    /* ...message cannot be NULL */
    BUG(m == NULL, _x("Invalid transaction"));

    /* ...it is not permitted to invoke this when port is being unrouted (or flushed - tbd) */
    BUG(xf_output_port_unrouting(port), _x("invalid transaction"));

    /* ...complete message with specified amount of bytes produced */
    xf_response_data(m, n);

    /* ...clear port idle flag (technically, not needed for unrouted port) */
    port->flags &= ~XF_OUTPUT_FLAG_IDLE;

    /* ...return indication of pending message availability */
    return (xf_msg_queue_head(&port->queue) != NULL);
}

/* ...flush output port */
int xf_output_port_flush(xf_output_port_t *port, UWORD32 opcode)
{
    xf_message_t   *m;

    /* ...if port is routed, we shall pass flush command to sink port */
    if (xf_output_port_routed(port))
    {
        /* ...if port is idle, satisfy immediately */
        if ((port->flags & XF_OUTPUT_FLAG_IDLE)
            && (opcode != XF_FILL_THIS_BUFFER) /* ...TENA-2662 */
        )
        {
            return 1;
        }

        /* ...start flushing sequence if not already started */
        if ((port->flags & XF_OUTPUT_FLAG_FLUSHING) == 0)
        {
            /* ...put flushing flag */
            port->flags ^= XF_OUTPUT_FLAG_FLUSHING;

            /* ...get control message from associated pool */
            m = xf_output_port_control_msg(port);

            /* ...set flow-control operation */
            m->opcode = opcode;

            /* ...message is a command, but source and destination are swapped */
            xf_response(m);
        }

        /* ...zero-result indicates the flushing is in progress */
        return 0;
    }
    else
    {
        /* ...for non-routed port just complete all queued messages */
        while ((m = xf_msg_dequeue(&port->queue)) != NULL)
        {
            /* ...pass generic zero-length "okay" response - tbd */
            xf_response_ok(m);
        }

        /* ...non-zero result indicates the flushing is done */
        return 1;
    }
}

/* ...mark flushing sequence is completed */
void xf_output_port_flush_done(xf_output_port_t *port)
{
    /* ...make sure flushing sequence is ongoing */
    BUG((port->flags & XF_OUTPUT_FLAG_FLUSHING) == 0, _x("invalid state: %x"), port->flags);

    /* ...clear flushing flag and set idle flag */
    port->flags ^= XF_OUTPUT_FLAG_IDLE | XF_OUTPUT_FLAG_FLUSHING;

    TRACE(OUTPUT, _b("port[%p] flush sequence completed"), port);
}

/* ...destroy output port */
void xf_output_port_destroy(xf_output_port_t *port, UWORD32 core, UWORD32 mem_pool_type)
{
    /* ...check if port is routed */
    if (xf_output_port_routed(port))
    {
        /* ...port must be in idle state */
        BUG(!xf_output_port_idle(port), _x("destroying non-idle port[%p]"), port);

        /* ...unroute port */
        xf_output_port_unroute(port, mem_pool_type);
    }

    /* ...reset port flags */
    port->flags = 0;

    TRACE(INIT, _b("output-port[%p] destroyed"), port);
}
