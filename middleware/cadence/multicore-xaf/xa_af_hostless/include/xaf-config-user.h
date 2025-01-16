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
 * Copyright 2024 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __XA_CFG_USER_H__
#define __XA_CFG_USER_H__

/* Constants */
#define XAF_MAX_INBUFS                      2

#define XAF_INBUF_SIZE                      4096

#define XAF_MAX_WORKER_THREADS              16

/* ...maximum number of components to be created in the subsystem */
#define XF_CFG_MAX_COMPS                    16

#ifndef XF_CFG_CORES_NUM
#define XF_CFG_CORES_NUM                    2
#endif //XF_CFG_CORES_NUM

#define XF_CORE_ID_MASTER                   0

#if (XF_CFG_CORES_NUM > 1)
#define XF_EXTERNAL_INTERRUPT_NUMBER	    7
#endif //XF_CFG_CORES_NUM

typedef enum {
    XAF_MEM_ID_DEV = 0,
    XAF_MEM_ID_DEV_MAX = XAF_MEM_ID_DEV,   /* ...ID_DEV_MAX set to the last DEV mem type. To insert additional pools before ID_MAX */
    XAF_MEM_ID_COMP,
    XAF_MEM_ID_COMP_MAX = XAF_MEM_ID_COMP, /* ...ID_COMP_MAX set to the last COMP mem type. To insert additional pools before ID_MAX */
    XAF_MEM_ID_DSP,
    XAF_MEM_ID_DSP_MAX = XAF_MEM_ID_DSP,        /* ...ID_DSP_MAX set to the last DSP mem type. To insert additional pools before ID_MAX */
    XAF_MEM_ID_MAX
} XAF_MEM_ID;

#endif /* __XA_CFG_USER_H__ */
