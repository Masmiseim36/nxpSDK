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
 * xf-config.h
 *
 * TCP-shunt target configuration parameters
 *******************************************************************************/

/* ...number of DSP cores */
//#define XF_CFG_CORES_NUM                4

/* ...maximal number of clients supported by proxy */
#define XF_CFG_PROXY_MAX_CLIENTS        256

/* ...size of the shared memory pool (in bytes) */
#define XF_CFG_REMOTE_IPC_POOL_SIZE     (256 << 10)

/* ...alignment for shared buffers */
#if (XF_CFG_CORES_NUM > 1)
#define XF_PROXY_ALIGNMENT              XCHAL_DCACHE_LINESIZE
#else //(XF_CFG_CORES_NUM > 1)
#define XF_PROXY_ALIGNMENT              64
#endif //(XF_CFG_CORES_NUM > 1)
