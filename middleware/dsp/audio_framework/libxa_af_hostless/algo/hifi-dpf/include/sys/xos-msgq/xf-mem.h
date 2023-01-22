/*
* Copyright (c) 2015-2022 Cadence Design Systems Inc.
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
 * xf-mem.h
 *
 * System-specific memory allocator
 *******************************************************************************/

#ifndef __XF_H
#error "xf-mem.h mustn't be included directly"
#endif

 /*******************************************************************************/

/* ...memory allocation metadata */
typedef struct xf_mem_info
{
    void *buf_ptr;
    UWORD32 alloc_size;
} __attribute__((__packed__)) xf_mem_info_t;

#define _MAX(a, b)  (((a) > (b))?(a):(b))
#define XF_MIN_ALIGNMENT 1 
#define XF_MAX_ALIGNMENT 4096

/*******************************************************************************
 * System specific memory pools
 ******************************************************************************/

#if XF_CFG_CORES_NUM > 1
/* ...shared memory pool for communication within DSP-cluster */
//extern xf_mm_pool_t     xf_dsp_shmem_pool;
#endif

/*******************************************************************************
 * Platform-specific SHMEM allocation registering functions
 ******************************************************************************/

/* ...register shmem allocation address */
static inline void xf_shmem_alloc_addref(UWORD32 core, xf_message_t *m)
{
}

/* ...unregister shmem allocation address */
static inline void xf_shmem_alloc_rmref(UWORD32 core, xf_message_t *m)
{
}

/*******************************************************************************
 * API functions
 ******************************************************************************/

/* ...allocate aligned memory on particular core specifying if it is shared */
static inline void * xf_mem_alloc(UWORD32 size, UWORD32 align, UWORD32 core, UWORD32 shared)
{
    UWORD32 aligned_size;
    void *ptr, *aligned_ptr;
    xf_mem_info_t *mem_info;

    XF_CHK_ERR(align <= XF_MAX_ALIGNMENT, NULL);

    /* ... alignment value should be greater than 0 */
    align = _MAX(align, XF_MIN_ALIGNMENT); 

    /* ...need extra bytes to store allocation meta data, also size should be properly aligned */
    aligned_size = XF_MM(size + sizeof(xf_mem_info_t) + align-1);

#if (XF_CFG_CORES_NUM > 1)
    if (shared)
    {
        /* ...cache line aligned size for shmem allocs? */
        aligned_size = (aligned_size + XF_IPC_CACHE_ALIGNMENT-1) & ~(XF_IPC_CACHE_ALIGNMENT-1);

        /* ...if memory is shared, core is dropped?? */
        ptr = xf_ipc_mm_alloc(&XF_SHMEM_IPC_HANDLE(core)->xf_dsp_shmem_pool, aligned_size);
    }
    else
#endif
    {
        ptr = xf_mm_alloc(&XF_CORE_DATA(core)->local_pool, aligned_size);
    }
    if (ptr == NULL) return ptr;

    /* ...align the buffer pointer */
    aligned_ptr = (void *) (((UWORD32)ptr + align-1) & ~(align-1)); 

    /* ...store original buffer pointer and allocated size */
    mem_info = (xf_mem_info_t *) ((UWORD32)aligned_ptr+size);
    mem_info->buf_ptr = ptr;
    mem_info->alloc_size = aligned_size;

    return aligned_ptr;
}

/* ...release allocated memory */
static inline void xf_mem_free(void *p, UWORD32 size, UWORD32 core, UWORD32 shared)
{
    /* ...fetch alignment metadata and free */
    xf_mem_info_t *mem_info = (xf_mem_info_t *) ((UWORD32)p + size);

#if (XF_CFG_CORES_NUM > 1)
    if (shared)
    {
        /* ...if memory is shared, core is dropped?? */
        xf_ipc_mm_free(&(XF_SHMEM_IPC_HANDLE(core))->xf_dsp_shmem_pool, mem_info->buf_ptr, mem_info->alloc_size);
    }
    else
#endif
    {
        xf_mm_free(&XF_CORE_DATA(core)->local_pool, mem_info->buf_ptr, mem_info->alloc_size);
    }

    return;
}

/* ...allocate AP-DSP shared memory */
static inline int xf_shmem_alloc(UWORD32 core, xf_message_t *m)
{
    xf_mm_pool_t   *pool = &XF_CORE_DATA(core)->shared_pool;

    /* ...length is always cache-line aligned */    
    if ((m->buffer = xf_mm_alloc(pool, XF_ALIGNED(m->length))) != NULL)
    {
        /* ...register allocation address */
        xf_shmem_alloc_addref(core, m);

        return 0;
    }
    else
    {
        return XAF_MEMORY_ERR;
    }
}

/* ...free AP-DSP shared memory */
static inline void xf_shmem_free(UWORD32 core, xf_message_t *m)
{
    xf_mm_pool_t   *pool = &XF_CORE_DATA(core)->shared_pool;

    /* ...length is always cache-line aligned */
    xf_mm_free(pool, m->buffer, XF_ALIGNED(m->length));

    /* ...unregister allocation address */
    xf_shmem_alloc_rmref(core, m);
}

/*******************************************************************************
 * Scratch memory management
 ******************************************************************************/

static inline void * xf_scratch_mem_init(UWORD32 core, UWORD32 thread_priority)
{
    /* ...allocate scratch memory from local DSP memory */
    return xf_mem_alloc(XF_CORE_DATA(core)->worker_thread_scratch_size[thread_priority], XF_CFG_CODEC_SCRATCHMEM_ALIGN, core, 0);
}

/*******************************************************************************
 * Helpers - hmm; they are platform-independent - tbd
 ******************************************************************************/

/* ...allocate local buffer */
static inline int xf_mm_alloc_buffer(UWORD32 size, UWORD32 align, UWORD32 core, xf_mm_buffer_t *b)
{
    /* ...allocate memory from proper local pool */
    if ((size = XF_MM(size)) != 0)
        XF_CHK_ERR(b->addr = xf_mem_alloc(size, align, core, 0), XAF_MEMORY_ERR);
    else
        b->addr = NULL;

    /* ...save address */
    b->size = size;
    
    return 0;
}

/* ...free local buffer */
static inline void  xf_mm_free_buffer(xf_mm_buffer_t *b, UWORD32 core)
{
    if (b->addr)
    {
        xf_mem_free(b->addr, b->size, core, 0);
    }
}
