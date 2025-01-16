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
 * xf.h
 *
 * Xtensa audio processing framework. Main header
 ******************************************************************************/

#ifdef  __XF_H
#error  "xf.h included more than once"
#endif

#define __XF_H

/*******************************************************************************
 * Global configuration parameters (changing is to be done carefully)
 ******************************************************************************/

/* ...allocate 6 bits for client number per core */
#define XF_CFG_MAX_CLIENTS              (1 << 6)

/* ...allocate 2 bits for core id */
#define XF_CFG_MAX_CORES                (1 << 2)

/* ...allocate 4 bits for maximal number of input/output ports per component */
#define XF_CFG_MAX_PORTS                (1 << 4)

/* ...allocate 6 bits for opcode type */
#define XF_CFG_MAX_CODES                (1 << 6)

/*******************************************************************************
 * Common runtime framework
 ******************************************************************************/

/* ...xaf API prototypes */
#include "xaf-api.h"

/* ... common definitions */
#include "xf-ap-dp-common.h"

/* ...target configuration */
#include "xf-dp_config.h"

/* ...platform run-time */
#include "xf-dp_runtime.h"

/* ...debugging facility */
#include "xf-debug.h"

/* ...flexible lock definition */
#include "xf-flx-lock-if.h"

#if (XF_CFG_CORES_NUM > 1)
#include "xf-mc-ipc.h"
#else
#ifndef XF_IPC_INVALIDATE
#define XF_IPC_INVALIDATE(buf, length)
#endif
#ifndef XF_IPC_FLUSH
#define XF_IPC_FLUSH(buf, length)
#endif
#endif //XF_CFG_CORES_NUM

/* ...generic memory allocator */
#include "xf-mm.h"

/* ...standard opcodes */
#include "xf-dp_opcode.h"

/* ...proxy definitions (shared messages - tbd) */
#include "xf-dp_proxy.h"

/* ...message API */
#include "xf-msg.h"

/* ...tracer data */
#include "xf-trace.h"

/* ...I/O ports */
#include "xf-io.h"

/* ...scheduler definition */
#include "xf-sched.h"

/* ...component definition */
#include "xf-component.h"

/* ...core data */
#include "xf-core.h"

/* ...system abstractions */
#include "xf-sys.h"

/* ...common timebase */
#include "xf-timebase.h"

/* ...probe port */
#include "xf-probe.h"

#if (XF_CFG_CORES_NUM > 1)
/* ...inter-core shared memory */
#include "xf-shared.h"

/* ...inter-core IPC */
#include "xf-ipc-if.h"

/* ...IPC mem interfaces */
#include "xf-mem-ipc.h"
#endif //XF_CFG_CORES_NUM

/* ...memory management */
#include "xf-mem.h"
