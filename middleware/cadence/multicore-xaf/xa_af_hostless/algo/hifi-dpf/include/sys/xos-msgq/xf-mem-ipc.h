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
 * xf-mem-ipc.h
 *
 *  Created on: Jul 6, 2020
 *      Author: nddivya
 */

#ifndef __XF_MEM_IPC__
#define __XF_MEM_IPC__

/*******************************************************************************
 * shmem access APIs
 ******************************************************************************/

extern void * xf_ipc_mm_alloc(xf_shared_mm_pool_t *pool, UWORD32 size, UWORD32 mem_pool_type);

extern void xf_ipc_mm_free(xf_shared_mm_pool_t *pool, void *addr, UWORD32 size, UWORD32 mem_pool_type);

extern int xf_ipc_mm_init(xf_shared_mm_pool_t *pool, void *addr, UWORD32 size, UWORD32 mem_pool_type);

extern int xf_ipc_mm_deinit(xf_shared_mm_pool_t *pool);

#endif /* __XF_MEM_IPC__ */
