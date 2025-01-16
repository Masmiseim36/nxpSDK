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
 * xf-mem-ipc.c
 *
 * Dynamic ipc-memory allocator implementation (based on rb-tree index)
 ******************************************************************************/

#define MODULE_TAG                      MM_IPC

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "xf-dp.h"

/*******************************************************************************
 * Internal helpers
 ******************************************************************************/

#if (XF_CFG_CORES_NUM > 1)

/* ...initialize block */
static inline xf_mm_block_t * xf_mm_block_init(void *addr, UWORD32 size)
{
    xf_mm_block_t  *b = (xf_mm_block_t *)addr;

    /* ...use 31 available bits of node color to keep aligned size */
    //return b->l_node.color = size, b;
    b->l_node.color = size;
    XF_IPC_FLUSH(b, sizeof(*b));
    return b;
}

/* ...check if the length of the block is less than given */
static inline int xf_mm_block_length_less(xf_mm_block_t *b, UWORD32 size)
{
    /* ...we don't really care about LSB of color */
    return (b->l_node.color < size);
}

/* ...return exact block length */
static inline UWORD32 xf_mm_block_length(xf_mm_block_t *b)
{
    /* ...wipe out least-significant bit from node color */
    return (b->l_node.color & ~1);
}

/* ...increase block length */
static inline UWORD32 xf_mm_block_length_add(xf_mm_block_t *b, UWORD32 size)
{
    /* ...return exact block length after increase */
    //return ((b->l_node.color += size) & ~1);
    UWORD32 ret_val = ((b->l_node.color += size) & ~1);
    XF_IPC_FLUSH(b, sizeof(*b));
    return ret_val;
}

/* ...decrease block length */
static inline UWORD32 xf_mm_block_length_sub(xf_mm_block_t *b, UWORD32 size)
{
    /* ...return exact block length after decrease */
    //return ((b->l_node.color -= size) & ~1);
    UWORD32 ret_val = ((b->l_node.color -= size) & ~1);
    XF_IPC_FLUSH(b, sizeof(*b));
    return ret_val;
}

/*******************************************************************************
 * Internal functions
 ******************************************************************************/

/* ...find best-match node given requested size */
static inline  xf_mm_block_t * xf_mm_find_by_size(xf_shared_mm_pool_t *pool, UWORD32 size)
{
    rb_tree_t *tree = &pool->l_map;
    rb_idx_t   p_idx, t_idx;
    xf_mm_block_t *b;

    /* ...find first block having length greater than requested */
    for (p_idx = rb_root(tree); p_idx != rb_null(tree); p_idx = rb_right(tree, p_idx))
    {
        XF_IPC_INVALIDATE(p_idx, sizeof(*p_idx));
        b = container_of(p_idx, xf_mm_block_t, l_node);

        /* ...break upon finding first matching candidate */
        if (!xf_mm_block_length_less(b, size))
            break;
    }

    /* ...bail out if haven't found a block with sufficient size */
    if (p_idx == rb_null(tree))
    {
        b = NULL;
    }
    else
    {
        /* ...try to find better match in left subtree */
        for (t_idx = rb_left(tree, p_idx); t_idx != rb_null(tree); )
        {
            XF_IPC_INVALIDATE(t_idx, sizeof(*t_idx));
            b = container_of(t_idx, xf_mm_block_t, l_node);

            /* ...check the size of the block */
            if (!xf_mm_block_length_less(b, size))
            {
                /* ...update best match */
                p_idx = t_idx;

                /* ...and check if we have anything better in left sbtree */
                t_idx = rb_left(tree, t_idx);
            }
            else
            {
                /* ...move towards higher block sizes in that subtree */
                t_idx = rb_right(tree, t_idx);
            }
        }

        /* ...p_idx is our best choice */
        b = container_of(p_idx, xf_mm_block_t, l_node);
    }

    return b;
}

/* ...find the neighbours of the block basing on its address */
static void xf_mm_find_by_addr(xf_shared_mm_pool_t *pool, void *addr, xf_mm_block_t **n)
{
    rb_tree_t  *tree = &pool->a_map;
    rb_idx_t    p_idx, l_idx, r_idx;

    /* ...it is not possible to have exact match in this map */
    for (p_idx = rb_root(tree), l_idx = r_idx = NULL; p_idx != rb_null(tree); )
    {
        XF_IPC_INVALIDATE(p_idx, sizeof(*p_idx));
        /* ...only "is less than" comparison is valid (as "a_node" pointer is biased) */
        if ((UWORD32)p_idx < (UWORD32)addr)
        {
            /* ...update lower neighbour */
            l_idx = p_idx;

            /* ...and move towards higher addresses */
            p_idx = rb_right(tree, p_idx);
        }
        else
        {
            /* ...update higher neighbour */
            r_idx = p_idx;

            /* ...and move towards lower addresses */
            p_idx = rb_left(tree, p_idx);
        }
    }

    /* ...translate nodes into blocks */
    n[0] = (l_idx ? container_of(l_idx, xf_mm_block_t, a_node) : NULL);
    n[1] = (r_idx ? container_of(r_idx, xf_mm_block_t, a_node) : NULL);
}

/* ...insert the block into L-map */
static void xf_mm_insert_size(xf_shared_mm_pool_t *pool, xf_mm_block_t *b, UWORD32 size)
{
    rb_tree_t  *tree = &pool->l_map;
    rb_idx_t    p_idx, t_idx;

    /* ...find the parent node for the next block */
    for (p_idx = rb_root(tree); p_idx != rb_null(tree); p_idx = t_idx)
    {
        XF_IPC_INVALIDATE(p_idx, sizeof(*p_idx));
        /* ...check for the size */
        if (xf_mm_block_length_less(container_of(p_idx, xf_mm_block_t, l_node), size))
        {
            /* ...move towards higher addresses */
            if ((t_idx = rb_right(tree, p_idx)) == rb_null(tree))
            {
                /* ...node becomes a right child of parent p */
                rb_set_right(tree, p_idx, &b->l_node);
                XF_IPC_FLUSH(p_idx, sizeof(*p_idx));
                break;
            }
        }
        else
        {
            /* ...move towards lower addresses (ok if exact size match is found) */
            if ((t_idx = rb_left(tree, p_idx)) == rb_null(tree))
            {
                /* ...node becomes a left child of parent p */
                rb_set_left(tree, p_idx, &b->l_node);
                XF_IPC_FLUSH(p_idx, sizeof(*p_idx));
                break;
            }
        }
    }

    /* ...insert node into tree */
    rb_insert(tree, &b->l_node, p_idx);

}

/* ...insert the block into A-map */
static void xf_mm_insert_addr(xf_shared_mm_pool_t *pool, xf_mm_block_t *b)
{
    rb_tree_t  *tree = &pool->a_map;
    rb_idx_t    p_idx, t_idx;

    /* ...find the parent node for the next block */
    for (p_idx = rb_root(tree); p_idx != rb_null(tree); p_idx = t_idx)
    {
        XF_IPC_INVALIDATE(p_idx, sizeof(*p_idx));
        /* ...check for the address (only "is less than" comparison is valid) */
        if ((UWORD32)p_idx < (UWORD32)b)
        {
            /* ...move towards higher addresses */
            if ((t_idx = rb_right(tree, p_idx)) == rb_null(tree))
            {
                /* ...node becomes a right child of parent p */
                rb_set_right(tree, p_idx, &b->a_node);
                XF_IPC_FLUSH(p_idx, sizeof(*p_idx));
                break;
            }
        }
        else
        {
            /* ...move towards lower addresses (by design there cannot be exact match) */
            if ((t_idx = rb_left(tree, p_idx)) == rb_null(tree))
            {
                /* ...node becomes a left child of parent p */
                rb_set_left(tree, p_idx, &b->a_node);
                XF_IPC_FLUSH(p_idx, sizeof(*p_idx));
                break;
            }
        }
    }

    /* ...insert node into tree */
    rb_insert(tree, &b->a_node, p_idx);

}

/*******************************************************************************
 * Entry points
 ******************************************************************************/

/* ...block allocation */
void * xf_ipc_mm_alloc(xf_shared_mm_pool_t *pool, UWORD32 size, UWORD32 mem_pool_type)
{
#if defined (XF_TRACE)
    UWORD32 osize = size;
#endif
    xf_mm_block_t  *b;
    void *addr;

    XF_IPC_INVALIDATE(pool, sizeof(*pool));
    XF_IPC_INVALIDATE(pool->lock, sizeof(xf_ipc_lock_t));
    __xf_ipc_lock(pool->lock);
    XF_IPC_FLUSH(pool->lock, sizeof(xf_ipc_lock_t));

    /* ...find best-fit free block */
    b = xf_mm_find_by_size(pool, size);

    /* ...check block received */
    if (b == NULL)
    {
        __xf_ipc_unlock(pool->lock);
        TRACE(WARNING, _b("proc[%d] ipc Allocation failed - out of memory: pool=%p size=%d"), XT_RSR_PRID(), pool, size);
        return b;
    }

    /* ...remove the block from the L-map */
    rb_delete(&pool->l_map, &b->l_node);

    /* ...update the buffer utilization counters for DSP's shared memory buffers */
    if(pool->addr == XF_SHMEM_IPC_HANDLE(core)->xf_dsp_shmem_buffer[mem_pool_type])
    {
        XF_IPC_INVALIDATE(&(XF_SHMEM_IPC_HANDLE(core))->dsp_shmem_buf_size_curr[mem_pool_type], sizeof((XF_SHMEM_IPC_HANDLE(core))->dsp_shmem_buf_size_curr[mem_pool_type]));
        XF_IPC_INVALIDATE(&(XF_SHMEM_IPC_HANDLE(core))->dsp_shmem_buf_size_peak[mem_pool_type], sizeof((XF_SHMEM_IPC_HANDLE(core))->dsp_shmem_buf_size_peak[mem_pool_type]));
        XF_SHMEM_IPC_HANDLE(core)->dsp_shmem_buf_size_curr[mem_pool_type] += size;
        if (XF_SHMEM_IPC_HANDLE(core)->dsp_shmem_buf_size_peak[mem_pool_type] < XF_SHMEM_IPC_HANDLE(core)->dsp_shmem_buf_size_curr[mem_pool_type])
        {
            XF_SHMEM_IPC_HANDLE(core)->dsp_shmem_buf_size_peak[mem_pool_type] = XF_SHMEM_IPC_HANDLE(core)->dsp_shmem_buf_size_curr[mem_pool_type];
            XF_IPC_FLUSH(&(XF_SHMEM_IPC_HANDLE(core))->dsp_shmem_buf_size_peak[mem_pool_type], sizeof((XF_SHMEM_IPC_HANDLE(core))->dsp_shmem_buf_size_peak[mem_pool_type]));
        }
        XF_IPC_FLUSH(&(XF_SHMEM_IPC_HANDLE(core))->dsp_shmem_buf_size_curr[mem_pool_type], sizeof((XF_SHMEM_IPC_HANDLE(core))->dsp_shmem_buf_size_curr[mem_pool_type]));
    }

    /* ...check if the size is exactly the same as requested */
    if ((size = xf_mm_block_length_sub(b, size)) == 0)
    {
        /* ...the block needs to be removed from the A-map as well */
        rb_delete(&pool->a_map, &b->a_node);

        /* ...entire block goes to user */
        TRACE(INFO, _b("ipc Allocated exact size: pool=%p buffer=%p size=%d"), pool, b, osize);
        addr = (void *) b;
    }
    else
    {
        /* ...insert the block into L-map */
        xf_mm_insert_size(pool, b, size);

        TRACE(INFO, _b("ipc Allocated: pool=%p buffer=%p size=%d"), pool, (void*)b+size, osize);
        /* ...A-map remains intact; tail of the block goes to user */
        addr = (void *) b + size;
    }

    __xf_ipc_unlock(pool->lock);

    XF_IPC_FLUSH(pool, sizeof(*pool));
    XF_IPC_FLUSH(b, sizeof(*b));
    XF_IPC_FLUSH(pool->lock, sizeof(xf_ipc_lock_t));

    return addr;
}

/* ...block deallocation */
void xf_ipc_mm_free(xf_shared_mm_pool_t *pool, void *addr, UWORD32 size, UWORD32 mem_pool_type)
{
#if defined (XF_TRACE)
    UWORD32 osize = size;
#endif
    xf_mm_block_t  *b;
    xf_mm_block_t  *n[2];

    XF_IPC_INVALIDATE(pool, sizeof(*pool));
    XF_IPC_INVALIDATE(pool->lock, sizeof(xf_ipc_lock_t));
    __xf_ipc_lock(pool->lock);
    XF_IPC_FLUSH(pool->lock, sizeof(xf_ipc_lock_t));

    /* ...update the buffer utilization counters for DSP's shared memory buffers */
    if(pool->addr == XF_SHMEM_IPC_HANDLE(core)->xf_dsp_shmem_buffer[mem_pool_type])
    {
        XF_IPC_INVALIDATE(&(XF_SHMEM_IPC_HANDLE(core))->dsp_shmem_buf_size_curr[mem_pool_type], sizeof((XF_SHMEM_IPC_HANDLE(core))->dsp_shmem_buf_size_curr[mem_pool_type]));
        XF_SHMEM_IPC_HANDLE(core)->dsp_shmem_buf_size_curr[mem_pool_type] -= size;
        XF_IPC_FLUSH(&(XF_SHMEM_IPC_HANDLE(core))->dsp_shmem_buf_size_curr[mem_pool_type], sizeof((XF_SHMEM_IPC_HANDLE(core))->dsp_shmem_buf_size_curr[mem_pool_type]));
    }

    XF_IPC_INVALIDATE(addr, sizeof(*b)); //addr, b pair

    b = xf_mm_block_init(addr, size);

    /* ...find block neighbours in A-map */
    xf_mm_find_by_addr(pool, addr, n);

    /* ...check if we can merge block to left neighbour */
    if (n[0])
    {
        if ((void *)n[0] + xf_mm_block_length(n[0]) == addr)
        {
            /* ...merge free block with left neighbour; delete it from L-map */
            rb_delete(&pool->l_map, &n[0]->l_node);

            /* ...adjust block length (block remains in A-map) */
            addr = (void *)(b = n[0]), size = xf_mm_block_length_add(b, size);
        }
        else
        {
            /* ...mark there is no left-merge */
            n[0] = NULL;
        }
    }

    /* ...check if we can merge block to right neighbour */
    if (n[1])
    {
        if ((void *)n[1] == addr + size)
        {
            /* ...merge free block with right neighbour; delete it from L-map */
            rb_delete(&pool->l_map, &n[1]->l_node);

            /* ...adjust block length */
            size = xf_mm_block_length_add(b, xf_mm_block_length(n[1]));

            /* ...check if left merge took place as well */
            if (n[0])
            {
                /* ...left neighbour covers now all three blocks; drop record from A-map */
                rb_delete(&pool->a_map, &n[1]->a_node);
            }
            else
            {
                /* ...fixup tree pointers (equivalent to remove/reinsert the same key) */
                    rb_replace(&pool->a_map, &n[1]->a_node, &b->a_node);
            }
        }
        else
        {
            n[1] = NULL;
        }
    }

    /* ...if any merge has occured, A-map is updated */
    if (n[0] == NULL && n[1] == NULL)
    {
        /* ...add new block into A-map */
        xf_mm_insert_addr(pool, b);
    }

    /* ...add (new or adjusted) block into L-map */
    xf_mm_insert_size(pool, b, size);

    __xf_ipc_unlock(pool->lock);
    XF_IPC_FLUSH(pool, sizeof(*pool));
    XF_IPC_FLUSH(b, sizeof(*b));
    XF_IPC_FLUSH(pool->lock, sizeof(xf_ipc_lock_t));
    TRACE(INFO, _b("proc[%d] ipc Freed: pool=%p addr=%p size=%d"), XT_RSR_PRID(), pool, addr, osize);
}

/* ...initialize memory allocator */
int xf_ipc_mm_init(xf_shared_mm_pool_t *pool, void *addr, UWORD32 size, UWORD32 mem_pool_type)
{
    /* ...check pool alignment validity */
    XF_CHK_ERR(((UWORD32)addr & (sizeof(xf_mm_block_t) - 1)) == 0, XAF_INVALIDVAL_ERR);

    /* ...check pool size validity */
    XF_CHK_ERR(((size) & (sizeof(xf_mm_block_t) - 1)) == 0, XAF_INVALIDVAL_ERR);

    /* ...set pool parameters (need that stuff at all? - tbd) */
    pool->addr = addr, pool->size = size;

    /* ...initialize rb-trees */
    rb_init(&pool->l_map), rb_init(&pool->a_map);

    pool->lock = (xf_ipc_lock_t *)shared_mem_ipc_lock_mem_pool[0]; /* ... pool index is 0 for now. TODO: provide index as global macro/argument as per requirement if multiple pools exists */
    __xf_ipc_lock_init(pool->lock);

    XF_IPC_FLUSH(pool, sizeof(*pool));
    XF_IPC_FLUSH(pool->lock, sizeof(xf_ipc_lock_t));

    /* ..."free" the entire block */
    xf_ipc_mm_free(pool, addr, size, mem_pool_type);

    TRACE(INIT, _b("proc[%d] ipc memory allocator initialized: [%p..%p)"), XT_RSR_PRID(), addr, addr + size);

    /* initialize the buffer size utilization counters for DSP's shared memory buffers */
    if(addr == (XF_SHMEM_IPC_HANDLE(core)->xf_dsp_shmem_buffer[mem_pool_type]))
    {
        XF_SHMEM_IPC_HANDLE(core)->dsp_shmem_buf_size_peak[mem_pool_type] = XF_SHMEM_IPC_HANDLE(core)->dsp_shmem_buf_size_curr[mem_pool_type] = 0;
        XF_IPC_FLUSH(&(XF_SHMEM_IPC_HANDLE(core))->dsp_shmem_buf_size_peak[mem_pool_type], sizeof((XF_SHMEM_IPC_HANDLE(core))->dsp_shmem_buf_size_peak[mem_pool_type]));
        XF_IPC_FLUSH(&(XF_SHMEM_IPC_HANDLE(core))->dsp_shmem_buf_size_curr[mem_pool_type], sizeof((XF_SHMEM_IPC_HANDLE(core))->dsp_shmem_buf_size_curr[mem_pool_type]));
    }

    return 0;
}

/* ...deinitialize memory allocator */
int xf_ipc_mm_deinit(xf_shared_mm_pool_t *pool)
{
    __xf_ipc_lock_destroy(pool->lock);
    return 0;
}
#endif //XF_CFG_CORES_NUM
