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
 * xf-probe.h
 *
 * Probe port handling
 *******************************************************************************/

#ifndef __XF_H
#error "xf-probe.h mustn't be included directly"
#endif

/*******************************************************************************
 * Helper macros
 ******************************************************************************/

/* ...probe aligned buffer size */
#define XF_ALIGNED_PROBE_SIZE(len)         (((len) + 2*sizeof(UWORD32) + 7) & ~7)

/* ... align pointer to 8 bytes */
#define XF_ALIGN_PROBE_8BYTES(ptr)         (((UWORD32)(ptr) + 7) & ~7)

/*******************************************************************************
 * Port flag helper functions
 ******************************************************************************/

static inline UWORD32 xa_port_test_flags(UWORD32 *port_flag, UWORD32 flags)
{
    return (*port_flag & flags);
}

static inline UWORD32 xa_port_set_flags(UWORD32 *port_flag, UWORD32 flags)
{
    return (*port_flag |= flags);
}

static inline UWORD32 xa_port_clear_flags(UWORD32 *port_flag, UWORD32 flags)
{
    return (*port_flag &= ~flags);
}

static inline UWORD32 xa_port_toggle_flags(UWORD32 *port_flag, UWORD32 flags)
{
    return (*port_flag ^= flags);
}

/*******************************************************************************
 * Helper functions
 ******************************************************************************/

/* ...copy probe data */
static inline void * xf_copy_probe_data(VOID *probe_buf, UWORD32 port_num, UWORD32 data_length, VOID *data_buf)
{
    probe_buf = (VOID *) XF_ALIGN_PROBE_8BYTES(probe_buf);
    ((UWORD32 *)probe_buf)[0] = port_num;
    ((UWORD32 *)probe_buf)[1] = data_length;

    probe_buf += (2 * sizeof(UWORD32));

    if (!data_length) return (probe_buf);

    memcpy(probe_buf, data_buf, data_length);

    return (probe_buf + data_length);
}

