/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "../source/system/nt_system_prv.h"
#include "../source/system/nt_system_frmstr_prv.h"
#include "nt.h"

int32_t nt_init(const struct nt_system *system, uint8_t *pool, const uint32_t size)
{
    NT_ASSERT(system != NULL);
    
    int32_t result = NT_SUCCESS;
    
    if (_nt_mem_init(pool, size) < NT_SUCCESS)
    {
      return NT_FAILURE;
    }  
    if (_nt_freemaster_init() < NT_SUCCESS)
    {
      return NT_FAILURE;
    }  
    if ((result = _nt_system_init(system)) < NT_SUCCESS)
    {
      return result;
    }
    if ((result = _nt_system_module_function(NT_SYSTEM_MODULE_INIT)) < NT_SUCCESS)
    {
      return result;
    }
    if ((result = _nt_system_control_function(NT_SYSTEM_CONTROL_INIT)) < NT_SUCCESS)
    {
      return result;
    }
    return result;
}

int32_t nt_trigger(void)
{
    int32_t result = NT_SUCCESS;

    _nt_system_increment_time_counter();
    if (_nt_system_module_function(NT_SYSTEM_MODULE_TRIGGER) < NT_SUCCESS) {
        result = NT_FAILURE;
    }
    if (_nt_system_control_function(NT_SYSTEM_CONTROL_OVERRUN) < NT_SUCCESS) {
        result = NT_FAILURE;
    }
    if (result == NT_FAILURE) {
        /* triggering is faster than measurement/processing */
        _nt_system_invoke_callback(NT_SYSTEM_EVENT_OVERRUN, NULL);
    }

    return result;
}

int32_t nt_task(void)
{
    int32_t ret;
  
    ret = _nt_system_module_function(NT_SYSTEM_MODULE_PROCESS);
    if (ret < NT_SUCCESS) {
        return ret;
    }
    #if (NT_SAFETY_SUPPORT == 1)
    ret = _nt_system_module_function(NT_SYSTEM_MODULE_SAFETY_PROCESS);
    if (ret < NT_SUCCESS) {
        return ret;
    }
    #endif /* NT_SAFETY_SUPPORT */

    ret = _nt_system_control_function(NT_SYSTEM_CONTROL_PROCESS);
    if (ret < NT_SUCCESS) {
        return ret;
    }
    return NT_SUCCESS;
}

void nt_error(char *file, uint32_t line)
{
    /* User's error handling */
    #if (NT_DEBUG != 0) 
    if(_nt_system_get()->error_callback)
    {
      _nt_system_get()->error_callback(file, line);
    }
    #endif
    while (1)
    {
    };
}
