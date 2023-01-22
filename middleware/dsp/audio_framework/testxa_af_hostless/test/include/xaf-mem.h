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
#ifndef __XAF_MEM_H__
#define __XAF_MEM_H__


typedef struct {
    int persi_mem_dev;
    int num_malloc_dev;
    int persi_mem_comp;
    int num_malloc_comp;
}mem_obj_t;


void* mem_malloc(int size, int id);
void mem_free(void * heap_ptr, int id);
void* mem_init();
void mem_exit();
int mem_get_alloc_size(mem_obj_t* mem_handle, int id);

#include <xtensa/hal.h>
static inline int xaf_update_mpu(void)
{
    int err = XTHAL_SUCCESS;
#if XCHAL_HAVE_EXCLUSIVE
#define EXCLUSIVE_SHARED_MEMORY_MAP_INDEX   25
    xthal_MPU_entry entry[XCHAL_MPU_ENTRIES];
    if((err = xthal_read_map(entry)) == XTHAL_SUCCESS)
    {
        /* ...disable cache on global shared memory */
        /* ...NOTE: MAP_INDEX corresponds to the configuration of the core used, TODO: to generalize it to fit core-configs used. */
        XTHAL_MPU_ENTRY_SET_MEMORY_TYPE(entry[EXCLUSIVE_SHARED_MEMORY_MAP_INDEX], XTHAL_MEM_NON_CACHEABLE);
        xthal_write_map(entry, XCHAL_MPU_ENTRIES);
    }
    else
    {
        fprintf(stderr,"c[%d] xthal_read_map error:%08x\n", XF_CORE_ID, err);
    }
#endif //XCHAL_HAVE_EXCLUSIVE
    return err;
}

static inline int xmp_set_mpu_attrs_region_uncached(void *addr, size_t size)
{
  int err;
  err = xthal_mpu_set_region_attribute(addr, size,
                                       XTHAL_MPU_USE_EXISTING_ACCESS_RIGHTS,
                                        (XTHAL_MEM_NON_CACHEABLE    |
                                         XTHAL_MEM_SYSTEM_SHAREABLE |
                                         XTHAL_MEM_BUFFERABLE),
                                       0);
  if (err != XTHAL_SUCCESS) {
    fprintf(stderr,"Error setting region attribute on c[%d][%08x,%08x], err %d\n",
            XF_CORE_ID, (unsigned int)addr, (unsigned int)(addr+size), err);
  }
  else {
    fprintf(stderr,"Success setting region attribute on c[%d][%08x,%08x], size %d\n",
            XF_CORE_ID, (unsigned int)addr, (unsigned int)(addr+size), size);
  }
  return err;
}

#endif /* __XAF_MEM_H__ */
