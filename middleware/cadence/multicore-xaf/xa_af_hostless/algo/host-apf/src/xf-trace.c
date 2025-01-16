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
/*
 * Copyright 2023-2024 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*******************************************************************************
 * xf-trace.c
 *
 * Tracing facility
 ******************************************************************************/

#include "xf.h"
#include <sys/time.h>

#if XF_TRACE
#if DUMP_TRACE_TO_BUF
#include <time.h>
#endif
/*******************************************************************************
 * Local data definitions
 ******************************************************************************/

/* ...tracing lock */
static xf_lock_t  xf_trace_mutex;

// `time_t` is an arithmetic time type
time_t now;

/* ...trace buffer to dump trace logs into*/
typedef struct trace_buffer
{
    void *buffer;     // trace buffer
    void *buffer_end; // end of trace buffer
    size_t count;     // Running trace message counter
    void *head;       // pointer to head
} trace_buffer;

trace_buffer tb;

/*******************************************************************************
 * Tracing facility
 ******************************************************************************/

/* ...timestamp function */
static UWORD32 xf_timenow(void)
{
    struct timeval          tv;

    /* ...get current time value */
    gettimeofday(&tv, NULL);

    /* ...wrap over every 100 seconds */
    return (UWORD32)((tv.tv_sec % 100) * 1000000 + tv.tv_usec);
}

/* ...tracing initialization */
void xf_trace_init(const char *banner, void *buffer_p, int size)
{
    int hours, minutes, seconds;
    /* ...initialize tracing lock */
    __xf_lock_init(&xf_trace_mutex);

#if DUMP_TRACE_TO_BUF
    /* ... Route trace logs to console */
    // Obtain current time
    time(&now);
    // local time converts a `time_t` value to calendar time and
    // returns a pointer to a `tm` structure with its members
    // filled with the corresponding values
    struct tm *local = localtime(&now);

    hours = local->tm_hour;         // get hours since midnight (0-23)
    minutes = local->tm_min;        // get minutes passed after the hour (0-59)
    seconds = local->tm_sec;        // get seconds passed after a minute (0-59)

    /* ...output timestamp */

    printf("%08u \r\n", xf_timenow());
    printf("Time now: %02d:%02d:%02d \r\n", hours, minutes, seconds);

    /* ...initialize trace buffer */
    tb.buffer = buffer_p;  //malloc(capacity * size) from testbench;
    if(tb.buffer == NULL)
    {
        // handle error
        printf("Trace buffer can not be allocated\r\n");
    }
    printf("Trace buffer allocated at 0x%x\r\n", (int)buffer_p);
    tb.buffer_end = (char *)tb.buffer + size;
    tb.count = 0;
    tb.head = tb.buffer;

    printf("Trace buffer end at 0x%x\r\n", (int)tb.buffer_end);

#endif

    /* ...output banner */
    xf_trace(banner);
}

/* ...tracing deinitialization */
void xf_trace_deinit()
{
    /* ...destroy tracing lock */
    __xf_lock_destroy(&xf_trace_mutex);
}

/* ...tracing primitive */
int xf_trace(const char *format, ...)
{
    va_list args;
    /* ...get global tracing lock */
    __xf_lock(&xf_trace_mutex);

#if DUMP_TRACE_TO_BUF
    int length = 0;
    char *b = tb.head;

#ifndef DEBUG
    /* This could break timing even in release - disable if necessary */
    b += sprintf(b, "[%08u] ", xf_timenow());
#endif
    b += sprintf(b, "c[%d]", XT_RSR_PRID());

    /* ...output format string */
    va_start(args, format);
    length = vsprintf(b, format, args);
    b += length;
    va_end(args);
    length = b - (char*)tb.head + 1;
    /* ...add terminator */
    *b = '\n';
    /* ...output prepared string */
    {
        tb.head = (char*)tb.head + length;
        if(tb.head >= tb.buffer_end)
        {
            tb.head = tb.buffer;  // Rolling back to the start of the buffer
        }
        tb.count++;
    }
#else
    static char buf[256];
    char       *b = buf;

    /* ...get global tracing lock */
    __xf_lock(&xf_trace_mutex);

    /* ...output timestamp */
    b += sprintf(b, "[%08u] ", xf_timenow());

    b += sprintf(b, "c[%d]", XT_RSR_PRID());

    /* ...output format string */
    va_start(args, format);
    b += vsprintf(b, format, args);
    va_end(args);

    /* ...put terminator */
    *b = '\0';

    /* ...output prepared string */
    __xf_puts(buf);
#endif

    /* ...release tracing lock */
    __xf_unlock(&xf_trace_mutex);

    return 0;
}

#endif  /* XF_TRACE */
