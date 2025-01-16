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
#include <stdio.h>
#include <stdlib.h>

#include "xa_type_def.h"

/* ...debugging facility */
#include "xf-debug.h"
#include "xaf-test.h"
#include "xaf-api.h"
#include "xaf-mem.h"

#include "xaf-fio-test.h"

mem_obj_t g_mem_obj;

void *mem_malloc(int size, int id)
{
    void *heap_ptr = NULL;

    switch(id)
    {
        case XAF_MEM_ID_DEV ... (XAF_MEM_ID_DEV_MAX):
            g_mem_obj.num_malloc_dev[id]++;
            g_mem_obj.persi_mem_dev[id] += size;
            heap_ptr = malloc(size);
            //fprintf(stderr,"core[%d] mem_alloc[%p] id:%s[%d] num_mallocs:%d size:%d\n", XF_CORE_ID, heap_ptr, "DEV", id, g_mem_obj.num_malloc_dev[id], size);
        break;

        case XAF_MEM_ID_COMP ... (XAF_MEM_ID_COMP_MAX):
            g_mem_obj.num_malloc_comp[id]++;
            g_mem_obj.persi_mem_comp[id] += size;
            heap_ptr = malloc(size);
            //fprintf(stderr,"core[%d] mem_alloc[%p] id:%s[%d] num_mallocs:%d size:%d\n", XF_CORE_ID, heap_ptr, "COMP", id, g_mem_obj.num_malloc_comp[id], size);
        break;

        default:
            fprintf(stderr,"ERROR XAF_MEM_ID: core[%d] mem_alloc[%p] id:[%d] size:%d\n", XF_CORE_ID, heap_ptr, id, size);
        break;

    }//switch(i)

    return heap_ptr;
}

void mem_free(void * heap_ptr, int id)
{

    switch(id)
    {
        case XAF_MEM_ID_DEV ... (XAF_MEM_ID_DEV_MAX):
            g_mem_obj.num_malloc_dev[id]--;
            //fprintf(stderr,"core[%d] mem_free[%p] id:%s[%d] num_mallocs:%d\n", XF_CORE_ID, heap_ptr, "DEV", id, g_mem_obj.num_malloc_dev[id]);
        break;

        case XAF_MEM_ID_COMP ... (XAF_MEM_ID_COMP_MAX):
            g_mem_obj.num_malloc_comp[id]--;
            //fprintf(stderr,"core[%d] mem_free[%p] id:%s[%d] num_mallocs:%d\n", XF_CORE_ID, heap_ptr, "COMP", id, g_mem_obj.num_malloc_comp[id]);
        break;

        default:
            fprintf(stderr,"ERROR XAF_MEM_ID: core[%d] mem_free[%p] id:[%d]\n", XF_CORE_ID, heap_ptr, id);
            return;
        break;
    }//switch(i)

    free(heap_ptr);
}

int mem_get_alloc_size(mem_obj_t* mem_handle, int id)
{
    int mem_size = 0;

    switch(id)
    {
        case XAF_MEM_ID_DEV ... (XAF_MEM_ID_DEV_MAX):
            mem_size =  g_mem_obj.persi_mem_dev[id];
        break;

        case XAF_MEM_ID_COMP ... (XAF_MEM_ID_COMP_MAX):
            mem_size = g_mem_obj.persi_mem_comp[id];
        break;

        default:
            /* ...no supported type, ignore */
            fprintf(stderr,"ERROR XAF_MEM_ID: core[%d] get_alloc_size id[%d]\n", XF_CORE_ID, id);
        break;
    }//switch(i)

    return mem_size;
}

void* mem_init()
{
    void* ptr;
    ptr = &g_mem_obj;
    return ptr;
}

void mem_exit()
{
    int id;
    for(id = 0; id < XAF_MEM_ID_MAX; id++)
    {
        switch(id)
        {
            case XAF_MEM_ID_DEV ... (XAF_MEM_ID_DEV_MAX):
                if((g_mem_obj.num_malloc_dev[id] != 0))
                {
                    FIO_PRINTF(stdout,"\nMEMORY LEAK ERROR!!! All the allocated DEV memory is not freed. core[%d] num_malloc:%d id:%d\n\n\n", XF_CORE_ID, g_mem_obj.num_malloc_dev[id], id);
                }
            break;
            case XAF_MEM_ID_COMP ... (XAF_MEM_ID_COMP_MAX):
                if((g_mem_obj.num_malloc_comp[id] != 0))
                {
                    FIO_PRINTF(stdout,"\nMEMORY LEAK ERROR!!! All the allocated COMP memory is not freed. core[%d] num_malloc:%d id:%d\n\n\n", XF_CORE_ID, g_mem_obj.num_malloc_comp[id], id);
                }
            break;
            default:
                /* ...no supported type, ignore */
            break;
        }//switch(id)
    }//for(i)

    return;
}
