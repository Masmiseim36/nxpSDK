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
 * xf.h
 *
 * Xtensa audio processing framework. Main header for AP-side
 ******************************************************************************/

#ifdef  __XF_H
#error  "xf.h included more than once"
#endif

#define __XF_H

/*******************************************************************************
 * Common runtime framework
 ******************************************************************************/

/* ...xaf API prototypes */
#include "xaf-api.h"

/* ... common definitions */
#include "xf-ap-dp-common.h"

/* ...target configuration */
#include "xf-config.h"

/* ...platform run-time */
#include "xf-runtime.h"

/* ...debugging facility */
#include "xf-debug.h"

/* ...API prototypes */
#include "xf-proto.h"

/* ...standard opcodes */
#include "xf-opcode.h"

/* ...platform-specific IPC layer */
#include "xf-ipc.h"

/* ...proxy definitions */
#include "xf-proxy.h"

