/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "nt_system.h"
#include "../source/system/nt_system_prv.h"
#include "../source/system/nt_system_mem_prv.h"

int32_t _nt_mem_init(uint8_t *pool, const uint32_t size)
{
  uint32_t  *data_pointer;
  uint32_t  counter;
  struct nt_kernel * system = _nt_system_get();

  if(pool == NULL)
  {
    return NT_FAILURE;
  }

  /* Is the buffer start address aligned? */
  if(((uint32_t)pool) & 0x03)
  {
    return NT_FAILURE;
  }

  if(system->memory.pool != NULL)
  {
    return NT_FAILURE;
  }

  system->memory.pool = pool;
  system->memory.pool_size = (size & 0xfffffffc); /* Just only 4 byte aligned area is interesting */
  system->memory.free_pointer = pool;

  data_pointer = (uint32_t*)system->memory.pool;
  counter = system->memory.pool_size / 4;

  while(counter--)
  {
     *data_pointer = 0;
     data_pointer++;
  }

  return NT_SUCCESS;
}

void* _nt_mem_alloc(const uint32_t size)
{
  uint32_t  free_size;
  uint8_t   *memory_block;
  uint32_t  pool_size = size;
  struct nt_kernel * system = _nt_system_get();

  if(system->memory.pool == NULL)
  {
    return NULL;
  }

  if(size == 0)
  {
    return NULL;
  }

  /* compute free size */
  free_size = system->memory.pool_size - ((uint32_t)system->memory.free_pointer - (uint32_t)system->memory.pool);

  if(pool_size > free_size)
  {
    return NULL;
  }

  memory_block = system->memory.free_pointer;

  pool_size--;
  pool_size |= 0x03; /* Get full aligned block to 4 bytes */
  pool_size++;

  system->memory.free_pointer += pool_size;

  return memory_block;
}

uint32_t nt_mem_get_free_size(void)
{
  struct nt_kernel * system = _nt_system_get();
  struct nt_mem * mem = &system->memory;
  
  int32_t size = mem->pool_size;
  
  size -= mem->free_pointer - mem->pool;
  
  NT_ASSERT(size >= 0);
  
  return size;
}

int32_t _nt_mem_deinit(void)
{
  struct nt_kernel * system = _nt_system_get();

  system->memory.pool = NULL;
  system->memory.pool_size = 0;
  system->memory.free_pointer = NULL;

  return NT_SUCCESS;
}
